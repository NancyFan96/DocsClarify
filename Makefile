CC = g++
PROC = classify

SRC=	count.cpp main.cpp
HDR=	count.hpp


all: 	$(PROC)

classify: 	$(SRC) $(HDR)
	$(CC) -o $(PROC) count.cpp main.cpp -Wall


clean:
	-rm $(PROC) *.o

