/**
 * @file ArduinoSerial.cpp
 * @brief Implementación de la clase ArduinoSerial.
 * @details Esta clase se encarga de la comunicación serial con el Arduino para la recopilación de datos del mismo.
 * @author Carlos Gael Guerrero Salazar
 * @date 2025-10-29
 * @version 1.0
 */
#include "ArduinoSerial.h"
#include <cstdio>
#include <unistd.h>
#include <sys/ioctl.h>

/**
 * @brief Constructor de la clase ArduinoSerial.
 * @details Intenta abrir el puerto serial, configura los parámetros de comunicación (9600 baudios, 8N1, no canónico) 
 * y establece la conexión.
 * @param puerto Puntero a una cadena de caracteres con la ruta del dispositivo serial a utilizar (p. ej., "/dev/ttyUSB0").
 * @pre Los puertos seriales deben estar configurados correctamente en el sistema operativo.
 */
ArduinoSerial::ArduinoSerial(const char* puerto) : conectado(false) {
    // Intentar abrir el puerto en modo lectura/escritura y sin terminal de control (O_NOCTTY)
    serial_port = open(puerto, O_RDWR | O_NOCTTY);
    if (serial_port < 0) {
        std::cerr << "[ERROR] No se pudo abrir el puerto serial: " << puerto << " (Error: " << errno << ")" << std::endl;
        return;
    }

    struct termios tty;
    std::memset(&tty, 0, sizeof(tty));
        
    // Obtener atributos actuales
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "[ERROR] Error en tcgetattr" << std::endl;
        close(serial_port);
        serial_port = -1;
        return;
    }
    
    // Configurar velocidad (Baud Rate)
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    
    // Configuración CFLAGS (Control Mode)
    tty.c_cflag &= ~PARENB;     // Sin paridad
    tty.c_cflag &= ~CSTOPB;     // 1 bit de parada
    tty.c_cflag &= ~CSIZE;      // Limpiar bits de tamaño
    tty.c_cflag |= CS8;         // 8 bits de datos
    tty.c_cflag &= ~CRTSCTS;    // Deshabilitar control de flujo de hardware (RTS/CTS)
    tty.c_cflag |= CREAD | CLOCAL; // Habilitar recepción y no tomar control del puerto
    
    // Configuración LFLAGS (Local Mode)
    tty.c_lflag &= ~ICANON;     // Deshabilitar modo canónico (lectura byte a byte)
    tty.c_lflag &= ~ECHO;       // Deshabilitar eco
    tty.c_lflag &= ~ISIG;       // Deshabilitar manejo de señales especiales (INTR, QUIT, etc.)
    
    // Configuración IFLAGS (Input Mode)
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Deshabilitar control de flujo de software (XON/XOFF)
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Deshabilitar procesamiento especial de entrada
    
    // Configuración OFLAGS (Output Mode)
    tty.c_oflag &= ~OPOST;      // Deshabilitar procesamiento de salida
    tty.c_oflag &= ~ONLCR;      // Deshabilitar mapeo NL a CR-NL
    
    // Configuración CC (Control Characters) - Bloqueo de tiempo
    tty.c_cc[VTIME] = 10;       // Tiempo de espera (en décimas de segundo)
    tty.c_cc[VMIN] = 0;         // Retorno inmediato si no hay datos
    
    // Aplicar atributos
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "[ERROR] Error en tcsetattr" << std::endl;
        close(serial_port);
        serial_port = -1;
        return;
    }
    
    conectado = true;
    std::cout << "[OK] Puerto serial " << puerto << " abierto correctamente" << std::endl;
}


/**
 * @brief Lee una línea de datos terminada en salto de línea ('\n') desde el puerto serial.
 * @details Esta función utiliza una lectura bloqueante (con timeout por `VTIME`) y almacena
 * los bytes hasta encontrar un '\n' o alcanzar el límite de 255 caracteres.
 * @return Un puntero estático a la cadena de caracteres con la línea leída, o `nullptr` si la conexión no está activa o si ocurre un error de lectura.
 * @pre Debe haber una conexión activa (`conectado == true`) con el puerto serial.
 */
char* ArduinoSerial::leerLinea() {
    if (!conectado) {
        return nullptr;
    }
    
    static char linea[256];
    int idx = 0;
    char buf;
    
    while (idx < 255) {
        int n = read(serial_port, &buf, 1);
        
        if (n > 0) {
            if (buf == '\n') {
                linea[idx] = '\0';
                return linea;
            } else if (buf != '\r') {
                linea[idx++] = buf;
            }
        } else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            // Error real de lectura
            std::cerr << "[ERROR] Error leyendo del puerto serial" << std::endl;
            return nullptr;
        }
    }
    linea[255] = '\0';
    return linea;
}

/**
 * @brief Verifica si el objeto `ArduinoSerial` ha establecido una conexión exitosa con el puerto.
 * @return `true` si el descriptor de archivo es válido y la configuración fue exitosa; `false` en caso contrario.
 */
bool ArduinoSerial::estaConectado() const {
    return conectado;
}

/**
 * @brief Destructor de la clase ArduinoSerial.
 * @details Cierra el descriptor de archivo del puerto serial si está abierto y la conexión está marcada como activa, liberando el recurso del sistema operativo.
 */
ArduinoSerial::~ArduinoSerial() {
    if (conectado && serial_port >= 0) {
        close(serial_port);
        std::cout << "[OK] Puerto serial cerrado" << std::endl;
    }
}

void ArduinoSerial::iniciarArduinoSerial() {
    int status;
    ioctl(serial_port, TIOCMGET, &status);
    status &= ~TIOCM_DTR;  // Bajar DTR
    ioctl(serial_port, TIOCMSET, &status);
    usleep(100000);  // 100ms
    
    status |= TIOCM_DTR;   // Subir DTR
    ioctl(serial_port, TIOCMSET, &status);
    sleep(2);
}