#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // Generadores aleatorios modernos
#include <vector>
#include <algorithm> // Para find_if, sort
#include <map>       // Para agrupaciones
#include <iomanip>   // Para formateo
#include <iostream>  // Para cout

// --- Bases de datos para generación realista ---

// Nombres femeninos comunes en Colombia
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

// Nombres masculinos comunes en Colombia
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Sofia","Camila","Juana","Ana","Laura","Karla","Andrea","Daniela","Alejandra","Martina",
    "Nelly","María","Nestor","Trinidad","Fernanda", "Carolina", "Lina", "Gertridis"
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

// Implementación de funciones generadoras

std::string generarFechaNacimiento() {
    // Genera día aleatorio (1-28 para simplificar)
    int dia = 1 + rand() % 28;
    // Mes aleatorio (1-12)
    int mes = 1 + rand() % 12;
    // Año entre 1960-2010
    int anio = 1960 + rand() % 50;
    
    // Convierte a string en formato DD/MM/AAAA
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

std::string generarID() {
    static long contador = 1000000000; // ID inicial
    return std::to_string(contador++); // Incrementa después de usar
}

double randomDouble(double min, double max) {
    // Generador moderno Mersenne Twister
    static std::mt19937 generator(time(nullptr));
    // Distribución uniforme en rango [min, max]
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

Persona generarPersona() {
    Persona p; // Crea una instancia de la estructura Persona
    
    // Decide aleatoriamente si es hombre o mujer
    bool esHombre = rand() % 2;
    
    // Selecciona nombre según género
    p.nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    // Combina dos apellidos aleatorios
    p.apellido = apellidos[rand() % apellidos.size()] + " " + 
                 apellidos[rand() % apellidos.size()];
    
    // Genera identificadores únicos
    p.id = generarID();
    // Ciudad aleatoria de Colombia
    p.ciudadNacimiento = ciudadesColombia[rand() % ciudadesColombia.size()];
    // Fecha aleatoria
    p.fechaNacimiento = generarFechaNacimiento();
    
    // --- Generación de datos económicos realistas ---
    // Ingresos entre 10 millones y 500 millones COP
    p.ingresosAnuales = randomDouble(10000000, 500000000);
    // Patrimonio entre 0 y 2 mil millones COP
    p.patrimonio = randomDouble(0, 2000000000);
    // Deudas hasta el 70% del patrimonio
    p.deudas = randomDouble(0, p.patrimonio * 0.7);
    // 70% probabilidad de ser declarante si gana > 50 millones
    p.declaranteRenta = (p.ingresosAnuales > 50000000) && (rand() % 100 > 30);
    
    return p; // Retorna la estructura completa
}

std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    // Reserva espacio para n personas (optimización)
    personas.reserve(n);
    
    // Genera n personas y las añade al vector
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    // Búsqueda lineal por ID (solución simple para colecciones medianas)
    for (const auto& persona : personas) {
        if (persona.id == id) { // Acceso directo al campo id
            return &persona; // Retorna dirección si encuentra coincidencia
        }
    }
    return nullptr; // Retorna nulo si no encuentra
}

char generarGrupoDIAN(const std::string& id) {
    if (id.length() < 2) {
        return 'A'; // Valor por defecto si el id es muy corto
    }
    
    // Obtiene los dos últimos caracteres y los convierte a número
    std::string ultimosDosDigitos = id.substr(id.length() - 2);
    int digitos = std::stoi(ultimosDosDigitos);
    
    // Aplica las reglas del calendario tributario de la DIAN del 2025
    if (digitos >= 0 && digitos <= 39) {
        return 'A'; // Grupo A: 00-39
    } else if (digitos >= 40 && digitos <= 79) {
        return 'B'; // Grupo B: 40-79
    } else { // digitos >= 80 && digitos <= 99
        return 'C'; // Grupo C: 80-99
    }
}

// Función auxiliar para calcular la edad aproximada
int calcularEdad(const std::string& fechaNacimiento) {
    size_t pos = fechaNacimiento.find_last_of('/');
    if (pos == std::string::npos) return 0;
    
    int anioNacimiento = std::stoi(fechaNacimiento.substr(pos + 1));
    int anioActual = 2025;
    return anioActual - anioNacimiento;
}

const Persona* encontrarPersonaMasLongeva(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;
    
    const Persona* masLongeva = &personas[0];
    int mayorEdad = calcularEdad(masLongeva->fechaNacimiento);
    
    for (const auto& persona : personas) {
        int edad = calcularEdad(persona.fechaNacimiento);
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
        std::string ciudad = persona.ciudadNacimiento;
        int edad = calcularEdad(persona.fechaNacimiento);
        
        if (longevasPorCiudad.find(ciudad) == longevasPorCiudad.end() || 
            edad > edadesPorCiudad[ciudad]) {
            longevasPorCiudad[ciudad] = &persona;
            edadesPorCiudad[ciudad] = edad;
        }
    }
    
    std::cout << "\n=== PERSONAS MÁS LONGEVAS POR CIUDAD ===\n";
    for (const auto& par : longevasPorCiudad) {
        std::cout << "📍 " << par.first << ": " 
                  << par.second->nombre << " " << par.second->apellido
                  << " (ID: " << par.second->id << ") - " 
                  << edadesPorCiudad[par.first] << " años\n";
    }
}

const Persona* encontrarMayorPatrimonio(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;
    
    const Persona* mayorPatrimonio = &personas[0];
    
    for (const auto& persona : personas) {
        if (persona.patrimonio > mayorPatrimonio->patrimonio) {
            mayorPatrimonio = &persona;
        }
    }
    
    return mayorPatrimonio;
}

void encontrarMayorPatrimonioPorCiudad(const std::vector<Persona>& personas) {
    std::map<std::string, const Persona*> mayoresPorCiudad;
    
    for (const auto& persona : personas) {
        std::string ciudad = persona.ciudadNacimiento;
        
        if (mayoresPorCiudad.find(ciudad) == mayoresPorCiudad.end() || 
            persona.patrimonio > mayoresPorCiudad[ciudad]->patrimonio) {
            mayoresPorCiudad[ciudad] = &persona;
        }
    }
    
    std::cout << "\n=== MAYOR PATRIMONIO POR CIUDAD ===\n";
    for (const auto& par : mayoresPorCiudad) {
        std::cout << "📍 " << par.first << ": " 
                  << par.second->nombre << " " << par.second->apellido
                  << " (ID: " << par.second->id << ") - $" 
                  << std::fixed << std::setprecision(2) << par.second->patrimonio << "\n";
    }
}

void encontrarMayorPatrimonioPorGrupoDIAN(const std::vector<Persona>& personas) {
    std::map<char, const Persona*> mayoresPorGrupo;
    
    for (const auto& persona : personas) {
        char grupo = generarGrupoDIAN(persona.id);
        
        if (mayoresPorGrupo.find(grupo) == mayoresPorGrupo.end() || 
            persona.patrimonio > mayoresPorGrupo[grupo]->patrimonio) {
            mayoresPorGrupo[grupo] = &persona;
        }
    }
    
    std::cout << "\n=== MAYOR PATRIMONIO POR GRUPO DIAN ===\n";
    for (const auto& par : mayoresPorGrupo) {
        std::cout << " Grupo " << par.first << ": " 
                  << par.second->nombre << " " << par.second->apellido
                  << " (ID: " << par.second->id << ") - $" 
                  << std::fixed << std::setprecision(2) << par.second->patrimonio << "\n";
    }
}

void listarDeclarantesPorGrupo(std::vector<Persona> personas) {
    std::map<char, std::vector<const Persona*>> declarantesPorGrupo;
    std::map<char, int> contadorPorGrupo = {{'A', 0}, {'B', 0}, {'C', 0}};
    
    // ANÁLISIS RENDIMIENTO: Vector copiado por valor + acceso directo (.id, .declaranteRenta)
    for (const auto& persona : personas) {
        char grupo = generarGrupoDIAN(persona.id);
        contadorPorGrupo[grupo]++;
        
        if (persona.declaranteRenta) {
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
                std::cout << "   • " << persona->nombre << " " << persona->apellido
                          << " (ID: " << persona->id << ") - $" 
                          << std::fixed << std::setprecision(2) << persona->ingresosAnuales << "\n";
            }
        }
    }
}

