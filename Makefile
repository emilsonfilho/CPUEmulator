# Configuração de compilação
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDETAGS = -Iinclude

# Diretórios
SRC_DIR = src
BIN_DIR = bin
PROGRAMS_DIR = Programs

# Busca recursiva por arquivos fonte
SRCS = Main.cpp $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:%.cpp=$(BIN_DIR)/%.o)

# Nome do executável
TARGET = app

# Regra padrão
all: $(TARGET)
	@./$(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	@echo "Linkando $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(INCLUDETAGS) $^ -o $@

# Compilação de cada arquivo fonte
$(BIN_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDETAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	@echo "Limpando arquivos..."
	rm -rf $(BIN_DIR) $(TARGET)
