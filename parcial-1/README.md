# Manejo de Grandes Volúmenes de Datos en Linux con C/C++

elaborado por _Valentina Morales Villada_
### Preguntas adicionales planteadas

1. Ciudades con patrimonio promedio más alto
2. Porcentaje de personas > 60 años por calendario

## Resumen de Comparación de Rendimiento (2 millones de registros)

**Versión con Clases + Referencias:**

=== RESUMEN DE ESTADÍSTICAS ===
- Crear datos: 4062.49 ms, 395708 KB
- Mostrar resumen: 40603.27 ms, 0 KB
- Análisis longevidad en el país: 1386.81 ms, 0 KB
- Análisis longevidad por cada ciudad: 1270.69 ms, 0 KB
- Análisis patrimonio en el país: 768.22 ms, 0 KB
- Análisis patrimonio por cada ciudad: 1117.32 ms, 0 KB
- Análisis patrimonio por grupo de declaración: 2313.19 ms, 0 KB
- Análisis declarantes: 23569.02 ms, 452 KB
- Análisis ciudades patrimonio: 161.72 ms, 0 KB
- Análisis mayores 60 años: 142.90 ms, 0 KB
- **Total tiempo: 75395.65 ms**
- **Memoria máxima: 395708 KB**

**Versión con Estructuras + Referencias:**

=== RESUMEN DE ESTADÍSTICAS ===
- Crear datos: 4245.00 ms, 0 KB
- Mostrar resumen: 42770.00 ms, 0 KB
- Análisis longevidad en el país: 1290.00 ms, 0 KB
- Análisis longevidad por cada ciudad: 2106.00 ms, 0 KB
- Análisis patrimonio en el país: 893.00 ms, 0 KB
- Análisis patrimonio por cada ciudad: 2997.00 ms, 0 KB
- Análisis patrimonio por grupo de declaración: 1537.00 ms, 0 KB
- Análisis declarantes: 26439.00 ms, 0 KB
- Análisis ciudades patrimonio: 638.00 ms, 0 KB
- Análisis mayores 60 años: 381.00 ms, 0 KB
- **Total tiempo: 83296.00 ms**
- **Memoria máxima: 0 KB**

**Versión con Clases + Valores**

=== RESUMEN DE ESTADÍSTICAS ===
- Crear datos: 3326.91 ms, 395700 KB
- Mostrar resumen: 56318.53 ms, 0 KB
- Análisis longevidad en el país: 1235.95 ms, 0 KB
- Análisis longevidad por cada ciudad: 939.98 ms, 0 KB
- Análisis patrimonio en el país: 741.47 ms, 0 KB
- Análisis patrimonio por cada ciudad: 1075.01 ms, 0 KB
- Análisis patrimonio por grupo de declaración: 853.11 ms, 0 KB
- Análisis declarantes: 35075.12 ms, 19544 KB
- Análisis ciudades patrimonio: 471.74 ms, 0 KB
- Análisis mayores 60 años: 479.22 ms, 0 KB
- **Total tiempo: 100517.03 ms**
- **Memoria máxima: 395700 KB**


**Versión con Estructuras + Valores**

=== RESUMEN DE ESTADÍSTICAS ===
- Crear datos: 3995.00 ms, 0 KB
- Mostrar resumen: 57964.00 ms, 0 KB
- Análisis longevidad por país: 1122.00 ms, 0 KB
- Análisis longevidad por cada ciudad: 1476.00 ms, 0 KB
- Análisis patrimonio por país: 728.00 ms, 0 KB
- Análisis patrimonio por cada ciudad: 1325.00 ms, 0 KB
- Análisis patrimonio por grupo de declaración: 1917.00 ms, 0 KB
- Análisis declarantes: 38412.00 ms, 0 KB
- Análisis ciudades patrimonio: 1043.00 ms, 0 KB
- Análisis mayores 60 años: 687.00 ms, 0 KB
- **Total tiempo: 108669.00 ms**
- **Memoria máxima: 0 KB**

### Métricas adicionales RAM

#### Versión estructuras referencias

<img width="870" height="645" alt="image" src="https://github.com/user-attachments/assets/1f5c737c-a8cc-48d5-a4aa-833ca1c338ef" />

listado de objetos de persona



<img width="863" height="648" alt="image" src="https://github.com/user-attachments/assets/351c4986-6fe1-420c-a7f2-3189042e7dae" />

