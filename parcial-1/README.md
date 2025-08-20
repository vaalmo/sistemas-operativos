## Manejo de Grandes Volúmenes de Datos en Linux con C/C++

elaborado por _Valentina Morales Villada_

## Preguntas adicionales planteadas

1. Ciudades con patrimonio promedio más alto
2. Porcentaje de personas > 60 años por calendario

### Estadísticas de comparación de rendimiento (2 millones de registros)

#### Versión con Clases + Referencias:

```
= RESUMEN DE ESTADÍSTICAS =
Crear datos: 3095.61 ms, 395568 KB
Mostrar resumen: 36141.27 ms, 0 KB
Análisis longevidad: 1094.33 ms, 0 KB
Análisis longevidad: 973.63 ms, 0 KB
Análisis patrimonio: 742.37 ms, 0 KB
Análisis patrimonio: 884.56 ms, 0 KB
Análisis patrimonio: 904.51 ms, 0 KB
Análisis declarantes: 21187.14 ms, 2360 KB
Análisis ciudades patrimonio: 135.72 ms, 0 KB
Análisis mayores 60 años: 158.53 ms, 0 KB
Total tiempo: 65317.68 ms
Memoria máxima: 395568 KB
```

#### Versión con Estructuras + Referencias:

```
= RESUMEN DE ESTADÍSTICAS =
Crear datos: 5139.00 ms, 0 KB
Mostrar resumen: 32835.00 ms, 0 KB
Análisis longevidad: 2271.00 ms, 0 KB
Análisis longevidad: 1477.00 ms, 0 KB
Análisis patrimonio: 919.00 ms, 0 KB
Análisis patrimonio: 1230.00 ms, 0 KB
Análisis patrimonio: 1452.00 ms, 0 KB
Análisis declarantes: 19343.00 ms, 0 KB
Análisis ciudades patrimonio: 632.00 ms, 0 KB
Análisis mayores 60 años: 378.00 ms, 0 KB
Total tiempo: 65676.00 ms
Memoria máxima: 0 KB
```

#### Versión con Clases + Valores:

```
= RESUMEN DE ESTADÍSTICAS =
Crear datos: 4269.97 ms, 395712 KB
Mostrar resumen: 37225.72 ms, 0 KB
Análisis longevidad: 1044.24 ms, 0 KB
Análisis longevidad: 1062.54 ms, 0 KB
Análisis patrimonio: 762.63 ms, 0 KB
Análisis patrimonio: 901.72 ms, 0 KB
Análisis patrimonio: 1203.17 ms, 0 KB
Análisis declarantes: 25365.17 ms, 19560 KB
Análisis ciudades patrimonio: 596.49 ms, 0 KB
Análisis mayores 60 años: 649.53 ms, 0 KB
Total tiempo: 73081.19 ms
Memoria máxima: 395712 KB
```

#### Versión con Estructuras + Valores:

```
= RESUMEN DE ESTADÍSTICAS =
Crear datos: 5040.00 ms, 0 KB
Mostrar resumen: 37055.00 ms, 0 KB
Análisis longevidad: 1334.00 ms, 0 KB
Análisis longevidad: 2151.00 ms, 0 KB
Análisis patrimonio: 765.00 ms, 0 KB
Análisis patrimonio: 2075.00 ms, 0 KB
Análisis patrimonio: 1483.00 ms, 0 KB
Análisis declarantes: 24442.00 ms, 0 KB
Análisis ciudades patrimonio: 1809.00 ms, 0 KB
Análisis mayores 60 años: 1132.00 ms, 0 KB
Total tiempo: 77286.00 ms
Memoria máxima: 0 KB
```



## Tabla de comparación de rendimiento

### Complete Performance Analysis (2 Million Objects)

