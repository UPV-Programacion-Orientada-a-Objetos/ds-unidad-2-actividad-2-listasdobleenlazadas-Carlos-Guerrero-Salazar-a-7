/**
 * @file RotorDeMapeo.h
 * @brief Implementación de la lista circular doblemente enlazada para el disco de cifrado
 * @ingroup data_management
 */
#ifndef ROTORDEMAPEO_H
#define ROTORDEMAPEO_H

#include <iostream>

/**
 * @class RotorDeMapeo
 * @brief Lista circular doblemente enlazada que representa el disco de cifrado
 * @details Contiene el alfabeto (A-Z y espacio) y puede rotar para cambiar el mapeo
 */
class RotorDeMapeo {
private:
    /**
     * @struct Nodo
     * @brief Nodo de la lista circular que contiene un carácter
     */
    struct Nodo {
        char dato;         ///< Carácter almacenado
        Nodo* siguiente;   ///< Puntero al siguiente nodo
        Nodo* anterior;    ///< Puntero al nodo anterior
        
        Nodo(char c) : dato(c), siguiente(nullptr), anterior(nullptr) {}
    };
    
    Nodo* cabeza; ///< Puntero que marca la posición 'cero' actual del rotor
    
public:
    RotorDeMapeo() : cabeza(nullptr) {
        const char alfabeto[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int tam = 26;
        
        Nodo* ultimo = nullptr;
        
        for (int i = 0; i < tam; i++) {
            Nodo* nuevo = new Nodo(alfabeto[i]);
            
            if (!cabeza) {
                cabeza = nuevo;
                ultimo = nuevo;
            } else {
                ultimo->siguiente = nuevo;
                nuevo->anterior = ultimo;
                ultimo = nuevo;
            }
        }
        
        // Cerrar el círculo
        if (cabeza && ultimo) {
            ultimo->siguiente = cabeza;
            cabeza->anterior = ultimo;
        }
    }

    void rotar(int n) {
        if (!cabeza) return;
        
        if (n > 0) {
            // Rotar hacia adelante
            for (int i = 0; i < n; i++) {
                cabeza = cabeza->siguiente;
            }
        } else if (n < 0) {
            // Rotar hacia atrás
            for (int i = 0; i < -n; i++) {
                cabeza = cabeza->anterior;
            }
        }
    }

    char getMapeo(char entrada) {
        if (!cabeza) return entrada;
        if (entrada == ' ') {
            return ' ';
        }
        int posicion;
        if (entrada >= 'A' && entrada <= 'Z') {
            posicion = entrada - 'A';
        } else {
            return entrada;
        }
        
        Nodo* resultado = cabeza;
        for (int i = 0; i < posicion; i++) {
            resultado = resultado->siguiente;
        }
        
        return resultado->dato;
    }

    void imprimir() const {
        if (!cabeza) {
            std::cout << "[ROTOR VACIO]" << std::endl;
            return;
        }
        
        std::cout << "[ROTOR] Cabeza en '" << cabeza->dato << "': ";
        Nodo* actual = cabeza;
        int count = 0;
        do {
            std::cout << actual->dato;
            actual = actual->siguiente;
            count++;
            if (count > 30) break; // Seguridad anti-loop infinito
        } while (actual != cabeza);
        std::cout << std::endl;
    }

    ~RotorDeMapeo() {
        if (!cabeza) return;
        
        // Romper el círculo
        Nodo* ultimo = cabeza->anterior;
        if (ultimo) {
            ultimo->siguiente = nullptr;
        }
        
        // Eliminar todos los nodos
        while (cabeza) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

#endif // ROTORDEMAPEO_H
