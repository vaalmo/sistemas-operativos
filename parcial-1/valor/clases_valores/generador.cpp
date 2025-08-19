#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // std::mt19937, std::uniform_real_distribution
#include <vector>
#include <algorithm> // std::find_if, std::sort
#include <map>       // std::map para agrupaciones
#include <iomanip>   // std::fixed, std::setprecision

// Bases de datos para generación realista

// Nombres femeninos comunes en Colombia
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

// Nombres masculinos comunes en Colombia
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Nestor", "Gertridis"
};

// Apellidos comunes en Colombia
const std::vector<std::string> apellidos = {
    "Gómez", "Rodríguez", "Martínez", "López", "García", "Pérez", "González", "Sánchez", "Ramírez", "Torres",
    "Díaz", "Vargas", "Castro", "Ruiz", "Álvarez", "Romero", "Suárez", "Rojas", "Moreno", "Muñoz", "Valencia",
};

// Principales ciudades colombianas
const std::vector<std::string> ciudadesColombia = {
    "Bogotá", "Medellín", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Santa Marta", "Cúcuta", "Ibagué",
    "Manizales", "Pasto", "Neiva", "Villavicencio", "Armenia", "Sincelejo", "Valledupar", "Montería", "Popayán", "Tunja"
};

/**
 * Implementación de generarFechaNacimiento.
 * 
 * POR QUÉ: Simular fechas de nacimiento realistas.
 * CÓMO: Día (1-28), mes (1-12), año (1960-2009).
 * PARA QUÉ: Atributo fechaNacimiento de Persona.
 */
