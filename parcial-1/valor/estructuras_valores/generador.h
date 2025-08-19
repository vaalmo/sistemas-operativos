#ifndef GENERADOR_H
#define GENERADOR_H

#include "persona.h"
#include <vector>

// --- Funciones para generación de datos aleatorios ---

// Genera fecha aleatoria entre 1960-2010
std::string generarFechaNacimiento();

// Genera ID único secuencial
std::string generarID();

// Genera número decimal en rango [min, max]
double randomDouble(double min, double max);

// Crea una persona con datos aleatorios
Persona generarPersona();

// Genera colección de n personas
std::vector<Persona> generarColeccion(int n);

// Busca persona por ID en un vector
// Retorna puntero a persona si la encuentra, nullptr si no
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id);

// Determina el grupo DIAN (A/B/C) según los últimos dos dígitos del documento
// Calendario tributario DIAN 2025: A(00-39), B(40-79), C(80-99)
char generarGrupoDIAN(const std::string& id);

// Encuentra la persona más longeva en todo el país
const Persona* encontrarPersonaMasLongeva(const std::vector<Persona>& personas);

// Encuentra las personas más longevas por cada ciudad
void encontrarLongevasPorCiudad(const std::vector<Persona>& personas);

// Encuentra la persona con mayor patrimonio en todo el país
const Persona* encontrarMayorPatrimonio(const std::vector<Persona>& personas);

// Encuentra las personas con mayor patrimonio por ciudad
void encontrarMayorPatrimonioPorCiudad(const std::vector<Persona>& personas);

// Encuentra las personas con mayor patrimonio por grupo DIAN
void encontrarMayorPatrimonioPorGrupoDIAN(const std::vector<Persona>& personas);

// Lista y cuenta declarantes de renta por calendario tributario
// ANÁLISIS RENDIMIENTO: Pass-by-value para medir overhead de copia + direct field access
void listarDeclarantesPorGrupo(std::vector<Persona> personas);

// Analiza ciudades ordenadas por patrimonio promedio más alto
// ANÁLISIS RENDIMIENTO: Pass-by-value para medir overhead de copia + direct field access
void analizarCiudadesPorPatrimonioPromedio(std::vector<Persona> personas);

/**
 * Calcula el porcentaje de personas mayores de 60 años por calendario DIAN.
 * 
 * POR QUÉ: Análisis demográfico de población de tercera edad por grupo tributario.
 * CÓMO: Contando personas >60 años en cada grupo DIAN y calculando porcentajes.
 * PARA QUÉ: Estudios demográficos y políticas para adultos mayores.
 * 
 * ANÁLISIS RENDIMIENTO: Pass-by-value para medir overhead de copia + direct field access
 */
void analizarPorcentajeMayores60PorCalendario(std::vector<Persona> personas);

#endif // GENERADOR_H