.PHONY: build
build:
	@g++ main.cpp -lncurses -std=c++20

.PHONY: run
run: build
	@./a.out
