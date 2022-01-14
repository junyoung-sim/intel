COMPILER=g++
VERSION=-std=c++2a

output: main.o linear.o dnn.o signal.o util.o
	$(COMPILER) $(VERSION) main.o linear.o dnn.o signal.o util.o -o run
	rm *.o

main.o: ./src/main.cpp
	$(COMPILER) $(VERSION) -c ./src/main.cpp

linear.o: ./src/linear.cpp
	$(COMPILER) $(VERSION) -c ./src/linear.cpp

dnn.o: ./src/dnn.cpp
	$(COMPILER) $(VERSION) -c ./src/dnn.cpp

signal.o: ./src/signal.cpp
	$(COMPILER) $(VERSION) -c ./src/signal.cpp

util.o: ./src/util.cpp
	$(COMPILER) $(VERSION) -c ./src/util.cpp