std::string generarFechaNacimiento() {
    int dia = 1 + rand() % 28;       // Día: 1 a 28 (evita problemas con meses)
    int mes = 1 + rand() % 12;        // Mes: 1 a 12
    int anio = 1960 + rand() % 50;    // Año: 1960 a 2009
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

/**
 * Implementación de generarID.
 * 
 * POR QUÉ: Generar identificadores únicos y secuenciales.
 * CÓMO: Contador estático que inicia en 1000000000 y se incrementa.
 * PARA QUÉ: Simular números de cédula.
 */
std::string generarID() {
    static long contador = 1000000000; // Inicia en 1,000,000,000
    return std::to_string(contador++); // Convierte a string e incrementa
}

/**
 * Implementación de randomDouble.
 * 
 * POR QUÉ: Generar números decimales aleatorios en un rango.
 * CÓMO: Mersenne Twister (mejor que rand()) y distribución uniforme.
 * PARA QUÉ: Valores de ingresos, patrimonio, etc.
 */
double randomDouble(double min, double max) {
    static std::mt19937 generator(time(nullptr)); // Semilla basada en tiempo
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

/**
 * Implementación de generarPersona.
 * 
 * POR QUÉ: Crear una persona con datos aleatorios.
 * CÓMO: Seleccionando aleatoriamente de las bases de datos y generando números.
 * PARA QUÉ: Generar datos de prueba.
 */
Persona generarPersona() {
    // Decide si es hombre o mujer
    bool esHombre = rand() % 2;
    
    // Selecciona nombre según género
    std::string nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    // Construye apellido compuesto (dos apellidos aleatorios)
    std::string apellido = apellidos[rand() % apellidos.size()];
    apellido += " ";
    apellido += apellidos[rand() % apellidos.size()];
    
    // Genera los demás atributos
    std::string id = generarID();
    std::string ciudad = ciudadesColombia[rand() % ciudadesColombia.size()];
    std::string fecha = generarFechaNacimiento();
    
    // Genera datos financieros realistas
    double ingresos = randomDouble(10000000, 500000000);   // 10M a 500M COP
    double patrimonio = randomDouble(0, 2000000000);       // 0 a 2,000M COP
    double deudas = randomDouble(0, patrimonio * 0.7);     // Deudas hasta el 70% del patrimonio
    bool declarante = (ingresos > 50000000) && (rand() % 100 > 30); // Probabilidad 70% si ingresos > 50M
    
    return Persona(nombre, apellido, id, ciudad, fecha, ingresos, patrimonio, deudas, declarante);
}

/**
 * Implementación de generarColeccion.
 * 
 * POR QUÉ: Generar un conjunto de n personas.
 * CÓMO: Reservando espacio y agregando n personas generadas.
 * PARA QUÉ: Crear datasets para pruebas.
 */
std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    personas.reserve(n); // Reserva espacio para n personas (eficiencia)
    
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

/**
 * Implementación de buscarPorID.
 * 
 * POR QUÉ: Encontrar una persona por su ID en una colección.
 * CÓMO: Usando un algoritmo de búsqueda secuencial (lineal).
 * PARA QUÉ: Para operaciones de búsqueda en la aplicación.
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    // Usa find_if con una lambda para buscar por ID
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.getId() == id; });
    
    if (it != personas.end()) {
        return &(*it); // Devuelve puntero a la persona encontrada
    } else {
        return nullptr; // No encontrado
    }
}

/**
 * Implementación de generarGrupoDIAN.
 * 
 * POR QUÉ: Clasificar contribuyentes según el calendario tributario DIAN 2025.
 * CÓMO: Extrayendo los dos últimos dígitos del ID y aplicando las reglas.
 * PARA QUÉ: Determinar fechas de declaración de renta.
 */
char generarGrupoDIAN(const std::string& id) {
    // Extrae los dos últimos dígitos del ID
    if (id.length() < 2) {
        return 'A'; // Valor por defecto si el ID es muy corto
    }
    
    // Obtiene los dos últimos caracteres y los convierte a número
    std::string ultimosDosDigitos = id.substr(id.length() - 2);
    int digitos = std::stoi(ultimosDosDigitos);
    
    // Aplica las reglas del calendario tributario DIAN 2025
    if (digitos >= 0 && digitos <= 39) {
        return 'A'; // Grupo A: 00-39
    } else if (digitos >= 40 && digitos <= 79) {
        return 'B'; // Grupo B: 40-79
    } else { // digitos >= 80 && digitos <= 99
        return 'C'; // Grupo C: 80-99
    }
}

/**
 * Función auxiliar para calcular la edad aproximada de una persona.
 */
int calcularEdad(const std::string& fechaNacimiento) {
    // Extrae el año de la fecha (formato DD/MM/AAAA)
    size_t pos = fechaNacimiento.find_last_of('/');
    if (pos == std::string::npos) return 0;
    
    int anioNacimiento = std::stoi(fechaNacimiento.substr(pos + 1));
    int anioActual = 2025; // Año actual
    return anioActual - anioNacimiento;
}

const Persona* encontrarPersonaMasLongeva(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;
    
    const Persona* masLongeva = &personas[0];
    int mayorEdad = calcularEdad(masLongeva->getFechaNacimiento());
    
    for (const auto& persona : personas) {
        int edad = calcularEdad(persona.getFechaNacimiento());
        if (edad > mayorEdad) {
            mayorEdad = edad;
            masLongeva = &persona;
        }
    }
    
    return masLongeva;
}

void encontrarLongevasPorCiudad(const std::vector<Persona>& personas) {
    std::map<std::string, const Persona*> longevasPorCiudad;
    std::map<std::string, int> edadesPorCiudad;
    
    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        int edad = calcularEdad(persona.getFechaNacimiento());
        
        if (longevasPorCiudad.find(ciudad) == longevasPorCiudad.end() || 
            edad > edadesPorCiudad[ciudad]) {
            longevasPorCiudad[ciudad] = &persona;
            edadesPorCiudad[ciudad] = edad;
        }
    }
    
    std::cout << "\n=== PERSONAS MÁS LONGEVAS POR CIUDAD ===\n";
    for (const auto& par : longevasPorCiudad) {
        std::cout << "📍 " << par.first << ": " 
                  << par.second->getNombre() << " " << par.second->getApellido()
                  << " (ID: " << par.second->getId() << ") - " 
                  << edadesPorCiudad[par.first] << " años\n";
    }
}

const Persona* encontrarMayorPatrimonio(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;
    
    const Persona* mayorPatrimonio = &personas[0];
    
    for (const auto& persona : personas) {
        if (persona.getPatrimonio() > mayorPatrimonio->getPatrimonio()) {
            mayorPatrimonio = &persona;
        }
    }
    
    return mayorPatrimonio;
}

void encontrarMayorPatrimonioPorCiudad(const std::vector<Persona>& personas) {
    std::map<std::string, const Persona*> mayoresPorCiudad;
    
    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        
        if (mayoresPorCiudad.find(ciudad) == mayoresPorCiudad.end() || 
            persona.getPatrimonio() > mayoresPorCiudad[ciudad]->getPatrimonio()) {
            mayoresPorCiudad[ciudad] = &persona;
        }
    }
    
    std::cout << "\n=== MAYOR PATRIMONIO POR CIUDAD ===\n";
    for (const auto& par : mayoresPorCiudad) {
        std::cout << "📍 " << par.first << ": " 
                  << par.second->getNombre() << " " << par.second->getApellido()
                  << " (ID: " << par.second->getId() << ") - $" 
                  << std::fixed << std::setprecision(2) << par.second->getPatrimonio() << "\n";
    }
}