| Función/Operación                            | Clases + Referencias | Structs + Referencias | Clases + Valores | Structs + Valores | Costo por Copia   |
| -------------------------------------------- | -------------------- | --------------------- | ---------------- | ----------------- | ----------------- |
| Crear datos                                  | 3,096 ms             | 5,139 ms              | 4,270 ms         | 5,040 ms          | +38% / -2%        |
| Mostrar resumen                              | 36,141 ms            | 32,835 ms             | 37,226 ms        | 37,055 ms         | +3% / +13%        |
| Análisis longevidad en el país               | 1,094 ms             | 2,271 ms              | 1,044 ms         | 1,334 ms          | -5% / -41%        |
| Análisis longevidad por ciudad               | 974 ms               | 1,477 ms              | 1,063 ms         | 2,151 ms          | +9% / +46%        |
| Análisis patrimonio en el país               | 742 ms               | 919 ms                | 763 ms           | 765 ms            | +3% / -17%        |
| Análisis patrimonio por ciuad                | 885 ms               | 1,230 ms              | 902 ms           | 2,075 ms          | +2% / +69%        |
| Análisis patrimonio por grupo de declaración | 905 ms               | 1,452 ms              | 1,203 ms         | 1,483 ms          | +33% / +2%        |
| **Análisis declarantes**                     | **21,187 ms**        | **19,343 ms**         | **25,365 ms**    | **24,442 ms**     | **+20% / +26%**   |
| **Análisis ciudades**                        | **136 ms**           | **632 ms**            | **596 ms**       | **1,809 ms**      | **+339% / +186%** |
| **Análisis mayores 60**                      | **159 ms**           | **378 ms**            | **650 ms**       | **1,132 ms**      | **+309% / +199%** |
| **Tiempo total**                             | **65,318 ms**        | **65,676 ms**         | **73,081 ms**    | **77,286 ms**     | **+12% / +18%**   |
| **Memoria máxima**                           | **395,568 KB**       | **0 KB***             | **395,712 KB**   | **0 KB***         |                   |

### Análisis del costo por copia en las funciones objetivo

| Función Modificada           | Costo por copia clases | Costo por copia structs | Impacto en Memoria  |
| ---------------------------- | ---------------------- | ----------------------- | ------------------- |
| Análisis declarantes         | +4,178 ms (+20%)       | +5,099 ms (+26%)        | +19,560 KB (clases) |
| Análisis ciudades patrimonio | +460 ms (+339%)        | +1,177 ms (+186%)       | Sin cambio          |
| Análisis mayores 60 años     | +491 ms (+309%)        | +754 ms (+199%)         | Sin cambio          |

## Resumen de resultados

### Ranking de rendimiento (tiempo total):

1. **Classes + Referencias**: 65.3 segundos (más rápido)
2. **Structs + Referencias**: 65.7 segundos (+0.5%)
3. **Classes + Valores**: 73.1 segundos (+12%)
4. **Structs + Valores**: 77.3 segundos (+18% más lento)

### Impacto del paso por valor:

- **Costo por copia promedio**: 12-18% más lento en tiempo total
- **Funciones objetivo**: 20-339% más lentas (como esperado)
- **Funciones no modificadas**: Impacto mínimo o nulo
- **Uso de memoria**: +19.5 MB adicionales en operación crítica de copia

### Clases vs Structs:

- **Con referencias**: Rendimiento similar (65.3s vs 65.7s)
- **Con valores**: Clases mantienen ventaja (73.1s vs 77.3s)
- **Acceso a campos**: Métodos (.getPatrimonio()) vs acceso directo (.patrimonio) muestra diferencias menores que el costo por copia

### Métricas adicionales RAM

#### Versión clases referencias

listado personas

<img width="692" height="515" alt="image" src="https://github.com/user-attachments/assets/67f3164e-63cf-4b6a-ae44-d054cd7d4a5f" />

listado declarantes

<img width="690" height="534" alt="image" src="https://github.com/user-attachments/assets/9d8ac665-e51a-4c2d-add1-ee60f56a7272" />

#### Versión estructuras referencias

<img width="694" height="518" alt="image" src="https://github.com/user-attachments/assets/6d432bec-82da-430b-ad4a-4d64a44dacd6" />

listado personas

<img width="693" height="522" alt="image" src="https://github.com/user-attachments/assets/3b9cd2b4-dfe4-4aa0-8790-1d2d9152f5da" />

listado declarantes


#### Versión clases valores

<img width="692" height="520" alt="image" src="https://github.com/user-attachments/assets/bb6323a2-af17-4f55-a9e3-3d4ca98630fa" />

