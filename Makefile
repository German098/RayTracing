#
# RULES
#
include = include

main : main.o
	g++ main.o -o main

main.o : main.cpp
	g++ -I./$(include) -c main.cpp

# 4.6 Phony targets
.PHONY : clean

clean : 
	rm main.o main
