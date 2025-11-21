#ifndef BARBERSHOP_H
#define BARBERSHOP_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <string>

/**
 * @brief Clase que representa la barbería
 * 
 * Implementa el problema clásico del "Barbero Durmiente" usando:
 * - Mutex para exclusión mutua
 * - Variables de condición para sincronización
 * - Atomic para contadores thread-safe
 */
class Barbershop {
private:
    // Configuración de la barbería
    const int MAX_WAITING_CHAIRS;
    
    // Estado de la barbería
    std::queue<int> waiting_queue;      // Cola de clientes esperando
    std::atomic<int> waiting_customers; // Número de clientes esperando
    std::atomic<bool> barber_sleeping;  // Estado del barbero
    std::atomic<bool> shop_open;        // ¿Está abierta la barbería?
    
    // Sincronización
    std::mutex shop_mutex;              // Protege acceso a recursos compartidos
    std::condition_variable customer_ready;  // Señal: hay cliente listo
    std::condition_variable barber_ready;    // Señal: barbero listo para atender
    std::condition_variable customer_done;   // Señal: cliente terminó
    
    // Estadísticas
    std::atomic<int> customers_served;  // Total de clientes atendidos
    std::atomic<int> customers_rejected; // Clientes rechazados (sin sillas)
    
    // Cliente actual siendo atendido
    int current_customer;
    bool customer_in_chair;

public:
    /**
     * @brief Constructor de la barbería
     * @param max_chairs Número máximo de sillas de espera
     */
    explicit Barbershop(int max_chairs);
    
    /**
     * @brief Destructor
     */
    ~Barbershop();
    
    /**
     * @brief El barbero intenta atender al siguiente cliente
     * Duerme si no hay clientes, despierta cuando llega uno
     */
    void barber_work();
    
    /**
     * @brief Un cliente intenta obtener un corte de pelo
     * @param customer_id ID del cliente
     * @return true si fue atendido, false si la barbería estaba llena
     */
    bool customer_arrives(int customer_id);
    
    /**
     * @brief Cierra la barbería (no acepta más clientes)
     */
    void close_shop();
    
    /**
     * @brief Obtiene estadísticas de la barbería
     */
    void print_statistics() const;
    
    /**
     * @brief Verifica si la barbería está abierta
     */
    bool is_open() const;
    
    /**
     * @brief Obtiene el número de clientes esperando
     */
    int get_waiting_customers() const;
};

#endif
