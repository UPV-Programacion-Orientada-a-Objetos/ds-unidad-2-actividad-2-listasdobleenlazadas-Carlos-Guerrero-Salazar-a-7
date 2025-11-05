/**
 * @file ListaDeCarga.h
 * @brief Implementación de la lista doblemente enlazada para almacenar el mensaje
 * @ingroup data_management
 */
#ifndef LISTADECARGA_H
#define LISTADECARGA_H

#include <iostream>

/**
 * @class ListaDeCarga
 * @brief Lista doblemente enlazada que almacena los caracteres decodificados
 * @details Mantiene el orden de llegada de los caracteres decodificados
 */
class ListaDeCarga {
private:
    /**
     * @struct Nodo
     * @brief Nodo de la lista que contiene un carácter decodificado
     */
    struct Nodo {
        char dato;         ///< Carácter decodificado
        Nodo* siguiente;   ///< Puntero al siguiente nodo
        Nodo* anterior;    ///< Puntero al nodo anterior
        
        Nodo(char c) : dato(c), siguiente(nullptr), anterior(nullptr) {}
    };
    
    Nodo* cabeza; ///< Primer nodo de la lista
    Nodo* cola;   ///< Último nodo de la lista
    
public:
    ListaDeCarga() : cabeza(nullptr), cola(nullptr) {}

    void insertarAlFinal(char c) {
        Nodo* nuevo = new Nodo(c);
        
        if (!cabeza) {
            // Lista vacía
            cabeza = nuevo;
            cola = nuevo;
        } else {
            // Agregar al final
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }

    void imprimirMensaje() const {
        if (!cabeza) {
            std::cout << "[MENSAJE VACIO]" << std::endl;
            return;
        }
        
        Nodo* actual = cabeza;
        while (actual) {
            std::cout << actual->dato;
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }

    void imprimirMensajeDetallado() const {
        if (!cabeza) {
            std::cout << "[MENSAJE VACIO]" << std::endl;
            return;
        }
        
        Nodo* actual = cabeza;
        while (actual) {
            std::cout << "[" << actual->dato << "]";
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }

    ~ListaDeCarga() {
        while (cabeza) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

#endif // LISTADECARGA_H
