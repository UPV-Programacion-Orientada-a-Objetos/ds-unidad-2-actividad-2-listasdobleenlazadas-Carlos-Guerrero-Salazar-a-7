/**
 * @file main.cpp
 * @brief Programa principal del decodificador PRT-7
 * @details Lee tramas desde el Arduino y decodifica el mensaje oculto
 */
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "ArduinoSerial.h"
#include "TramaBase.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

/**
 * @brief Parsea una línea del serial y crea la trama correspondiente
 * @param linea Cadena recibida del serial (ej. "L,A" o "M,5")
 * @return Puntero a TramaBase (TramaLoad o TramaMap) o nullptr si hay error
 */
TramaBase* parsearTrama(const char* linea) {
    if (!linea || linea[0] == '\0') {
        return nullptr;
    }
    
    // Verificar formato mínimo: "X,Y"
    if (std::strlen(linea) < 3 || linea[1] != ',') {
        std::cerr << "[ERROR] Trama mal formada: " << linea << std::endl;
        return nullptr;
    }
    
    char tipo = linea[0];
    
    if (tipo == 'L') {
        // Trama de carga: L,X donde X es un carácter
        const char* dato = &linea[2];
        if (std::strcmp(dato, "Space") == 0) {
            std::cout << "[INFO] Trama especial L,Space recibida.\n";
            return new TramaLoad(' '); // Crea una TramaLoad con el carácter espacio
        }
        char caracter = linea[2];
        return new TramaLoad(caracter);
    } 
    else if (tipo == 'M') {
        // Trama de mapeo: M,N donde N es un entero
        int rotacion = std::atoi(&linea[2]);
        return new TramaMap(rotacion);
    } 
    else {
        std::cerr << "[ERROR] Tipo de trama desconocido: " << tipo << std::endl;
        return nullptr;
    }
}

int main() {
    std::cout << "   Iniciando Decodificador\n";
    
    // 1. Conectar al Arduino
    std::cout << "Conectando a puerto COM...\n";
    ArduinoSerial* arduino = new ArduinoSerial("/dev/ttyUSB0");
    
    if (!arduino->estaConectado()) {
        std::cerr << "[ERROR] No se pudo conectar al Arduino." << std::endl;
        delete arduino;
        return 1;
    }
    
    std::cout << "Conexión establecida. Esperando tramas...\n\n";
    arduino->iniciarArduinoSerial();
    ListaDeCarga* listaCarga = new ListaDeCarga();
    RotorDeMapeo* rotor = new RotorDeMapeo();
    int tramasProcesadas = 0;
    const int MAX_TRAMAS = 100;
    
    while (tramasProcesadas < MAX_TRAMAS) {
        // Leer línea del serial
        char* linea = arduino->leerLinea();
        
        if (!linea) {
            // No hay datos disponibles, esperar un poco
            continue;
        }
        
        // Verificar si la línea está vacía
        if (linea[0] == '\0') {
            continue;
        }
        
        // Verificar si es el fin del flujo (puedes enviar "FIN" desde Arduino)
        if (std::strcmp(linea, "FIN") == 0) {
            std::cout << "\n[INFO] Señal de fin recibida.\n";
            break;
        }
        
        // Parsear la trama
        TramaBase* trama = parsearTrama(linea);
        
        if (trama) {
            // Procesar la trama (polimorfismo en acción)
            trama->procesar(listaCarga, rotor);
            
            // Liberar memoria de la trama
            delete trama;
            tramasProcesadas++;
        }
    }
    
    // 4. Mostrar resultado final
    std::cout << "Flujo de datos terminado.\n";
    std::cout << "MENSAJE OCULTO ENSAMBLADO:\n";
    listaCarga->imprimirMensaje();
    
    // 5. Liberar memoria
    std::cout << "Liberando memoria... ";
    delete listaCarga;
    delete rotor;
    delete arduino;
    std::cout << "Sistema apagado.\n";
    
    return 0;
}