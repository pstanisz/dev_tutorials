# Copyright (c) 2022, Piotr Staniszewski

CC = g++
CFLAGS = -g -Wall -Werror -Wextra -pedantic
TARGET = is_same

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
