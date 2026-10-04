#
# RULES
#
include = include

main : main.o
	g++ -g -O0 main.o -o main

main.o : main.cpp
	g++ -g -O0 -I./$(include) -c main.cpp

# 4.6 Phony targets
.PHONY : clean

clean : 
	rm main.o main