void encontrarMayorPatrimonioPorGrupoDIAN(const std::vector<Persona>& personas) {
    std::map<char, const Persona*> mayoresPorGrupo;
    
    for (const auto& persona : personas) {
        char grupo = generarGrupoDIAN(persona.getId());
        
        if (mayoresPorGrupo.find(grupo) == mayoresPorGrupo.end() || 
            persona.getPatrimonio() > mayoresPorGrupo[grupo]->getPatrimonio()) {
            mayoresPorGrupo[grupo] = &persona;
        }
    }
    
    std::cout << "\n=== MAYOR PATRIMONIO POR GRUPO DIAN ===\n";
    for (const auto& par : mayoresPorGrupo) {
        std::cout << " Grupo " << par.first << ": " 
                  << par.second->getNombre() << " " << par.second->getApellido()
                  << " (ID: " << par.second->getId() << ") - $" 
                  << std::fixed << std::setprecision(2) << par.second->getPatrimonio() << "\n";
    }
}

void listarDeclarantesPorGrupo(std::vector<Persona> personas) {
    std::map<char, std::vector<const Persona*>> declarantesPorGrupo;
    std::map<char, int> contadorPorGrupo = {{'A', 0}, {'B', 0}, {'C', 0}};
    
    // ANÁLISIS RENDIMIENTO: Vector copiado por valor + método calls (.getId(), .getDeclaranteRenta())
    for (const auto& persona : personas) {
        char grupo = generarGrupoDIAN(persona.getId());
        contadorPorGrupo[grupo]++;
        
        if (persona.getDeclaranteRenta()) {
            declarantesPorGrupo[grupo].push_back(&persona);
        }
    }
    
    std::cout << "\n=== 📅 DECLARANTES DE RENTA POR CALENDARIO TRIBUTARIO ===\n";
    
    for (char grupo : {'A', 'B', 'C'}) {
        std::cout << "\n GRUPO " << grupo << " (Terminación ";
        if (grupo == 'A') std::cout << "00-39";
        else if (grupo == 'B') std::cout << "40-79";
        else std::cout << "80-99";
        std::cout << "):\n";
        
        std::cout << "   Total personas en grupo: " << contadorPorGrupo[grupo] << "\n";
        std::cout << "   Declarantes de renta: " << declarantesPorGrupo[grupo].size() << "\n";
        
        if (!declarantesPorGrupo[grupo].empty()) {
            std::cout << "   Lista de declarantes:\n";
            for (const auto* persona : declarantesPorGrupo[grupo]) {
                std::cout << "   • " << persona->getNombre() << " " << persona->getApellido()
                          << " (ID: " << persona->getId() << ") - $" 
                          << std::fixed << std::setprecision(2) << persona->getIngresosAnuales() << "\n";
            }
        }
    }
}

/**
 * Implementación de analizarCiudadesPorPatrimonioPromedio.
 * 
 * POR QUÉ: Identificar las ciudades con mayor concentración de riqueza.
 * CÓMO: Calculando patrimonio promedio por ciudad y ordenando descendentemente.
 * PARA QUÉ: Análisis económico territorial y toma de decisiones.
 */
