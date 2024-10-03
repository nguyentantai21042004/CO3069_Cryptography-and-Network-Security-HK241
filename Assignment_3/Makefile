# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

# Libraries
LIBS = -lgmp -lgmpxx

# Directories
SRCDIR = src
INCLUDEDIR = include
BINDIR = bin

# Source files and object files
SOURCES = main.cpp $(SRCDIR)/GCD.cpp $(SRCDIR)/PrimeGenerator.cpp $(SRCDIR)/RSA.cpp $(SRCDIR)/Utility.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

# Default target: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $(BINDIR)/$(TARGET)

# Rule to compile each source file into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
	rm -rf $(BINDIR)/*

# Phony targets
.PHONY: all clean