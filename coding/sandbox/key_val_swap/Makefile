# Copyright (c) 2022, Piotr Staniszewski

CC = g++
CFLAGS = -g -Wall -Werror -Wextra -pedantic
TARGET = swapper

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
