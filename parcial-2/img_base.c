// Programa de procesamiento de imágenes en C para principiantes en Linux.
// QUÉ: Procesa imágenes PNG (escala de grises o RGB) usando matrices, con soporte
// para carga, visualización, guardado y ajuste de brillo concurrente.
// CÓMO: Usa stb_image.h para cargar PNG y stb_image_write.h para guardar PNG,
// con hilos POSIX (pthread) para el procesamiento paralelo del brillo.
// POR QUÉ: Diseñado para enseñar manejo de matrices, concurrencia y gestión de
// memoria en C, manteniendo simplicidad y robustez para principiantes.
// Dependencias: Descarga stb_image.h y stb_image_write.h desde
// https://github.com/nothings/stb
//   wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
//   wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
//
// Compilar: gcc -o img img_base.c -pthread -lm
// Ejecutar: ./img [ruta_imagen.png]

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
    int ancho;
    int alto;
    int canales;
    unsigned char*** pixeles;
} ImagenInfo;

void liberarImagen(ImagenInfo* info) {
    if (info->pixeles) {
        for (int y = 0; y < info->alto; y++) {
            for (int x = 0; x < info->ancho; x++) {
                free(info->pixeles[y][x]);
            }
            free(info->pixeles[y]);
        }
        free(info->pixeles);
        info->pixeles = NULL;
    }
    info->ancho = 0;
    info->alto = 0;
    info->canales = 0;
}

int cargarImagen(const char* ruta, ImagenInfo* info) {
    int canales;
    unsigned char* datos = stbi_load(ruta, &info->ancho, &info->alto, &canales, 0);
    if (!datos) {
        fprintf(stderr, "Error al cargar imagen: %s\n", ruta);
        return 0;
    }
    info->canales = (canales == 1 || canales == 3) ? canales : 1;

    info->pixeles = (unsigned char***)malloc(info->alto * sizeof(unsigned char**));
    if (!info->pixeles) {
        fprintf(stderr, "Error de memoria al asignar filas\n");
        stbi_image_free(datos);
        return 0;
    }
    for (int y = 0; y < info->alto; y++) {
        info->pixeles[y] = (unsigned char**)malloc(info->ancho * sizeof(unsigned char*));
        if (!info->pixeles[y]) {
            fprintf(stderr, "Error de memoria al asignar columnas\n");
            liberarImagen(info);
            stbi_image_free(datos);
            return 0;
        }
        for (int x = 0; x < info->ancho; x++) {
            info->pixeles[y][x] = (unsigned char*)malloc(info->canales * sizeof(unsigned char));
            if (!info->pixeles[y][x]) {
                fprintf(stderr, "Error de memoria al asignar canales\n");
                liberarImagen(info);
                stbi_image_free(datos);
                return 0;
            }
            for (int c = 0; c < info->canales; c++) {
                info->pixeles[y][x][c] = datos[(y * info->ancho + x) * info->canales + c];
            }
        }
    }

    stbi_image_free(datos);
    printf("Imagen cargada: %dx%d, %d canales (%s)\n", info->ancho, info->alto,
           info->canales, info->canales == 1 ? "grises" : "RGB");
    return 1;
}

void mostrarMatriz(const ImagenInfo* info) {
    if (!info->pixeles) {
        printf("No hay imagen cargada.\n");
        return;
    }
    printf("Matriz de la imagen (primeras 10 filas):\n");
    for (int y = 0; y < info->alto && y < 10; y++) {
        for (int x = 0; x < info->ancho; x++) {
            if (info->canales == 1) {
                printf("%3u ", info->pixeles[y][x][0]);
            } else {
                printf("(%3u,%3u,%3u) ", info->pixeles[y][x][0], info->pixeles[y][x][1],
                       info->pixeles[y][x][2]);
            }
        }
        printf("\n");
    }
    if (info->alto > 10) {
        printf("... (más filas)\n");
    }
}

