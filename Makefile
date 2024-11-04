CXX := g++

CXX_FLAGS := -O3 -std=c++17
LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

testsAuto : ./bin/RayCasting.o ./bin/testsAuto.o
	$(CXX) -o ./bin/testsAuto.exe ./bin/testsAuto.o ./bin/RayCasting.o
	./bin/testsAuto.exe

./bin/testsAuto.o : ./src/testsAuto.cpp
	$(CXX) -c ./src/testsAuto.cpp -o ./bin/testsAuto.o

./bin/RayCasting.o : ./src/RayCasting.cpp ./src/RayCasting.h
	$(CXX) -c ./src/RayCasting.cpp -o ./bin/RayCasting.o

testsVisual : ./bin/RayCasting.o ./bin/testsVisual.o
	$(CXX) -o ./bin/testsVisual.exe ./bin/RayCasting.o ./bin/testsVisual.o $(LDFLAGS)
	./bin/testsVisual.exe

./bin/testsVisual.o : ./src/testsVisual.cpp
	$(CXX) -c ./src/testsVisual.cpp -o ./bin/testsVisual.o

clean :
	rm -f ./bin/*
