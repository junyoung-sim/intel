COMPILER=g++
VERSION=-std=c++2a

output: main.o linear.o dnn.o  data.o
	$(COMPILER) $(VERSION) main.o linear.o dnn.o data.o -o run
	rm *.o

main.o: ./src/main.cpp
	$(COMPILER) $(VERSION) -c ./src/main.cpp

linear.o: ./src/linear.cpp
	$(COMPILER) $(VERSION) -c ./src/linear.cpp

dnn.o: ./src/dnn.cpp
	$(COMPILER) $(VERSION) -c ./src/dnn.cpp

data.o: ./src/data.cpp
	$(COMPILER) $(VERSION) -c ./src/data.cpp
