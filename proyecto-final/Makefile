# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O2
TARGET = barbero_durmiente
SOURCES = main.cpp barbershop.cpp
HEADERS = barbershop.h
OBJECTS = $(SOURCES:.cpp=.o)

# Colores para output
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
NC = \033[0m # No Color

.PHONY: all clean run help

all: $(TARGET)
	@echo "$(GREEN) Compilación exitosa!$(NC)"

$(TARGET): $(OBJECTS)
	@echo "$(BLUE) Enlazando...$(NC)"
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	@echo "$(BLUE) Compilando $<...$(NC)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(RED)🧹 Limpiando...$(NC)"
	rm -f $(OBJECTS) $(TARGET)
	@echo "$(GREEN) Limpieza completada$(NC)"

run: $(TARGET)
	@echo "$(GREEN) Ejecutando simulación...$(NC)"
	@echo ""
	./$(TARGET)

help:
	@echo "$(BLUE)Comandos disponibles:$(NC)"
	@echo "  make          - Compilar el proyecto"
	@echo "  make run      - Compilar y ejecutar"
	@echo "  make clean    - Limpiar archivos compilados"
	@echo "  make help     - Mostrar esta ayuda"
