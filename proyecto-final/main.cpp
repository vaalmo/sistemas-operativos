#include "barbershop.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <csignal>
#include <atomic>

// Variable global para manejar señales
std::atomic<bool> interrupted(false);

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\n\n⚠️  Señal de interrupción recibida...\n";
        interrupted = true;
    }
}

/**
 * @brief Simula la llegada de clientes a la barbería
 * @param shop Referencia a la barbería
 * @param customer_id ID del cliente
 * @param arrival_delay Tiempo antes de llegar (ms)
 */
void customer_thread(Barbershop& shop, int customer_id, int arrival_delay) {
    // Simular tiempo de llegada
    std::this_thread::sleep_for(std::chrono::milliseconds(arrival_delay));
    
    if (interrupted) return;
    
    std::cout << " Cliente #" << customer_id << ": Llegando a la barbería...\n";
    
    // Intentar obtener un corte de pelo
    shop.customer_arrives(customer_id);
}

/**
 * @brief Imprime el menú de opciones
 */
void print_menu() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "💈 SIMULADOR DEL BARBERO DURMIENTE 💈\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "1. Simulación rápida (10 clientes, 3 sillas)\n";
    std::cout << "2. Simulación con sobrecarga (20 clientes, 3 sillas)\n";
    std::cout << "3. Simulación extendida (30 clientes, 5 sillas)\n";
    std::cout << "4. Configuración personalizada\n";
    std::cout << "5. Modo demostración (visualización paso a paso)\n";
    std::cout << "0. Salir\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "Seleccione una opción: ";
}

/**
 * @brief Ejecuta una simulación con parámetros dados
 */
void run_simulation(int num_customers, int num_chairs, int max_arrival_delay) {
    std::cout << "\n Iniciando simulación...\n";
    std::cout << " Número de clientes: " << num_customers << "\n";
    std::cout << " Sillas de espera: " << num_chairs << "\n";
    std::cout << " Retraso máximo de llegada: " << max_arrival_delay << " ms\n\n";
    
    // Crear la barbería
    Barbershop shop(num_chairs);
    
    // Crear hilo del barbero
    std::thread barber([&shop]() {
        shop.barber_work();
    });
    
    // Crear hilos de clientes
    std::vector<std::thread> customers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> delay_dist(0, max_arrival_delay);
    
    for (int i = 1; i <= num_customers && !interrupted; ++i) {
        int delay = delay_dist(gen);
        customers.emplace_back(customer_thread, std::ref(shop), i, delay);
    }
    
    // Esperar a que todos los clientes terminen
    for (auto& customer : customers) {
        if (customer.joinable()) {
            customer.join();
        }
    }
    
    // Dar tiempo para que se procesen los últimos clientes
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Cerrar la barbería
    shop.close_shop();
    
    // Esperar a que el barbero termine
    if (barber.joinable()) {
        barber.join();
    }
    
    // Mostrar estadísticas
    shop.print_statistics();
}

/**
 * @brief Modo demostración con pausa entre eventos
 */
void demo_mode() {
    std::cout << "\n MODO DEMOSTRACIÓN - Visualización paso a paso\n";
    std::cout << "Presione Enter después de cada paso...\n\n";
    
    Barbershop shop(2);  // Solo 2 sillas para demo
    
    std::thread barber([&shop]() {
        shop.barber_work();
    });
    
    std::cout << "\n[Paso 1] Barbero esperando en barbería vacía...\n";
    std::cin.get();
    
    std::thread c1([&shop]() {
        shop.customer_arrives(1);
    });
    
    std::cout << "\n[Paso 2] Cliente #1 llega y despierta al barbero...\n";
    std::cin.get();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::thread c2([&shop]() {
        shop.customer_arrives(2);
    });
    
    std::cout << "\n[Paso 3] Cliente #2 llega mientras #1 está siendo atendido...\n";
    std::cin.get();
    
    std::thread c3([&shop]() {
        shop.customer_arrives(3);
    });
    
    std::cout << "\n[Paso 4] Cliente #3 llega (sala de espera tiene 2 sillas)...\n";
    std::cin.get();
    
    std::thread c4([&shop]() {
        shop.customer_arrives(4);
    });
    
    std::cout << "\n[Paso 5] Cliente #4 llega (¿habrá espacio?)...\n";
    std::cin.get();
    
    c1.join();
    c2.join();
    c3.join();
    c4.join();
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    shop.close_shop();
    
    if (barber.joinable()) {
        barber.join();
    }
    
    shop.print_statistics();
}

int main() {
    // Configurar manejo de señales
    signal(SIGINT, signal_handler);
    
    std::cout << "💈💈💈 PROBLEMA DEL BARBERO DURMIENTE 💈💈💈\n";
    std::cout << "Implementación con hilos, mutex y variables de condición\n";
    
    int choice;
    
    while (!interrupted) {
        print_menu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << " Entrada inválida. Intente de nuevo.\n";
            continue;
        }
        std::cin.ignore();
        
        switch (choice) {
            case 0:
                std::cout << " ¡Hasta luego!\n";
                return 0;
                
            case 1:
                run_simulation(10, 3, 500);
                break;
                
            case 2:
                run_simulation(20, 3, 300);
                break;
                
            case 3:
                run_simulation(30, 5, 400);
                break;
                
            case 4: {
                int customers, chairs, delay;
                std::cout << "Número de clientes: ";
                std::cin >> customers;
                std::cout << "Número de sillas: ";
                std::cin >> chairs;
                std::cout << "Retraso máximo de llegada (ms): ";
                std::cin >> delay;
                std::cin.ignore();
                
                if (customers > 0 && chairs > 0 && delay >= 0) {
                    run_simulation(customers, chairs, delay);
                } else {
                    std::cout << " Valores inválidos\n";
                }
                break;
            }
                
            case 5:
                demo_mode();
                break;
                
            default:
                std::cout << " Opción inválida\n";
        }
        
        if (!interrupted && choice != 0) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.get();
        }
    }
    
    std::cout << "\n Programa terminado\n";
    return 0;
}
