#include "huffman.h"
#include <queue>     // Para manejar la cola de prioridad (min-heap) en la construcción del árbol de Huffman.
#include <fstream>   // Para manejo de archivos (lectura y escritura).
#include <bitset>    // Para manipular los bits al guardar la tabla de códigos y el archivo comprimido.
#include <iostream>  // Para imprimir mensajes de depuración o errores.
#include <cstdint>  //  Para definir uint8_t y uint64_t


// Constructor de NodoHuffman: inicializa un nodo con un carácter y su frecuencia.
NodoHuffman::NodoHuffman(char c, int f) : caracter(c), frecuencia(f), izquierda(nullptr), derecha(nullptr) {}

// Destructor del árbol de Huffman: libera la memoria del árbol de manera recursiva.
ArbolHuffman::~ArbolHuffman() {
    destruir(raiz);  // Llama a la función recursiva para liberar cada nodo del árbol.
}

// Función recursiva para destruir los nodos del árbol y liberar la memoria.
void ArbolHuffman::destruir(NodoHuffman* nodo) {
    if (nodo) {
        destruir(nodo->izquierda);  // Libera los nodos del subárbol izquierdo.
        destruir(nodo->derecha);    // Libera los nodos del subárbol derecho.
        delete nodo;                // Elimina el nodo actual.
    }
}

// Construye el árbol de Huffman a partir de las frecuencias de los caracteres.
void ArbolHuffman::construirArbol(const std::unordered_map<char, int>& frecuencias) {
    // Usamos una cola de prioridad (min-heap) para ordenar los nodos según su frecuencia.
    auto comparar = [](NodoHuffman* a, NodoHuffman* b) { return a->frecuencia > b->frecuencia; };
    std::priority_queue<NodoHuffman*, std::vector<NodoHuffman*>, decltype(comparar)> colaPrioridad(comparar);

    // Paso 1: Crear un nodo para cada carácter y añadirlo a la cola de prioridad.
    for (auto& par : frecuencias) {
        colaPrioridad.push(new NodoHuffman(par.first, par.second));
    }

    // Paso 2: Combinar los dos nodos con menor frecuencia hasta que quede un solo nodo (la raíz).
    while (colaPrioridad.size() > 1) {
        NodoHuffman* izquierda = colaPrioridad.top(); colaPrioridad.pop();
        NodoHuffman* derecha = colaPrioridad.top(); colaPrioridad.pop();

        // Crear un nuevo nodo que combina las frecuencias de los dos nodos más pequeños.
        NodoHuffman* nuevoNodo = new NodoHuffman('\0', izquierda->frecuencia + derecha->frecuencia);
        nuevoNodo->izquierda = izquierda;
        nuevoNodo->derecha = derecha;

        // Añadir el nuevo nodo a la cola de prioridad.
        colaPrioridad.push(nuevoNodo);
    }

    // El nodo que queda en la cola es la raíz del árbol de Huffman.
    raiz = colaPrioridad.top();
}

// Recorre el árbol de Huffman y genera los códigos binarios para cada carácter.
void ArbolHuffman::generarCodigos(NodoHuffman* nodo, std::string codigoActual) {
    if (!nodo) return;  // Caso base: si el nodo es nulo, se detiene la recursión.

    // Si es un nodo hoja, hemos llegado a un carácter y almacenamos su código.
    if (!nodo->izquierda && !nodo->derecha) {
        codigosHuffman[nodo->caracter] = codigoActual;
    }

    // Recorre el subárbol izquierdo añadiendo '0' al código actual.
    generarCodigos(nodo->izquierda, codigoActual + "0");

    // Recorre el subárbol derecho añadiendo '1' al código actual.
    generarCodigos(nodo->derecha, codigoActual + "1");
}

// Comprime un archivo utilizando el algoritmo de Huffman.
void CompresorHuffman::comprimirArchivo(const std::string& archivoEntrada, const std::string& archivoSalida) {
    // Paso 1: Calcular las frecuencias de los caracteres en el archivo de entrada.
    std::unordered_map<char, int> frecuencias = calcularFrecuencias(archivoEntrada);

    // Paso 2: Construir el árbol de Huffman basado en las frecuencias.
    arbol.construirArbol(frecuencias);

    // Paso 3: Generar los códigos de Huffman para cada carácter.
    arbol.generarCodigos(arbol.raiz, "");

    // Paso 4: Leer el archivo de entrada y codificarlo utilizando los códigos generados.
    std::ifstream archivo(archivoEntrada);
    std::string textoCodificado;
    char c;
    while (archivo.get(c)) {
        textoCodificado += arbol.codigosHuffman[c];  // Codifica cada carácter usando su código de Huffman.
    }
    archivo.close();

    // Paso 5: Guardar el archivo comprimido en binario.
    guardarArchivoBinario(archivoSalida, textoCodificado);
}

// Calcula las frecuencias de los caracteres en un archivo.
std::unordered_map<char, int> CompresorHuffman::calcularFrecuencias(const std::string& archivo) {
    std::unordered_map<char, int> frecuencias;
    std::ifstream entrada(archivo);
    char c;
    while (entrada.get(c)) {
        frecuencias[c]++;  // Incrementa la frecuencia de cada carácter encontrado.
    }
    entrada.close();
    return frecuencias;
}

