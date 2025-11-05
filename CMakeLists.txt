# Define la versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.10)

# Define el nombre del proyecto Y LA VERSIÓN
project(ProyectoArduino VERSION 1.0.0 LANGUAGES CXX)

# Establece el estándar de C++ (por ejemplo, C++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Define las rutas de inclusión
include_directories(include)

# 2. Define las fuentes (archivos .cpp)
file(GLOB SOURCES
    src/*.cpp
    main.cpp
)

# 3. Define el ejecutable
add_executable(proyectomain ${SOURCES})

# Opcional: Define la ruta de salida del ejecutable
set_target_properties(proyectomain PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# --- INTEGRACIÓN DOXYGEN ---

# 4. Busca el programa Doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
    message(STATUS "Doxygen encontrado. Configurando la documentación...")
    
    # 5. Define la plantilla y la ruta de salida del Doxyfile generado
    set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
    set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    # 6. Genera la configuración final (Doxyfile) reemplazando variables de CMake
    configure_file(
        ${DOXYFILE_IN}
        ${DOXYFILE_OUT}
        @ONLY
    )

    # 7. Crea el target 'doc' para generar la documentación
    # CORRECCIÓN: Se debe ejecutar doxygen con el Doxyfile configurado
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generando documentación con Doxygen"
        VERBATIM
    )
    
    message(STATUS "Target 'doc' creado. Usa 'make doc' o 'cmake --build . --target doc' para generar la documentación")
else()
    message(WARNING "Doxygen no encontrado. La documentación no se generará.")
endif()