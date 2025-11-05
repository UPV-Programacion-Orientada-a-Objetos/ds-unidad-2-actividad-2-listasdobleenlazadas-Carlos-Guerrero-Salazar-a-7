/**
 * @file TramaBase.h
 * @brief Definición de la jerarquía de clases para las tramas del protocolo PRT-7
 * @ingroup data_management
 */
#ifndef TRAMABASE_H
#define TRAMABASE_H
#include <iostream>
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

/**
 * @class TramaBase
 * @brief Clase base abstracta para todas las tramas del protocolo PRT-7
 * @details Define la interfaz común para TramaLoad y TramaMap
 */
class TramaBase {
public:
    /**
     * @brief Método virtual puro para procesar la trama
     * @param carga Puntero a la lista de carga donde se almacena el mensaje
     * @param rotor Puntero al rotor de mapeo que realiza la decodificación
     */
    virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;
    
    /**
     * @brief Destructor virtual para asegurar la limpieza polimórfica
     */
    virtual ~TramaBase() {}
};

/**
 * @class TramaLoad
 * @brief Representa una trama de carga (L,X) que contiene un carácter
 */
class TramaLoad : public TramaBase {
private:
    char caracter; ///< Carácter contenido en la trama
    
public:
    /**
     * @brief Constructor que inicializa la trama con un carácter
     * @param c Carácter a almacenar
     */
    TramaLoad(char c) : caracter(c) {}
    
    /**
     * @brief Procesa la trama decodificando el carácter y agregándolo a la lista
     * @param carga Lista donde se agrega el carácter decodificado
     * @param rotor Rotor utilizado para decodificar
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override {
        // Decodificar el carácter usando el rotor
        char decodificado = NULL;
        if(carga) {decodificado = rotor->getMapeo(caracter);}
        carga->insertarAlFinal(decodificado);
        
        // Mensaje de depuración
        if(caracter==' '){

            std::cout << "Trama recibida: [L,Space" << "] -> Procesando... -> ";
            std::cout << "Fragmento 'Space' decodificado como '" << decodificado << "'. ";
        }else{
        std::cout << "Trama recibida: [L," << caracter << "] -> Procesando... -> ";
        std::cout << "Fragmento '" << caracter << "' decodificado como '" << decodificado << "'. ";
        }
        std::cout << "Mensaje: ";
        carga->imprimirMensajeDetallado();
    }
    
    /**
     * @brief Obtiene el carácter almacenado
     * @return El carácter de la trama
     */
    char getCaracter() const { return caracter; }
};

/**
 * @class TramaMap
 * @brief Representa una trama de mapeo (M,N) que rota el disco de cifrado
 */
class TramaMap : public TramaBase {
private:
    int rotacion; ///< Cantidad de rotación (puede ser positiva o negativa)
    
public:
    /**
     * @brief Constructor que inicializa la trama con un valor de rotación
     * @param n Valor de rotación
     */
    TramaMap(int n) : rotacion(n) {}
    
    /**
     * @brief Procesa la trama rotando el disco de cifrado
     * @param carga No se utiliza en esta trama
     * @param rotor Rotor que será rotado
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override {
        // Rotar el disco de cifrado
        rotor->rotar(rotacion);
        
        // Mensaje de depuración
        std::cout << "\nTrama recibida: [M," << rotacion << "] -> Procesando... -> ";
        std::cout << "ROTANDO ROTOR ";
        if (rotacion > 0) std::cout << "+";
        std::cout << rotacion << ".\n";
    }

    
    /**
     * @brief Obtiene el valor de rotación
     * @return Cantidad de rotación
     */
    int getRotacion() const { return rotacion; }
};
#endif // TRAMABASE_H