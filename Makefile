CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: xHuffman

xHuffman: main.o huffman.o
	$(CXX) $(CXXFLAGS) -o xHuffman main.o huffman.o

main.o: main.cpp huffman.h
	$(CXX) $(CXXFLAGS) -c main.cpp

huffman.o: huffman.cpp huffman.h
	$(CXX) $(CXXFLAGS) -c huffman.cpp

run_comprimir: xHuffman
	@read -p "Introduce el nombre del archivo de entrada (para comprimir): " archivo_entrada; \
	read -p "Introduce el nombre del archivo de salida (comprimido): " archivo_salida; \
	./xHuffman -c $$archivo_entrada -o $$archivo_salida

run_descomprimir: xHuffman
	@read -p "Introduce el nombre del archivo comprimido (para descomprimir): " archivo_entrada; \
	read -p "Introduce el nombre del archivo de salida (descomprimido): " archivo_salida; \
	./xHuffman -d $$archivo_entrada -o $$archivo_salida

clean:
	rm -f *.o xHuffman
