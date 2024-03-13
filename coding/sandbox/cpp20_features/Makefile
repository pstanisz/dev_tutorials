# Copyright (c) 2022, Piotr Staniszewski

CXX = g++
CXXFLAGS = -std=c++20 -fmodules-ts -g -Wall -Werror -Wextra -pedantic
TARGET = cpp20_features

all: $(TARGET)

$(TARGET): $(TARGET).o my_module.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) my_module.o $(TARGET).o

my_module.o: my_module.cpp
	$(CXX) $(CXXFLAGS) -c my_module.cpp

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp

clean:
	$(RM) $(TARGET) $(TARGET).o my_module.o