// Implementación de analizarCiudadesPorPatrimonioPromedio
void analizarCiudadesPorPatrimonioPromedio(std::vector<Persona> personas) {
    std::map<std::string, double> sumaPatrimonioPorCiudad;
    std::map<std::string, int> contadorPorCiudad;
    
    // ANÁLISIS RENDIMIENTO: Vector copiado por valor + acceso directo (.ciudadNacimiento, .patrimonio)
    // Acumular patrimonio por ciudad
    for (const auto& persona : personas) {
        const std::string& ciudad = persona.ciudadNacimiento;
        sumaPatrimonioPorCiudad[ciudad] += persona.patrimonio;
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

// Implementación de analizarPorcentajeMayores60PorCalendario
void analizarPorcentajeMayores60PorCalendario(std::vector<Persona> personas) {
    std::map<char, int> totalPorGrupo = {{'A', 0}, {'B', 0}, {'C', 0}};
    std::map<char, int> mayores60PorGrupo = {{'A', 0}, {'B', 0}, {'C', 0}};
    
    // ANÁLISIS RENDIMIENTO: Vector copiado por valor + acceso directo (.id, .fechaNacimiento)
    for (const auto& persona : personas) {
        char grupo = generarGrupoDIAN(persona.id);
        totalPorGrupo[grupo]++;
        
        int edad = calcularEdad(persona.fechaNacimiento);
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