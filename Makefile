CXX := g++

CXX_FLAGS := -O3 -std=c++17
LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Build is in debug mode (-g)
# Remove -g, make clean, and build to build non-debug mode build

testsAuto : ./bin/RayCasting.o ./bin/testsAuto.o
	$(CXX) -g -o ./bin/testsAuto.exe ./bin/testsAuto.o ./bin/RayCasting.o
	./bin/testsAuto.exe

./bin/testsAuto.o : ./src/testsAuto.cpp
	$(CXX) -g -c ./src/testsAuto.cpp -o ./bin/testsAuto.o

./bin/RayCasting.o : ./src/RayCasting.cpp ./src/RayCasting.h
	$(CXX) -g -c ./src/RayCasting.cpp -o ./bin/RayCasting.o

testsVisual : ./bin/RayCasting.o ./bin/testsVisual.o
	$(CXX) -g -o ./bin/testsVisual.exe ./bin/RayCasting.o ./bin/testsVisual.o $(LDFLAGS)
	./bin/testsVisual.exe

./bin/testsVisual.o : ./src/testsVisual.cpp
	$(CXX) -g -c ./src/testsVisual.cpp -o ./bin/testsVisual.o

testsMap : ./bin/Map.o ./bin/testsMap.o ./bin/RayCasting.o
	$(CXX) -g -o ./bin/testsMap.exe ./bin/Map.o ./bin/testsMap.o ./bin/RayCasting.o 
	./bin/testsMap.exe

./bin/Map.o : ./src/Map.h ./src/Map.cpp
	$(CXX) -g -c ./src/Map.cpp -o ./bin/Map.o 

./bin/testsMap.o : ./src/testsMap.cpp
	$(CXX) -g -c ./src/testsMap.cpp -o ./bin/testsMap.o 

clean :
	rm -f ./bin/*
