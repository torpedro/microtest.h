
# Directories
SRC     = src
BIN     = bin

# Compile Flags.
CFLAGS  = -Wall -Isrc/ -std=c++11 -lstdc++

# Installation
INSTALL = /usr/local

all: test

clean:
	rm -f ./$(BIN)/*

test: $(BIN)/sample_tests $(BIN)/argument_tests
	bash test/test.sh

install:
	cp -r $(SRC)/* $(INSTALL)/include/

$(BIN)/sample_tests:  examples/sample_tests.cpp examples/sample_tests2.cpp
	@mkdir -p $(BIN)/
	$(CXX) $(CFLAGS) examples/sample_tests.cpp examples/sample_tests2.cpp -o $(BIN)/sample_tests

$(BIN)/argument_tests:  examples/argument_tests.cpp
	@mkdir -p $(BIN)/
	$(CXX) $(CFLAGS) examples/argument_tests.cpp -o $(BIN)/argument_tests
