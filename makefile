file := epoll_server.cpp inet.h

epoll_server:epoll_server.o 
	gcc epoll_server.o -o epoll_server $(CXXFLAGE)

epoll_server.o:$(file)
	gcc -c epoll_server.cpp -I ./ $(CXXFLAGE)

.PHONY:clear
clear:
	-rm *.o
