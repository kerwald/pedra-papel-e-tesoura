# --- CONFIGURAÇÕES ---
CXX = g++
# Adicionada a flag -pthread para suporte a threads
CXXFLAGS = -std=c++17 -Wall -g -Iinclude -pthread
EXECUTABLE = programa.exe

# --- CAMINHO DE BUSCA PARA FONTES ---
VPATH = src

# --- DETECÇÃO AUTOMÁTICA DE ARQUIVOS ---
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(notdir $(SOURCES:.cpp=.o))

# --- REGRAS DO MAKEFILE ---

all: $(EXECUTABLE)

# Na linkagem também é necessário o -pthread
$(EXECUTABLE): $(OBJECTS)
	@echo ">> Juntando os arquivos objeto para criar o executavel..."
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) -pthread
	@echo ">> Sucesso! Executavel '$(EXECUTABLE)' criado."

%.o: %.cpp
	@echo ">> Compilando '$<' para '$@'..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	@echo ">> Executando o programa..."
	./$(EXECUTABLE)

clean:
	@echo ">> Limpando arquivos gerados..."
	-del $(subst /,\,$(OBJECTS)) $(subst /,\,$(EXECUTABLE)) 2>nul

.PHONY: all run clean