listado personas

<img width="691" height="523" alt="image" src="https://github.com/user-attachments/assets/e1fb8d9f-9951-4e44-898d-7b8ed2a5010e" />

listado declarantes


#### Versión estructuras valores

<img width="689" height="517" alt="image" src="https://github.com/user-attachments/assets/45882272-2356-443e-aa28-e511b2f101d7" />

listado personas 

<img width="693" height="524" alt="image" src="https://github.com/user-attachments/assets/d60899f8-c36f-450a-b905-2ada1ed13b20" />

listado declarantes

## Tabla de comparación final

| Versión               | Opción 6 (Declarantes) | Opción 7 (Ciudades) | Opción 8 (Mayores 60) | Tiempo total | Pico de uso de memoria |
| --------------------- | ---------------------- | ------------------- | --------------------- | ------------ | ---------------------- |
| **Clases + Refs**     | 21,187ms               | 136ms               | 159ms                 | 65.3s        | 395MB                  |
| **Structs + Refs**    | 19,343ms               | 632ms               | 378ms                 | 65.7s        |                        |
| **Clases + Valores**  | 25,365ms (+20%)        | 596ms (+339%)       | 650ms (+309%)         | 73.1s        | 395MB (+19.5MB pico)   |
| **Structs + Valores** | 24,442ms (+26%)        | 1,809ms (+186%)     | 1,132ms (+199%)       | 77.3s        |                        |

Podemos evidenciar que el uso de **clases + acceso por referencia** fue la versión con tiempo de ejecución más reducido y uso de memoria controlado. Lo cual la convierte en una excelente opción para acceso a datos de esta complejidad y estructura, y además para métodos que involuccran estructuras de datos más complejas.

Las opciones del menú que se ven en la tabla fueron las opciones comparadas e implementadas en ambos referencias y valores, y clases y estructuras para comparar su rendimiento.

### Análisis de comportamiento

Solo se midió el impacto en las 3 funciones que se cambiaron de paso por referencia a paso por valor, el resto se mantuvieron iguales para aislar correctamente el costo por copia.

1. **Costo por copia evidente**: Las funciones modificadas para usar paso por valor mostraron ser un **20-339%** más lentas
2. **Impacto aislado**: Solo las 3 funciones objetivo que se modificaron muestran costo de copia significativo
3. **Clases superan structs**: Las clases mantienen mejor rendimiento, aunque por poca diferencia
4. **Memoria**: El paso port valor genera costo por copia de memoria (+19.5 MB)
5. **Validación del enfoque**: La comparación logra aislar exitosamente el impacto del paso por valor vs el paso por referencia

### Preguntas críticas

#### 1. ¿Por qué referencias reducen significativamente el tiempo de ejecución?

Según los resultados obtenidos, las referencias eliminan el costo por copia completa de vectores en cada llamado a la función . Las funciones objetivo que se modificaron muestran ser **20-339%** más lentas cuando se pasa por valor, confirmando que evitar copias masivas de datos es un punto crítico que afecta el rendimiento.

#### 2. ¿Cómo optimizar búsquedas por grupo calendario?

Cálculo directo basado en últimos 2 dígitos del documento:

```cpp
char calcularGrupo(const string& documento) {
    int ultimos = stoi(documento.substr(documento.length()-2));
    return (ultimos <= 39) ? 'A' : (ultimos <= 79) ? 'B' : 'C';
}
```

#### 3. ¿Array de structs vs vector de clases?

En este caso es mejor el **vector de clases + referencias**, ya que logra el mejor tiempo total (65.3s vs 65.7s vs 73.1s vs 77.3s). También se evidencia que la diferencia de rendimiento entre classes y structs es menor que el impacto del paso por valor vs el paso por referencia.

#### 4. Escalabilidad con memoria virtual:

Con 2 millones de registros ya se observa impacto en rendimiento. Para mayor cantidad de datos como 10 millones, usaría procesamiento por lotes o memory mapping. El mmap es transparente y el sistema operativo se encarga de manejar todo, mientras que en el procesamiento por lotes se tiene control total y puede ser más predecible.

-----

Link del video

