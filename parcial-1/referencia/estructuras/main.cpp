#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include "persona.h"
#include "generador.h"
#include "monitor.h"

/**
 * Muestra el menú principal de la aplicación.
 * 
 * POR QUÉ: Guiar al usuario a través de las funcionalidades disponibles.
 * CÓMO: Imprimiendo las opciones en consola.
 * PARA QUÉ: Interacción amigable con el usuario.
 */
void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Crear nuevo conjunto de datos";
    std::cout << "\n1. Mostrar resumen de todas las personas";
    std::cout << "\n2. Mostrar detalle completo por índice";
    std::cout << "\n3. Buscar persona por ID";
    std::cout << "\n4. Persona más longeva";
    std::cout << "\n5. Persona con mayor patrimonio";
    std::cout << "\n6. Declarantes de renta por calendario";
    std::cout << "\n7. Ciudades por patrimonio promedio";
    std::cout << "\n8. Porcentaje mayores 60 años por calendario";
    std::cout << "\n9. Mostrar estadísticas de rendimiento";
    std::cout << "\n10. Exportar estadísticas a CSV";
    std::cout << "\n11. Salir";
    std::cout << "\nSeleccione una opción: ";
}

/**
 * Punto de entrada principal del programa.
 * 
 * POR QUÉ: Iniciar la aplicación y manejar el flujo principal.
 * CÓMO: Mediante un bucle que muestra el menú y procesa la opción seleccionada.
 * PARA QUÉ: Ejecutar las funcionalidades del sistema.
 */
int main() {
    srand(time(nullptr)); // Semilla para generación aleatoria
    
    // Puntero inteligente para gestionar la colección de personas
    // POR QUÉ: Evitar fugas de memoria y garantizar liberación automática.
    std::unique_ptr<std::vector<Persona>> personas = nullptr;
    
    Monitor monitor; // Monitor para medir rendimiento
    
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        // Variables locales para uso en los casos
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        
        // Iniciar medición de tiempo y memoria para la operación actual
        monitor.iniciar_tiempo();
        long memoria_inicio = monitor.obtener_memoria();
        
        switch(opcion) {
            case 0: { // Crear nuevo conjunto de datos
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;
                
                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }
                
                // Generar el nuevo conjunto de personas
                auto nuevasPersonas = generarColeccion(n);
                tam = nuevasPersonas.size();
                
                // Mover el conjunto al puntero inteligente (propiedad única)
                personas = std::make_unique<std::vector<Persona>>(std::move(nuevasPersonas));
                
                // Medir tiempo y memoria usada
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;
                
                std::cout << "Generadas " << tam << " personas en " 
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";
                
                // Registrar la operación
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }
                
            case 1: { // Mostrar resumen de todas las personas
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\n=== RESUMEN DE PERSONAS (" << tam << ") ===\n";
                for(size_t i = 0; i < tam; ++i) {
                    std::cout << i << ". ";
                    (*personas)[i].mostrarResumen();
                    std::cout << "\n";
                }
                
                double tiempo_mostrar = monitor.detener_tiempo();
                long memoria_mostrar = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar resumen", tiempo_mostrar, memoria_mostrar);
                break;
            }
                
            case 2: { // Mostrar detalle por índice
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\nIngrese el índice (0-" << tam-1 << "): ";
                if(std::cin >> indice) {
                    if(indice >= 0 && static_cast<size_t>(indice) < tam) {
                        (*personas)[indice].mostrar();
                    } else {
                        std::cout << "Índice fuera de rango!\n";
                    }
                } else {
                    std::cout << "Entrada inválida!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }
                
            case 3: { // Buscar por ID
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                std::cout << "\nIngrese el ID a buscar: ";
                std::cin >> idBusqueda;
                
                if(const Persona* encontrada = buscarPorID(*personas, idBusqueda)) {
                    encontrada->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Buscar por ID", tiempo_busqueda, memoria_busqueda);
                break;
            }
                
            case 4: { //Persona mas longeva
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                std::cout << "\n=== ANÁLISIS DE LONGEVIDAD ===\n";
                std::cout << "1. Persona más longeva del país\n";
                std::cout << "2. Personas más longevas por ciudad\n";
                std::cout << "Seleccione opción: ";
                
                int subOpcion;
                std::cin >> subOpcion;
                
                if (subOpcion == 1) {
                    const Persona* longeva = encontrarPersonaMasLongeva(*personas);
                    if (longeva) {
                        std::cout << "\n PERSONA MÁS LONGEVA DEL PAÍS:\n";
                        longeva->mostrar();
                        std::cout << "Edad aproximada: " << (2025 - std::stoi(longeva->fechaNacimiento.substr(longeva->fechaNacimiento.find_last_of('/') + 1))) << " años\n";
                    }
                } else if (subOpcion == 2) {
                    encontrarLongevasPorCiudad(*personas);
                }
                
                double tiempo_longeva = monitor.detener_tiempo();
                long memoria_longeva = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Análisis longevidad", tiempo_longeva, memoria_longeva);
                break;
            }

            case 5: { // Persona con mas patrimonio
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                std::cout << "\n=== ANÁLISIS DE PATRIMONIO ===\n";
                std::cout << "1. Mayor patrimonio del país\n";
                std::cout << "2. Mayor patrimonio por ciudad\n";
                std::cout << "3. Mayor patrimonio por grupo DIAN\n";
                std::cout << "Seleccione opción: ";
                
                int subOpcion;
                std::cin >> subOpcion;
                
                if (subOpcion == 1) {
                    const Persona* rica = encontrarMayorPatrimonio(*personas);
                    if (rica) {
                        std::cout << "\n MAYOR PATRIMONIO DEL PAÍS:\n";
                        rica->mostrar();
                    }
                } else if (subOpcion == 2) {
                    encontrarMayorPatrimonioPorCiudad(*personas);
                } else if (subOpcion == 3) {
                    encontrarMayorPatrimonioPorGrupoDIAN(*personas);
                }
                
                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Análisis patrimonio", tiempo_patrimonio, memoria_patrimonio);
                break;
            }
                
            case 6: { // Declarantes de renta
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                listarDeclarantesPorGrupo(*personas);
                
                double tiempo_declarantes = monitor.detener_tiempo();
                long memoria_declarantes = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Análisis declarantes", tiempo_declarantes, memoria_declarantes);
                break;
            }

            case 7: { // Ciudades por patrimonio promedio
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                analizarCiudadesPorPatrimonioPromedio(*personas);
                
                double tiempo_ciudades = monitor.detener_tiempo();
                long memoria_ciudades = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Análisis ciudades patrimonio", tiempo_ciudades, memoria_ciudades);
                break;
            }

            case 8: { // Porcentaje mayores 60 años por calendario
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                analizarPorcentajeMayores60PorCalendario(*personas);
                
                double tiempo_mayores60 = monitor.detener_tiempo();
                long memoria_mayores60 = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Análisis mayores 60 años", tiempo_mayores60, memoria_mayores60);
                break;
            }
                
            case 9: // Mostrar estadísticas de rendimiento
                monitor.mostrar_resumen();
                break;
                
            case 10: // Exportar estadísticas a CSV
                monitor.exportar_csv();
                break;
                
            case 11: // Salir
                std::cout << "Saliendo...\n";
                break;
                
            default:
                std::cout << "Opción inválida!\n";
        }
        
        // Mostrar estadísticas de la operación
        if (opcion >= 0 && opcion <= 8) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria() - memoria_inicio;
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }
        
    } while(opcion != 11);
    
    return 0;
}