int guardarPNG(const ImagenInfo* info, const char* rutaSalida) {
    if (!info->pixeles) {
        fprintf(stderr, "No hay imagen para guardar.\n");
        return 0;
    }
    unsigned char* datos1D = (unsigned char*)malloc(info->ancho * info->alto * info->canales);
    if (!datos1D) {
        fprintf(stderr, "Error de memoria al aplanar imagen\n");
        return 0;
    }
    for (int y = 0; y < info->alto; y++) {
        for (int x = 0; x < info->ancho; x++) {
            for (int c = 0; c < info->canales; c++) {
                datos1D[(y * info->ancho + x) * info->canales + c] = info->pixeles[y][x][c];
            }
        }
    }
    int resultado = stbi_write_png(rutaSalida, info->ancho, info->alto, info->canales,
                                   datos1D, info->ancho * info->canales);
    free(datos1D);
    if (resultado) {
        printf("Imagen guardada en: %s (%s)\n", rutaSalida,
               info->canales == 1 ? "grises" : "RGB");
        return 1;
    } else {
        fprintf(stderr, "Error al guardar PNG: %s\n", rutaSalida);
        return 0;
    }
}

typedef struct {
    unsigned char*** pixeles;
    int inicio;
    int fin;
    int ancho;
    int canales;
    int delta;
} BrilloArgs;

void* ajustarBrilloHilo(void* args) {
    BrilloArgs* bArgs = (BrilloArgs*)args;
    for (int y = bArgs->inicio; y < bArgs->fin; y++) {
        for (int x = 0; x < bArgs->ancho; x++) {
            for (int c = 0; c < bArgs->canales; c++) {
                int nuevoValor = bArgs->pixeles[y][x][c] + bArgs->delta;
                bArgs->pixeles[y][x][c] = (unsigned char)(nuevoValor < 0 ? 0 :
                                                          (nuevoValor > 255 ? 255 : nuevoValor));
            }
        }
    }
    return NULL;
}

void ajustarBrilloConcurrente(ImagenInfo* info, int delta) {
    if (!info->pixeles) {
        printf("No hay imagen cargada.\n");
        return;
    }
    const int numHilos = 2;
    pthread_t hilos[numHilos];
    BrilloArgs args[numHilos];
    int filasPorHilo = (int)ceil((double)info->alto / numHilos);

    for (int i = 0; i < numHilos; i++) {
        args[i].pixeles = info->pixeles;
        args[i].inicio = i * filasPorHilo;
        args[i].fin = (i + 1) * filasPorHilo < info->alto ? (i + 1) * filasPorHilo : info->alto;
        args[i].ancho = info->ancho;
        args[i].canales = info->canales;
        args[i].delta = delta;
        pthread_create(&hilos[i], NULL, ajustarBrilloHilo, &args[i]);
    }
    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    printf("Brillo ajustado concurrentemente con %d hilos (%s).\n", numHilos,
           info->canales == 1 ? "grises" : "RGB");
}

/* funcion para convolución*/
typedef struct {
    unsigned char*** src;
    unsigned char*** dst;
    int inicio, fin, ancho, alto, canales;
} ConvolArgs;

void* aplicarConvolucionHilo(void* args) {
    ConvolArgs* a = (ConvolArgs*)args;
    int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    int factor = 9;
    for (int y = a->inicio; y < a->fin; y++) {
        for (int x = 0; x < a->ancho; x++) {
            for (int c = 0; c < a->canales; c++) {
                int suma = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int ny = y + ky, nx = x + kx;
                        if (ny < 0) ny = 0;
                        if (nx < 0) nx = 0;
                        if (ny >= a->alto) ny = a->alto - 1;
                        if (nx >= a->ancho) nx = a->ancho - 1;
                        suma += a->src[ny][nx][c] * kernel[ky+1][kx+1];
                    }
                }
                a->dst[y][x][c] = (unsigned char)(suma / factor);
            }
        }
    }
    return NULL;
}

