# Copyright (c) 2022, Piotr Staniszewski

CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Werror -Wextra -pedantic
TARGET = cpp17_features

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
