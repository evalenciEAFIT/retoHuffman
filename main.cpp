#include "huffman.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Explicación de las opciones de CLI
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " -c|-d <archivo_entrada> -o <archivo_salida>\n";
        std::cerr << "-c: Comprimir el archivo de entrada y guardar el resultado comprimido en el archivo de salida.\n";
        std::cerr << "-d: Descomprimir el archivo de entrada comprimido y restaurar el archivo original en el archivo de salida.\n";
        std::cerr << "-o: Especifica el nombre del archivo de salida (tanto para compresión como para descompresión).\n";
        return 1;
    }

    // Modo de operación: -c para comprimir, -d para descomprimir
    std::string modo = argv[1];
    std::string archivoEntrada = argv[2];
    std::string archivoSalida = argv[4];

    // Comprimir el archivo
    if (modo == "-c") {
        CompresorHuffman compresor;
        compresor.comprimirArchivo(archivoEntrada, archivoSalida);
        std::cout << "Archivo comprimido con éxito en: " << archivoSalida << "\n";
    }
    // Descomprimir el archivo
    else if (modo == "-d") {
        DescompresorHuffman descompresor;
        descompresor.descomprimirArchivo(archivoEntrada, archivoSalida);
        std::cout << "Archivo descomprimido con éxito en: " << archivoSalida << "\n";
    }
    // Si no se reconoce el modo, se muestra un error
    else {
        std::cerr << "Modo desconocido. Use -c para comprimir y -d para descomprimir.\n";
    }

    return 0;
}
