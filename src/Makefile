#Author:  Tomáš Bydžovský
#		  bydzoto1@fit.cvut.cz


CXX       = g++
LD        = g++
CXXFLAGS  = -Wall -Wno-long-long -O0 -ggdb -Wextra -pedantic -std=c++11
LIBS      = -lncurses

TARGET	  = chip8
BINARIES  = bin
SOURCES   = src
DOC  	  = doc
DOXYFILE  = Doxyfile

OBJECTS   = $(BINARIES)/Chip8.o $(BINARIES)/Emulator.o $(BINARIES)/main.o $(BINARIES)/UserInterface.o

#compile program and create ducumentation 
all: compile doc

#compile
compile: $(TARGET)


run: $(TARGET)
	./$(TARGET)

clean:
	rm -Rf $(DOC) $(BINARIES) $(TARGET) *~ core

#link objects:
$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LIBS)

#compile objects:
$(BINARIES)/%.o: $(SOURCES)/%.cpp
	mkdir -p $(BINARIES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


#dependencies:
$(BINARIES)/Chip8.o: $(SOURCES)/Chip8.cpp $(SOURCES)/Chip8.hpp $(SOURCES)/UserInterface.cpp $(SOURCES)/UserInterface.hpp
$(BINARIES)/Emulator.o: $(SOURCES)/Emulator.cpp $(SOURCES)/Emulator.hpp $(SOURCES)/Chip8.cpp $(SOURCES)/Chip8.hpp $(SOURCES)/UserInterface.cpp $(SOURCES)/UserInterface.hpp
$(BINARIES)/main.o: $(SOURCES)/main.cpp $(SOURCES)/Emulator.hpp $(SOURCES)/Chip8.cpp $(SOURCES)/Chip8.hpp $(SOURCES)/UserInterface.cpp $(SOURCES)/UserInterface.hpp
$(BINARIES)/UserInterface.o: $(SOURCES)/UserInterface.cpp $(SOURCES)/UserInterface.hpp