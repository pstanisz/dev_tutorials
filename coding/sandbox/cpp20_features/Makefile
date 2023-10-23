# Copyright (c) 2022, Piotr Staniszewski

CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -Werror -Wextra -pedantic
TARGET = cpp20_features

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