void aplicarConvolucion(ImagenInfo* info) {
    if (!info->pixeles) { printf("No hay imagen cargada.\n"); return; }
    unsigned char*** dst = (unsigned char***)malloc(info->alto * sizeof(unsigned char**));
    for (int y = 0; y < info->alto; y++) {
        dst[y] = (unsigned char**)malloc(info->ancho * sizeof(unsigned char*));
        for (int x = 0; x < info->ancho; x++) {
            dst[y][x] = (unsigned char*)malloc(info->canales * sizeof(unsigned char));
        }
    }
    int numHilos = 2;
    pthread_t hilos[numHilos];
    ConvolArgs args[numHilos];
    int filasPorHilo = (info->alto + numHilos - 1) / numHilos;
    for (int i = 0; i < numHilos; i++) {
        args[i] = (ConvolArgs){info->pixeles, dst, i*filasPorHilo,
                               (i+1)*filasPorHilo < info->alto ? (i+1)*filasPorHilo : info->alto,
                               info->ancho, info->alto, info->canales};
        pthread_create(&hilos[i], NULL, aplicarConvolucionHilo, &args[i]);
    }
    for (int i = 0; i < numHilos; i++) pthread_join(hilos[i], NULL);

    int canales_prev = info->canales;
    int ancho_prev   = info->ancho;     
    int alto_prev    = info->alto;      

    liberarImagen(info);
    info->pixeles = dst;
    info->ancho   = ancho_prev;
    info->alto    = alto_prev;
    info->canales = canales_prev;       
    printf("Convolución aplicada.\n");
}

/*funcion de rotacion a 90°*/
typedef struct {
    unsigned char*** src;
    unsigned char*** dst;
    int inicio, fin, ancho, alto, canales;
} RotarArgs;

void* rotar90Hilo(void* args) {
    RotarArgs* a = (RotarArgs*)args;
    for (int y = a->inicio; y < a->fin; y++) {
        for (int x = 0; x < a->ancho; x++) {
            for (int c = 0; c < a->canales; c++) {
                a->dst[x][a->alto - 1 - y][c] = a->src[y][x][c];
            }
        }
    }
    return NULL;
}

void rotar90(ImagenInfo* info) {
    if (!info->pixeles) { printf("No hay imagen cargada.\n"); return; }
    unsigned char*** dst = (unsigned char***)malloc(info->ancho * sizeof(unsigned char**));
    for (int y = 0; y < info->ancho; y++) {
        dst[y] = (unsigned char**)malloc(info->alto * sizeof(unsigned char*));
        for (int x = 0; x < info->alto; x++) {
            dst[y][x] = (unsigned char*)malloc(info->canales * sizeof(unsigned char));
        }
    }

    int numHilos = 2;
    pthread_t hilos[numHilos];
    RotarArgs args[numHilos];
    int filasPorHilo = (info->alto + numHilos - 1) / numHilos;
    for (int i = 0; i < numHilos; i++) {
        args[i] = (RotarArgs){info->pixeles, dst, i*filasPorHilo,
                              (i+1)*filasPorHilo < info->alto ? (i+1)*filasPorHilo : info->alto,
                              info->ancho, info->alto, info->canales};
        pthread_create(&hilos[i], NULL, rotar90Hilo, &args[i]);
    }

    for (int i = 0; i < numHilos; i++) pthread_join(hilos[i], NULL);

    int prev_ancho   = info->ancho;   
    int prev_alto    = info->alto;    
    int prev_canales = info->canales; 

    liberarImagen(info);                

    info->pixeles = dst;
    info->ancho   = prev_alto;          
    info->alto    = prev_ancho;     
    info->canales = prev_canales;

    printf("Imagen rotada 90°.\n");
}

/*funcion para detectar bordes con sobel*/
typedef struct {
    unsigned char*** src;
    unsigned char*** dst;
    int inicio, fin, ancho, alto, canales;
} SobelArgs;

