TARGET = walors
FILE = $(shell find ./src -type f -name main.c)
DIR = ./build/
CC = gcc

all: $(TARGET)

$(TARGET): $(FILE)
	@if [ ! -d $(DIR) ]; then \
		mkdir $(DIR); \
	fi
	$(CC) -o $(DIR)$(TARGET) $(FILE)

run: all
	@$(DIR)$(TARGET)

clean: 
	rm -rf $(DIR)
