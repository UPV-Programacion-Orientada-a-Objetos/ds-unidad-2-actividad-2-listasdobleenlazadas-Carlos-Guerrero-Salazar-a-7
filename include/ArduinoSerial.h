/**
 * @page arduino Arduinos
 * @file ArduinoSerial.h
 * @brief Clase para la comunicación serie con Arduino.
 * @ingroup hardware
 */
#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <typeinfo>
#include <cstdio> // Para sscanf

/**
 * @class ArduinoSerial
 * @brief Genera un objeto para gestionar la comunicación serial con un dispositivo (ej. Arduino).
 * @details Esta clase se encarga de abrir y configurar el puerto serial, leer líneas de datos
 * y procesarlas para convertirlas a tipos numéricos (int o float) según sea necesario,
 * manejando el polimorfismo a través de plantillas.
 */
class ArduinoSerial {
private:
    int serial_port;     ///< Descriptor de archivo para el puerto serial.
    bool conectado;      ///< Indicador del estado de la conexión serial.
    
    /**
     * @brief Verifica si una cadena de caracteres contiene un separador decimal ('.' o ',').
     * @param str Cadena de caracteres a analizar.
     * @return true si la cadena contiene un punto o coma decimal, false en caso contrario.
     */
    bool tieneDecimal(const char* str) {
        if (*str == '-' || *str == '+') {
            str++;
        }
        while (*str != '\0') {
            if (*str == '.' || *str == ',') {
                return true;
            }
            str++;
        }
        return false;
    }

public:
    /**
     * @brief Constructor de la clase ArduinoSerial.
     * * Intenta abrir y configurar el puerto serial especificado.
     * @param puerto Cadena de caracteres con la ruta al dispositivo serial (p. ej., "/dev/ttyUSB0").
     */
    ArduinoSerial(const char* puerto = "/dev/ttyUSB0");

    /**
     * @brief Lee una línea de datos terminada en salto de línea desde el puerto serial.
     * @return Puntero a la cadena de caracteres leída, o nullptr en caso de error o no conectado.
     */
    char* leerLinea();
    
    /**
     * @brief Función plantilla para procesar y entregar un dato numérico del tipo esperado.
     * @details Esta función llama repetidamente a leerLinea() hasta que la línea leída 
     * pueda ser parseada como el tipo de dato especificado por la plantilla (T).
     * * Si T es int, solo acepta números enteros. Si T es float, acepta números con o sin decimales.
     * * @tparam T Tipo de dato esperado (generalmente int o float).
     * @return Un dato del tipo T si la lectura y conversión son exitosas. Retorna T() (valor por defecto, ej. 0 o 0.0f) en caso de fallo de lectura.
     */
    
    /**
     * @brief Comprueba el estado de la conexión serial.
     * @return true si el puerto serial está abierto y configurado, false en caso contrario.
     */
    bool estaConectado() const;

    /**
     * @brief Destructor de la clase ArduinoSerial.
     * * Cierra el descriptor de archivo del puerto serial si está abierto.
     */
    ~ArduinoSerial();

    void iniciarArduinoSerial();
};

#endif // SERIALREADER_H