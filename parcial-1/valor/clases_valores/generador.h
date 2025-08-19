#ifndef GENERADOR_H
#define GENERADOR_H

#include "persona.h"
#include <vector>

// Funciones para generación de datos aleatorios

/**
 * Genera una fecha de nacimiento aleatoria entre 1960 y 2010.
 * 
 * POR QUÉ: Simular fechas realistas para personas.
 * CÓMO: Combinando números aleatorios para día, mes y año.
 * PARA QUÉ: Inicializar el atributo fechaNacimiento de Persona.
 */
std::string generarFechaNacimiento();

/**
 * Genera un ID único secuencial.
 * 
 * POR QUÉ: Necesidad de identificadores únicos para cada persona.
 * CÓMO: Usando un contador estático que incrementa en cada llamada.
 * PARA QUÉ: Garantizar unicidad en los IDs.
 */
std::string generarID();

/**
 * Genera un número decimal aleatorio en un rango [min, max].
 * 
 * POR QUÉ: Necesidad de valores realistas para ingresos, patrimonio, etc.
 * CÓMO: Usando un generador Mersenne Twister y una distribución uniforme.
 * PARA QUÉ: Producir valores financieros aleatorios pero dentro de rangos lógicos.
 */
double randomDouble(double min, double max);

/**
 * Crea una persona con datos aleatorios.
 * 
 * POR QUÉ: Automatizar la creación de registros de personas.
 * CÓMO: Combinando las funciones generadoras y bases de datos de nombres, apellidos, etc.
 * PARA QUÉ: Poblar el sistema con datos de prueba.
 */
Persona generarPersona();

/**
 * Genera una colección (vector) de n personas.
 * 
 * POR QUÉ: Crear conjuntos de datos de diferentes tamaños.
 * CÓMO: Llamando a generarPersona() n veces.
 * PARA QUÉ: Pruebas de rendimiento y funcionalidad con volúmenes variables.
 */
std::vector<Persona> generarColeccion(int n);

/**
 * Busca una persona por ID en un vector de personas.
 * 
 * POR QUÉ: Recuperar una persona específica de una colección.
 * CÓMO: Usando un algoritmo de búsqueda lineal (o binaria si se ordena).
 * PARA QUÉ: Implementar funcionalidad de búsqueda en la aplicación.
 * 
 * @param personas Vector de personas donde buscar.
 * @param id ID a buscar.
 * @return Puntero a la persona encontrada o nullptr si no se encuentra.
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id);

/**
 * Determina el grupo DIAN (A/B/C) según los últimos dos dígitos del documento.
 * 
 * POR QUÉ: Clasificar contribuyentes según el calendario tributario DIAN 2025.
 * CÓMO: Evaluando los dos últimos dígitos del ID según rangos establecidos.
 * PARA QUÉ: Asignar fechas de declaración de renta correspondientes.
 * 
 * @param id Número de documento (cédula).
 * @return Carácter 'A', 'B' o 'C' según la clasificación DIAN.
 */
char generarGrupoDIAN(const std::string& id);

/**
 * Encuentra la persona más longeva (mayor edad) en todo el país.
 * 
 * POR QUÉ: Análisis demográfico para identificar la persona de mayor edad.
 * CÓMO: Comparando fechas de nacimiento para encontrar la más antigua.
 * PARA QUÉ: Reportes estadísticos y análisis poblacional.
 */
const Persona* encontrarPersonaMasLongeva(const std::vector<Persona>& personas);

/**
 * Encuentra las personas más longevas por cada ciudad.
 * 
 * POR QUÉ: Análisis demográfico regionalizado.
 * CÓMO: Agrupando por ciudad y encontrando la mayor edad en cada grupo.
 * PARA QUÉ: Reportes demográficos por región.
 */
void encontrarLongevasPorCiudad(const std::vector<Persona>& personas);

/**
 * Encuentra la persona con mayor patrimonio en todo el país.
 * 
 * POR QUÉ: Análisis económico para identificar la mayor fortuna.
 * CÓMO: Comparando valores de patrimonio entre todas las personas.
 * PARA QUÉ: Estudios de distribución de riqueza.
 */
const Persona* encontrarMayorPatrimonio(const std::vector<Persona>& personas);

/**
 * Encuentra las personas con mayor patrimonio por ciudad.
 * 
 * POR QUÉ: Análisis económico regionalizado.
 * CÓMO: Agrupando por ciudad y encontrando el mayor patrimonio en cada grupo.
 * PARA QUÉ: Estudios de distribución de riqueza por región.
 */
void encontrarMayorPatrimonioPorCiudad(const std::vector<Persona>& personas);

/**
 * Encuentra las personas con mayor patrimonio por grupo DIAN.
 * 
 * POR QUÉ: Análisis fiscal por categoría tributaria.
 * CÓMO: Agrupando por grupo DIAN (A/B/C) y encontrando el mayor patrimonio.
 * PARA QUÉ: Análisis tributario y distribución de contribuyentes.
 */
void encontrarMayorPatrimonioPorGrupoDIAN(const std::vector<Persona>& personas);

/**
 * Lista y cuenta declarantes de renta por calendario tributario.
 * 
 * POR QUÉ: Organización y conteo de obligaciones tributarias.
 * CÓMO: Clasificando por grupo DIAN y filtrando declarantes.
 * PARA QUÉ: Administración tributaria y reportes fiscales.
 */
// Lista y cuenta declarantes de renta por calendario tributario
// ANÁLISIS RENDIMIENTO: Pass-by-value para medir overhead de copia + method calls
void listarDeclarantesPorGrupo(std::vector<Persona> personas);

/**
 * Analiza ciudades ordenadas por patrimonio promedio más alto.
 * 
 * POR QUÉ: Identificar las ciudades con mayor concentración de riqueza.
 * CÓMO: Calculando el promedio de patrimonio por ciudad y ordenando.
 * PARA QUÉ: Análisis económico territorial y planificación urbana.
 * 
 * ANÁLISIS RENDIMIENTO: Pass-by-value para medir overhead de copia + method calls
 */
void analizarCiudadesPorPatrimonioPromedio(std::vector<Persona> personas);

/**
 * Calcula el porcentaje de personas mayores de 60 años por calendario DIAN.
 * 
 * POR QUÉ: Análisis demográfico de población de tercera edad por grupo tributario.
 * CÓMO: Contando personas >60 años en cada grupo DIAN y calculando porcentajes.
 * PARA QUÉ: Estudios demográficos y políticas para adultos mayores.
 * 
 * ANÁLISIS RENDIMIENTO: Pass-by-value para medir overhead de copia + method calls
 */
void analizarPorcentajeMayores60PorCalendario(std::vector<Persona> personas);

#endif // GENERADOR_H