# Nome do executavel
TARGET = patience_game

# Diretorios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Compilador e flags
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -Werror

# Regra padrao
all: $(BUILD_DIR) $(TARGET)

# Regra para criar o diretorio de build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Regra para compilar o executavel
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Regra para compilar os arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos de build
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Limpar tudo, incluindo dependencias
distclean: clean

.PHONY: all clean distclean
