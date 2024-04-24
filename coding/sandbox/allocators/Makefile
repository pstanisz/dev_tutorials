# Copyright (c) 2024, Piotr Staniszewski

CC = g++
CFLAGS = -g -Wall -Werror -Wextra -pedantic -std=c++20
TARGET = allocators

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
