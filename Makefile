# --- CONFIGURAÇÕES ---
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
EXECUTABLE = programa.exe

# --- CAMINHO DE BUSCA PARA FONTES ---
# VPATH: Diz ao 'make' para procurar em 'src' por qualquer arquivo que ele não encontre.
VPATH = src

# --- DETECÇÃO AUTOMÁTICA DE ARQUIVOS ---
# 1. Encontra todos os arquivos .cpp na pasta src
SOURCES = $(wildcard src/*.cpp)
# 2. Gera uma lista de alvos .o sem o prefixo do diretório
OBJECTS = $(notdir $(SOURCES:.cpp=.o))


# --- REGRAS DO MAKEFILE ---

# Regra padrão: compila o executável.
all: $(EXECUTABLE)

# Regra de linkagem: junta os arquivos .o para criar o .exe
$(EXECUTABLE): $(OBJECTS)
	@echo ">> Juntando os arquivos objeto para criar o executavel..."
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)
	@echo ">> Sucesso! Executavel '$(EXECUTABLE)' criado."

# Regra de compilação: transforma um .cpp em um .o.
# O VPATH encontrará o arquivo .cpp correspondente na pasta 'src'.
# A variável $< será preenchida com o caminho completo (ex: "src/Conta.cpp").
%.o: %.cpp
	@echo ">> Compilando '$<' para '$@'..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para executar o programa.
run: all
	@echo ">> Executando o programa..."
	.\$(EXECUTABLE)

# Regra para limpar os arquivos gerados.
clean:
	@echo ">> Limpando arquivos gerados..."
	-del $(subst /,\,$(OBJECTS)) $(subst /,\,$(EXECUTABLE)) 2>nul

.PHONY: all run clean