// Guarda el archivo comprimido en formato binario.
void CompresorHuffman::guardarArchivoBinario(const std::string& archivoSalida, const std::string& textoCodificado) {
    std::ofstream salida(archivoSalida, std::ios::binary);

    // Guardar la tabla de códigos de Huffman en binario.
    size_t numCodigos = arbol.codigosHuffman.size();
    salida.write(reinterpret_cast<const char*>(&numCodigos), sizeof(numCodigos));  // Guardar el número de códigos.

    for (const auto& par : arbol.codigosHuffman) {
        char caracter = par.first;
        std::string codigo = par.second;

        // Guardar el carácter.
        salida.write(&caracter, sizeof(caracter));

        // Guardar la longitud del código.
        uint8_t longitudCodigo = codigo.size();
        salida.write(reinterpret_cast<const char*>(&longitudCodigo), sizeof(longitudCodigo));

        // Guardar el código en binario.
        std::bitset<64> bits(codigo);  // Convierte el código en binario (hasta 64 bits).
        uint64_t codigoBinario = bits.to_ullong();  // Convertir los bits a un entero de 64 bits.
        salida.write(reinterpret_cast<const char*>(&codigoBinario), sizeof(codigoBinario));  // Guardar el código binario.
    }

    // Guardar el texto codificado en binario.
    for (size_t i = 0; i < textoCodificado.size(); i += 8) {
        std::bitset<8> byte(textoCodificado.substr(i, 8));  // Dividir el texto codificado en bytes.
        salida.put(static_cast<unsigned char>(byte.to_ulong()));  // Guardar cada byte.
    }

    salida.close();
}

// Descomprime un archivo comprimido utilizando la tabla de códigos de Huffman.
void DescompresorHuffman::descomprimirArchivo(const std::string& archivoEntrada, const std::string& archivoSalida) {
    // Leer el archivo comprimido y obtener la secuencia de bits codificada.
    std::string textoCodificado = leerArchivoBinario(archivoEntrada);

    // Inicializar el nodo actual como la raíz del árbol de Huffman.
    NodoHuffman* nodoActual = arbol.raiz;

    std::ofstream salida(archivoSalida);  // Archivo donde se guardará el archivo descomprimido.
    for (char bit : textoCodificado) {
        // Si el bit es '0', seguimos el camino izquierdo en el árbol de Huffman.
        if (bit == '0') {
            nodoActual = nodoActual->izquierda;
        } 
        // Si el bit es '1', seguimos el camino derecho.
        else {
            nodoActual = nodoActual->derecha;
        }

        // Si llegamos a un nodo hoja, hemos encontrado un carácter.
        if (!nodoActual->izquierda && !nodoActual->derecha) {
            salida.put(nodoActual->caracter);  // Escribir el carácter original en el archivo de salida.
            nodoActual = arbol.raiz;  // Reiniciar al nodo raíz para seguir decodificando.
        }
    }

    salida.close();  // Cerrar el archivo de salida.
}

// Lee el archivo comprimido en formato binario y reconstruye la tabla de Huffman.
std::string DescompresorHuffman::leerArchivoBinario(const std::string& archivoEntrada) {
    std::ifstream entrada(archivoEntrada, std::ios::binary);
    std::string textoCodificado;

    // Verificar que el archivo comprimido se puede abrir correctamente.
    if (!entrada.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo comprimido para lectura.\n";
        return "";
    }

    // Leer el número de códigos de Huffman.
    size_t numCodigos;
    entrada.read(reinterpret_cast<char*>(&numCodigos), sizeof(numCodigos));

    // Limpiar la tabla de códigos previa y leer los nuevos códigos.
    arbol.codigosHuffman.clear();
    for (size_t i = 0; i < numCodigos; ++i) {
        char caracter;
        uint8_t longitudCodigo;
        uint64_t codigoBinario;

        // Leer el carácter.
        entrada.read(&caracter, sizeof(caracter));

        // Leer la longitud del código.
        entrada.read(reinterpret_cast<char*>(&longitudCodigo), sizeof(longitudCodigo));

        // Leer el código binario.
        entrada.read(reinterpret_cast<char*>(&codigoBinario), sizeof(codigoBinario));

        // Convertir el código binario a una cadena de bits.
        std::bitset<64> bits(codigoBinario);
        std::string codigo = bits.to_string().substr(64 - longitudCodigo, longitudCodigo);

        // Guardar el código en la tabla.
        arbol.codigosHuffman[caracter] = codigo;
    }

    // Leer el texto codificado (en binario).
    unsigned char byte;
    while (entrada.read(reinterpret_cast<char*>(&byte), 1)) {
        std::bitset<8> bits(byte);  // Convertir el byte a un conjunto de 8 bits.
        textoCodificado += bits.to_string();  // Añadir los bits al texto codificado.
    }

    entrada.close();  // Cerrar el archivo de entrada.
    return textoCodificado;  // Retorna la secuencia completa de bits.
}
