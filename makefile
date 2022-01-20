main: main.o BMgr.o DSMgr.o
	g++ -o main main.o BMgr.o DSMgr.o
main.o: main.cpp BMgr.h
	g++ -c main.cpp
BMgr.o: BMgr.cpp BMgr.h
	g++ -c BMgr.cpp
DSMgr.o: DSMgr.cpp DSMgr.h
	g++ -c DSMgr.cpp
clean:
	rm *.o main