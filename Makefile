
BIN = bin

CFLAGS = -Wall -Isrc/ -std=c++11 -lstdc++

all: examples

clean:
	rm -f ./$(BIN)/*

test: examples
	bash ./test/test.sh

examples: $(BIN)/sample_tests $(BIN)/argument_tests


$(BIN)/sample_tests:  test/sample_tests.cpp test/sample_tests2.cpp
	@mkdir -p $(BIN)/
	$(CXX) $(CFLAGS) test/sample_tests.cpp test/sample_tests2.cpp -o $(BIN)/sample_tests

$(BIN)/argument_tests:  test/argument_tests.cpp
	@mkdir -p $(BIN)/
	$(CXX) $(CFLAGS) test/argument_tests.cpp -o $(BIN)/argument_tests
