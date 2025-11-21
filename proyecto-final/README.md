# Problema del Barbero Durmiente

## Descripción

Implementación completa del problema clásico de sincronización "El Barbero Durmiente" (Sleeping Barber Problem) utilizando hilos POSIX, mutex y variables de condición en C++

## Objetivos

1. **Evitar deadlocks**: Ningún hilo debe quedar bloqueado indefinidamente
2. **Evitar inanición**: Todos los clientes eventualmente son atendidos o rechazados
3. **Sincronización correcta**: Los recursos compartidos están protegidos adecuadamente
4. **Eficiencia**: Uso óptimo de recursos del sistema

## Arquitectura

### Componentes Principales

- **Barbershop**: Clase que maneja toda la lógica de sincronización
- **Barber Thread**: Hilo que representa al barbero
- **Customer Threads**: Hilos que representan a los clientes

### Mecanismos de Sincronización

1. **Mutex** (`std::mutex`): Protege el acceso a recursos compartidos
2. **Variables de condición** (`std::condition_variable`):
   - `customer_ready`: Muestra que hay un cliente esperando
   - `barber_ready`: Muestra que el barbero está listo
   - `customer_done`: Muestra que el cliente terminó de sentarse

3. **Atomic Variables** (`std::atomic`):
   - Contadores thread-safe sin necesidad de locks
   - Estado del barbero y la barbería

## Garantías de Sincronización

### Prevención de Deadlocks

- **Orden de adquisición de locks**: Siempre se adquiere el mismo mutex
- **Timeouts**: No se usan locks indefinidos
- **Variables de condición**: Evitan espera activa

### Prevención de Inanición

- **FIFO Queue**: Los clientes son atendidos en orden de llegada
- **Límite de espera**: Clientes rechazados inmediatamente si no hay espacio
- **Señalización explícita**: Cada cliente es notificado individualmente

## 🚀 Compilación y Ejecución

```bash
# Compilar
make

# Ejecutar
make run

# Limpiar
make clean
```

## Modos de Simulación

1. **Simulación rápida**: 10 clientes, 3 sillas
2. **Simulación con sobrecarga**: 20 clientes, 3 sillas (prueba de rechazo, demasiados clientes)
3. **Simulación extendida**: 30 clientes, 5 sillas
4. **Configuración personalizada**: Parámetros a elección
5. **Modo demostración**: Visualización paso a paso

## Casos de Prueba

### Test 1: Barbero Durmiente
```
Condición: No hay clientes
Resultado: Barbero duerme hasta que llega un cliente
```

### Test 2: Sala de Espera Llena
```
Condición: Todos los asientos ocupados
Resultado: Nuevos clientes son rechazados
```

### Test 3: Múltiples Clientes
```
Condición: Varios clientes llegando simultáneamente
Resultado: Se atienden en orden FIFO
```

### Test 4: Cierre de Barbería
```
Condición: Cerrar con clientes esperando
Resultado: Se atienden todos antes de cerrar
```

## Estadísticas

El programa muestra:
- Clientes atendidos
- Clientes rechazados
- Tasa de éxito
- Capacidad utilizada

## Detalles de Implementación

### Estado del Sistema

Estado de la barbería:
- waiting_queue: Cola FIFO de clientes
- barber_sleeping: El barbero está dormido?
- shop_open: La barbería está abierta? (Pueden entrar clientes)
- customer_in_chair: El cliente está siendo atendido o no?

### Flujo de Ejecución

#### Cliente:
1. Verifica si hay espacio
2. Se agrega a la cola
3. Despierta al barbero si está durmiendo
4. Espera su turno
5. Se sienta en la silla
6. Espera el corte
7. Se va

#### Barbero:
1. Verifica si hay clientes
2. Duerme si no hay nadie
3. Despierta cuando llega un cliente
4. Llama al siguiente cliente
5. Corta el pelo
6. Repite

## Notas adiicionales

- El tiempo de corte de pelo es aleatorio (500-1500ms)
- La llegada de clientes es aleatoria
- La implementación es determinística (no hay condiciones de carrerea)
- Compatible con Linux/Unix (usa POSIX threads)