void* sobelHilo(void* args) {
    SobelArgs* a = (SobelArgs*)args;
    int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    for (int y = a->inicio; y < a->fin; y++) {
        for (int x = 0; x < a->ancho; x++) {
            int sumX = 0, sumY = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int ny = y+ky, nx = x+kx;
                    if (ny < 0 || ny >= a->alto || nx < 0 || nx >= a->ancho) continue;
                    int val;
                    if (a->canales == 1) val = a->src[ny][nx][0];
                    else val = (a->src[ny][nx][0]+a->src[ny][nx][1]+a->src[ny][nx][2])/3;
                    sumX += val * Gx[ky+1][kx+1];
                    sumY += val * Gy[ky+1][kx+1];
                }
            }
            int mag = (int)sqrt(sumX*sumX + sumY*sumY);
            if (mag > 255) mag = 255;
            if (mag < 0) mag = 0;
            a->dst[y][x][0] = (unsigned char)mag;
        }
    }
    return NULL;
}

void aplicarSobel(ImagenInfo* info) {
    if (!info->pixeles) { printf("No hay imagen cargada.\n"); return; }
    unsigned char*** dst = (unsigned char***)malloc(info->alto * sizeof(unsigned char**));
    for (int y = 0; y < info->alto; y++) {
        dst[y] = (unsigned char**)malloc(info->ancho * sizeof(unsigned char*));
        for (int x = 0; x < info->ancho; x++) {
            dst[y][x] = (unsigned char*)malloc(sizeof(unsigned char));
        }
    }

    int numHilos = 2;
    pthread_t hilos[numHilos];
    SobelArgs args[numHilos];
    int filasPorHilo = (info->alto + numHilos - 1) / numHilos;
    for (int i = 0; i < numHilos; i++) {
        args[i] = (SobelArgs){info->pixeles, dst, i*filasPorHilo,
                              (i+1)*filasPorHilo < info->alto ? (i+1)*filasPorHilo : info->alto,
                              info->ancho, info->alto, info->canales};
        pthread_create(&hilos[i], NULL, sobelHilo, &args[i]);
    }
    for (int i = 0; i < numHilos; i++) pthread_join(hilos[i], NULL);

    int prev_ancho = info->ancho;      
    int prev_alto  = info->alto;       

    liberarImagen(info);               
    info->pixeles = dst;
    info->ancho   = prev_ancho;
    info->alto    = prev_alto; 
    info->canales = 1;         

    printf("Detección de bordes aplicada.\n");
}

/*funcion de escalado de imagen*/

typedef struct {
    unsigned char*** src;
    unsigned char*** dst;
    int inicio, fin, ancho, alto, nuevoAncho, nuevoAlto, canales;
} EscalarArgs;

void* escalarHilo(void* args) {
    EscalarArgs* a = (EscalarArgs*)args;
    for (int y = a->inicio; y < a->fin; y++) {
        for (int x = 0; x < a->nuevoAncho; x++) {
            int srcX = x * a->ancho / a->nuevoAncho;
            int srcY = y * a->alto / a->nuevoAlto;
            for (int c = 0; c < a->canales; c++) {
                a->dst[y][x][c] = a->src[srcY][srcX][c];
            }
        }
    }
    return NULL;
}

