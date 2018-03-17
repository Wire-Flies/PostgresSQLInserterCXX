CC = g++
FLAGS = -std=c++17 -Wall -O3
DEPS = -lstdc++fs -lpqxx -lpq -lzip -lpthread
# -lzlib
main.out: main.cpp fileHelpers.cpp pgHelpers.cpp zipHelpers.cpp
	$(CC) $(FLAGS) -o a.out main.cpp fileHelpers.cpp pgHelpers.cpp zipHelpers.cpp $(DEPS)