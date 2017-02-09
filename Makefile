
BIN = bin

CFLAGS = -Wall -Isrc/ -std=c++11 -lstdc++

all: test

test: $(BIN)/sample_tests
	./$(BIN)/sample_tests

$(BIN)/sample_tests:  test/sample_tests.cpp test/other_tests.cpp
	$(CXX) $(CFLAGS) test/sample_tests.cpp test/other_tests.cpp -o $(BIN)/sample_tests