listado y cuenta de declarantes de renta



#### Versión Clases Referencias

<img width="858" height="655" alt="image" src="https://github.com/user-attachments/assets/890f3538-2544-4c02-86a8-f2b193685dff" />

listado de objetos de persona




<img width="867" height="654" alt="image" src="https://github.com/user-attachments/assets/f91e9fec-8cf2-4c74-ad02-3967f39dace8" />

listado de declarantes por grupo 



#### Version clases valores

<img width="856" height="654" alt="image" src="https://github.com/user-attachments/assets/a9fa1075-6c8d-45a3-8308-4f5d1ee0f07e" />

listado de objetos



<img width="858" height="645" alt="image" src="https://github.com/user-attachments/assets/9a39ad83-db0f-4575-9e79-3d121d97cf49" />

listado de delcarantes por grupo



#### Version estructuras valores

<img width="856" height="652" alt="image" src="https://github.com/user-attachments/assets/f5caf94b-5351-49e6-9411-eed1e002f29f" />

listado objetos de persona



<img width="868" height="646" alt="image" src="https://github.com/user-attachments/assets/91008199-06a1-4764-86de-bbb5b0fb61e6" />

listado de declarantes por grupo 



## Tabla de comparación final

| Versión               | Opción 6<br/>(Declarantes) | Opción 7<br/>(Ciudades) | Opción 8<br/>(Mayores 60) | Tiempo total | Pico de uso de memoria |
| --------------------- | -------------------------- | ----------------------- | ------------------------- | ------------ | ---------------------- |
| **Clases + Refs**     | 1,362ms                    | 143ms                   | 165ms                     | **2.53s**    | 408MB                  |
| **Structs + Refs**    | 1,700ms                    | 665ms                   | 386ms                     | **4.25s**    | 417MB                  |
| **Clases + Valores**  | 1,833ms (+35%)             | 353ms (+147%)           | 338ms (+105%)             | **3.40s**    | 804MB                  |
| **Structs + Valores** | 1,985ms (+17%)             | 923ms (+39%)            | 646ms (+67%)              | **5.06s**    | 813MB                  |

Podemos evidenciar que el uso de clases + acceso por referencia, fue la versión con tiempo de ejecución más reducido y a su vez con menor uso de memoria. Lo cual la convierte en una excelente opción para acceso a datos de esta complejidad y estructura, y además para métodos que involucran estructuras de datos más complejas. 

Las opciones del menú que se ven en la tabla fueron las opciones comparadas e implementadas en ambos referencias y valores, y clases y estructuras para comparar su rendimiento. 

Algunas observaciones relevantes sobre este comportamiento son 

1. **Costo por copias**: Las versiones que pasan por valor son 17-147% más lentas en las funciones que probé
2. **Impacto en memoria**: Usa casi que el doble de memoria (400MB → 800MB) porque copia los vectores completos
3. **Las clases ganan en este caso**: Aunque tengan métodos vs acceso directo, igual son más rápidas
4. **Análisis y cambios específicos**: Solo medí el impacto en las 3 funciones que cambiamos, el resto las dejé igual



### Preguntas críticas

#### 1. ¿Por qué referencias reducen 75% memoria?

Según los resultados obtenidos, las referecnias usan casi que el 50% de la memoria, ya que se elimina la copia completa de vectores en cada llamado a la función

#### 2. ¿Cómo optimizar búsquedas por grupo calendario?

Cálculo directo basado en últimos 2 dígitos del documento

```cpp
char calcularGrupo(const string& documento) {
    int ultimos = stoi(documento.substr(documento.length()-2));
    return (ultimos <= 39) ? 'A' : (ultimos <= 79) ? 'B' : 'C';
}
```

#### 3. ¿Array de structs vs vector de clases?

En este caso es mejor el vector de clases + referencias, ya que es casi un 68% más rápido que si usara structs. La localidad de memoria es similar 417MB vs 408MB.
#### 4. Escalabilidad con memoria virtual:

Con 2 millones de registros ya sentía que empezaba a poner un poco de problema, para mayor cantidad de datos como 10 millones, usaría procesamiento por lotes o memory mapping. El mmap es transparente, y el sistema operativo se encarga de manejar todo, mientras que en el procesamiento por lotes, se tiene control total y puede ser más predecible.


----

Link del video 

