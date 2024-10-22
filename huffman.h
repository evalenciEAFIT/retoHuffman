#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <string>
#include <vector>

// Clase NodoHuffman: representa un nodo del árbol de Huffman.
class NodoHuffman {
public:
    char caracter;            // El carácter almacenado en el nodo.
    int frecuencia;           // La frecuencia de aparición del carácter.
    NodoHuffman* izquierda;   // Puntero al hijo izquierdo.
    NodoHuffman* derecha;     // Puntero al hijo derecho.

    NodoHuffman(char c, int f);  // Constructor del nodo.
};

// Clase ArbolHuffman: contiene la lógica para construir el árbol de Huffman y generar los códigos.
class ArbolHuffman {
public:
    NodoHuffman* raiz;  // Raíz del árbol de Huffman.
    std::unordered_map<char, std::string> codigosHuffman;  // Mapa de códigos binarios de Huffman por carácter.

    void construirArbol(const std::unordered_map<char, int>& frecuencias);  // Construye el árbol de Huffman.
    void generarCodigos(NodoHuffman* nodo, std::string codigoActual);      // Genera los códigos binarios de Huffman.
    ~ArbolHuffman();  // Destructor para liberar la memoria del árbol.

private:
    void destruir(NodoHuffman* nodo);  // Función auxiliar para destruir los nodos recursivamente.
};

// Clase CompresorHuffman: maneja el proceso de compresión de archivos utilizando el árbol de Huffman.
class CompresorHuffman {
public:
    void comprimirArchivo(const std::string& archivoEntrada, const std::string& archivoSalida);  // Comprime un archivo.

private:
    ArbolHuffman arbol;  // El árbol de Huffman utilizado para la compresión.
    std::unordered_map<char, int> calcularFrecuencias(const std::string& archivo);  // Calcula las frecuencias de los caracteres.
    void guardarArchivoBinario(const std::string& archivoSalida, const std::string& textoCodificado);  // Guarda el archivo comprimido en binario.
};

// Clase DescompresorHuffman: maneja el proceso de descompresión de archivos usando los códigos de Huffman.
class DescompresorHuffman {
public:
    void descomprimirArchivo(const std::string& archivoEntrada, const std::string& archivoSalida);  // Descomprime un archivo.

private:
    ArbolHuffman arbol;  // Árbol de Huffman reconstruido a partir del archivo comprimido.
    std::string leerArchivoBinario(const std::string& archivoEntrada);  // Lee el archivo comprimido en binario.
};

#endif
