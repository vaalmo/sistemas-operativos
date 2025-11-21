#include "barbershop.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

Barbershop::Barbershop(int max_chairs)
    : MAX_WAITING_CHAIRS(max_chairs),
      waiting_customers(0),
      barber_sleeping(true),
      shop_open(true),
      customers_served(0),
      customers_rejected(0),
      current_customer(-1),
      customer_in_chair(false) {
    
    std::cout << " Barbería abierta con " << MAX_WAITING_CHAIRS 
              << " sillas de espera\n";
}

Barbershop::~Barbershop() {
    close_shop();
}

void Barbershop::barber_work() {
    while (true) {
        std::unique_lock<std::mutex> lock(shop_mutex);
        
        // Si no hay clientes y la barbería está abierta, el barbero duerme
        while (waiting_queue.empty() && shop_open) {
            if (!barber_sleeping) {
                std::cout << "💤 Barbero: No hay clientes, me voy a dormir...\n";
                barber_sleeping = true;
            }
            customer_ready.wait(lock);
        }
        
        // Si la barbería cerró y no hay más clientes, terminar
        if (!shop_open && waiting_queue.empty()) {
            std::cout << "🔚 Barbero: Barbería cerrada, me voy pa la casa\n";
            break;
        }
        
        // Despertar si estaba durmiendo
        if (barber_sleeping) {
            std::cout << " Barbero: ¡Despierto! Hay clientes esperando\n";
            barber_sleeping = false;
        }
        
        // Tomar al siguiente cliente de la cola
        current_customer = waiting_queue.front();
        waiting_queue.pop();
        waiting_customers--;
        customer_in_chair = true;
        
        std::cout << "  Barbero: Llamando al cliente #" << current_customer 
                  << " (Esperando: " << waiting_customers << ")\n";
        
        // Señalar que el barbero está listo
        barber_ready.notify_one();
        
        // Esperar a que el cliente se siente
        customer_done.wait(lock, [this] { return !customer_in_chair; });
        
        lock.unlock();
        
        // Simular el tiempo de corte de pelo (fuera del lock)
        std::cout << " Barbero: Cortando pelo al cliente #" << current_customer << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 1000));
        
        customers_served++;
        std::cout << "✅ Barbero: Cliente #" << current_customer 
                  << " terminado (Total atendidos: " << customers_served << ")\n";
    }
}

bool Barbershop::customer_arrives(int customer_id) {
    std::unique_lock<std::mutex> lock(shop_mutex);
    
    // Verificar si la barbería está cerrada
    if (!shop_open) {
        std::cout << " Cliente #" << customer_id 
                  << ": Barbería cerrada, me voy\n";
        return false;
    }
    
    // Verificar si hay espacio en la sala de espera
    if (waiting_customers >= MAX_WAITING_CHAIRS) {
        customers_rejected++;
        std::cout << " Cliente #" << customer_id 
                  << ": No hay sillas libres, me voy (Rechazados: " 
                  << customers_rejected << ")\n";
        return false;
    }
    
    // Agregar cliente a la cola de espera
    waiting_queue.push(customer_id);
    waiting_customers++;
    
    std::cout << " Cliente #" << customer_id 
              << ": Me siento a esperar (Posición: " << waiting_customers << ")\n";
    
    // Si el barbero está durmiendo, despertarlo
    if (barber_sleeping) {
        std::cout << " Cliente #" << customer_id 
                  << ": ¡Despertando al barbero!\n";
        customer_ready.notify_one();
    }
    
    // Esperar a que el barbero esté listo para atender
    barber_ready.wait(lock, [this, customer_id] { 
        return current_customer == customer_id; 
    });
    
    std::cout << " Cliente #" << customer_id 
              << ": Me siento en la silla del barbero\n";
    
    // Simular sentarse en la silla
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    customer_in_chair = false;
    customer_done.notify_one();
    
    lock.unlock();
    
    std::cout << " Cliente #" << customer_id 
              << ": ¡Gracias! Me voy con mi nuevo corte\n";
    
    return true;
}

void Barbershop::close_shop() {
    std::lock_guard<std::mutex> lock(shop_mutex);
    shop_open = false;
    customer_ready.notify_all();
    std::cout << "\n Barbería cerrada - No se aceptan más clientes\n";
}

bool Barbershop::is_open() const {
    return shop_open;
}

int Barbershop::get_waiting_customers() const {
    return waiting_customers;
}

void Barbershop::print_statistics() const {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << " ESTADÍSTICAS DE LA BARBERÍA\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << " Clientes atendidos: " << customers_served << "\n";
    std::cout << " Clientes rechazados: " << customers_rejected << "\n";
    std::cout << " Total de clientes: " << (customers_served + customers_rejected) << "\n";
    std::cout << " Capacidad de espera: " << MAX_WAITING_CHAIRS << " sillas\n";
    
    if (customers_served + customers_rejected > 0) {
        double success_rate = (100.0 * customers_served) / (customers_served + customers_rejected);
        std::cout << " Tasa de éxito: " << std::fixed << std::setprecision(2) 
                  << success_rate << "%\n";
    }
    std::cout << std::string(50, '=') << "\n";
}