void escalarImagen(ImagenInfo* info, int nuevoAncho, int nuevoAlto) {
    if (!info->pixeles) { printf("No hay imagen cargada.\n"); return; }
    unsigned char*** dst = (unsigned char***)malloc(nuevoAlto * sizeof(unsigned char**));
    for (int y = 0; y < nuevoAlto; y++) {
        dst[y] = (unsigned char**)malloc(nuevoAncho * sizeof(unsigned char*));
        for (int x = 0; x < nuevoAncho; x++) {
            dst[y][x] = (unsigned char*)malloc(info->canales * sizeof(unsigned char));
        }
    }

    int numHilos = 2;
    pthread_t hilos[numHilos];
    EscalarArgs args[numHilos];
    int filasPorHilo = (nuevoAlto + numHilos - 1) / numHilos;
    for (int i = 0; i < numHilos; i++) {
        args[i] = (EscalarArgs){info->pixeles, dst, i*filasPorHilo,
                                (i+1)*filasPorHilo < nuevoAlto ? (i+1)*filasPorHilo : nuevoAlto,
                                info->ancho, info->alto, nuevoAncho, nuevoAlto, info->canales};
        pthread_create(&hilos[i], NULL, escalarHilo, &args[i]);
    }
    for (int i = 0; i < numHilos; i++) pthread_join(hilos[i], NULL);

    int canales_prev = info->canales;

    liberarImagen(info);
    info->pixeles = dst;
    info->ancho   = nuevoAncho;
    info->alto    = nuevoAlto;
    info->canales = canales_prev;

    printf("Imagen escalada a %dx%d.\n", nuevoAncho, nuevoAlto);
}


void mostrarMenu() {
    printf("\n--- Plataforma de Edición de Imágenes ---\n");
    printf("1. Cargar imagen PNG\n");
    printf("2. Mostrar matriz de píxeles\n");
    printf("3. Guardar como PNG\n");
    printf("4. Ajustar brillo (+/- valor) concurrentemente\n");
    printf("5. Aplicar convolución (blur)\n");
    printf("6. Rotar imagen 90°\n");
    printf("7. Detección de bordes (Sobel)\n");
    printf("8. Escalar imagen\n");
    printf("9. Salir\n");
    printf("Opción: ");
}

int main(int argc, char* argv[]) {
    ImagenInfo imagen = {0, 0, 0, NULL};
    char ruta[256] = {0};

    if (argc > 1) {
        strncpy(ruta, argv[1], sizeof(ruta) - 1);
        if (!cargarImagen(ruta, &imagen)) {
            return EXIT_FAILURE;
        }
    }

    int opcion;
    while (1) {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida.\n");
            continue;
        }
        while (getchar() != '\n');

        switch (opcion) {
            case 1: {
                printf("Ingresa la ruta del archivo PNG: ");
                if (fgets(ruta, sizeof(ruta), stdin) == NULL) {
                    printf("Error al leer ruta.\n");
                    continue;
                }
                ruta[strcspn(ruta, "\n")] = 0;
                liberarImagen(&imagen);
                if (!cargarImagen(ruta, &imagen)) continue;
                break;
            }
            case 2:
                mostrarMatriz(&imagen);
                break;
            case 3: {
                char salida[256];
                printf("Nombre del archivo PNG de salida: ");
                if (fgets(salida, sizeof(salida), stdin) == NULL) {
                    printf("Error al leer ruta.\n");
                    continue;
                }
                salida[strcspn(salida, "\n")] = 0;
                guardarPNG(&imagen, salida);
                break;
            }
            case 4: {
                int delta;
                printf("Valor de ajuste de brillo: ");
                if (scanf("%d", &delta) != 1) {
                    while (getchar() != '\n');
                    printf("Entrada inválida.\n");
                    continue;
                }
                while (getchar() != '\n');
                ajustarBrilloConcurrente(&imagen, delta);
                break;
            }
            case 5:
                aplicarConvolucion(&imagen);
                break;
            case 6:
                rotar90(&imagen);
                break;
            case 7:
                aplicarSobel(&imagen);
                break;
            case 8: {
                int nuevoAncho, nuevoAlto;
                printf("Nuevo ancho: "); scanf("%d", &nuevoAncho);
                printf("Nuevo alto: "); scanf("%d", &nuevoAlto);
                while (getchar() != '\n');
                escalarImagen(&imagen, nuevoAncho, nuevoAlto);
                break;
            }
            case 9:
                liberarImagen(&imagen);
                printf("¡Adiós!\n");
                return EXIT_SUCCESS;
            default:
                printf("Opción inválida.\n");
        }
    }
    liberarImagen(&imagen);
    return EXIT_SUCCESS;
}
