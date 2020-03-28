filename:=thread_server
file := $(filename).cpp inet.h

$(filename):$(filename).o 
	gcc $(filename).o -o $(filename) -pthread $(CXXFLAGE)

$(filename).o:$(file)
	gcc -c $(filename).cpp -I ./ -pthread $(CXXFLAGE)

.PHONY:clear
clear:
	-rm *.o
