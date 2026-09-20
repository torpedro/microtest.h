.PHONY: all configure test clean install
CMAKE ?= cmake
PREFIX ?= /usr/local

all: test

configure:
	$(CMAKE) --preset dev

test: configure
	$(CMAKE) --build --preset dev
	ctest --preset dev

clean:
	$(CMAKE) --build build/dev --target clean

install: configure
	$(CMAKE) --install build/dev --prefix "$(PREFIX)"