void analizarCiudadesPorPatrimonioPromedio(std::vector<Persona> personas) {
    std::map<std::string, double> sumaPatrimonioPorCiudad;
    std::map<std::string, int> contadorPorCiudad;
    
    // ANÁLISIS RENDIMIENTO: Vector copiado por valor + método calls (.getCiudadNacimiento(), .getPatrimonio())
    // Acumular patrimonio por ciudad
    for (const auto& persona : personas) {
        const std::string& ciudad = persona.getCiudadNacimiento();
        sumaPatrimonioPorCiudad[ciudad] += persona.getPatrimonio();
        contadorPorCiudad[ciudad]++;
    }
    
    // Calcular promedios y almacenar en vector para ordenar
    std::vector<std::pair<std::string, double>> ciudadesPromedio;
    for (const auto& par : sumaPatrimonioPorCiudad) {
        const std::string& ciudad = par.first;
        double promedio = par.second / contadorPorCiudad[ciudad];
        ciudadesPromedio.push_back({ciudad, promedio});
    }
    
    // Ordenar por patrimonio promedio descendente
    std::sort(ciudadesPromedio.begin(), ciudadesPromedio.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::cout << "\n=== CIUDADES POR PATRIMONIO PROMEDIO (MAYOR A MENOR) ===\n";
    std::cout << std::fixed << std::setprecision(2);
    
    for (size_t i = 0; i < ciudadesPromedio.size(); ++i) {
        const auto& ciudad = ciudadesPromedio[i];
        std::cout << (i + 1) << ". " << ciudad.first 
                  << ": $" << ciudad.second 
                  << " (Población: " << contadorPorCiudad[ciudad.first] << " personas)\n";
    }
}

/**
 * Implementación de analizarPorcentajeMayores60PorCalendario.
 * 
 * POR QUÉ: Análisis demográfico de población de tercera edad por grupo tributario.
 * CÓMO: Contando personas >60 años en cada grupo DIAN y calculando porcentajes.
 * PARA QUÉ: Estudios de envejecimiento poblacional y políticas públicas.
 */
void analizarPorcentajeMayores60PorCalendario(std::vector<Persona> personas) {
    std::map<char, int> totalPorGrupo = {{'A', 0}, {'B', 0}, {'C', 0}};
    std::map<char, int> mayores60PorGrupo = {{'A', 0}, {'B', 0}, {'C', 0}};
    
    // ANÁLISIS RENDIMIENTO: Vector copiado por valor + método calls (.getId(), .getFechaNacimiento())
    for (const auto& persona : personas) {
        char grupo = generarGrupoDIAN(persona.getId());
        totalPorGrupo[grupo]++;
        
        int edad = calcularEdad(persona.getFechaNacimiento());
        if (edad > 60) {
            mayores60PorGrupo[grupo]++;
        }
    }
    
    std::cout << "\n=== ANÁLISIS DEMOGRÁFICO: PERSONAS > 60 AÑOS POR CALENDARIO DIAN ===\n";
    std::cout << std::fixed << std::setprecision(2);
    
    int totalMayores60 = 0;
    int totalPersonas = 0;
    
    for (char grupo : {'A', 'B', 'C'}) {
        std::cout << "\n📅 GRUPO " << grupo << " (Terminación ";
        if (grupo == 'A') std::cout << "00-39";
        else if (grupo == 'B') std::cout << "40-79";
        else std::cout << "80-99";
        std::cout << "):\n";
        
        int total = totalPorGrupo[grupo];
        int mayores = mayores60PorGrupo[grupo];
        double porcentaje = (total > 0) ? (static_cast<double>(mayores) / total) * 100.0 : 0.0;
        
        std::cout << "   Total personas: " << total << "\n";
        std::cout << "   Mayores de 60 años: " << mayores << "\n";
        std::cout << "   Porcentaje: " << porcentaje << "%\n";
        
        totalMayores60 += mayores;
        totalPersonas += total;
    }
    
    // Estadística general
    double porcentajeGeneral = (totalPersonas > 0) ? 
        (static_cast<double>(totalMayores60) / totalPersonas) * 100.0 : 0.0;
    
    std::cout << "\n📊 RESUMEN GENERAL:\n";
    std::cout << "   Total nacional: " << totalPersonas << " personas\n";
    std::cout << "   Mayores de 60 años: " << totalMayores60 << " personas\n";
    std::cout << "   Porcentaje nacional: " << porcentajeGeneral << "%\n";
}