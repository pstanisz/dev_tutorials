# Copyright (c) 2023, Piotr Staniszewski

CC = g++
CFLAGS = -std=c++20 -g -Wall -Werror -Wextra -pedantic
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
