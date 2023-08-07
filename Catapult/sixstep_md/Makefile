CPP=$(wildcard src/*.cpp)
OBJ=$(CPP:.cpp=.o)
DEPS=$(wildcard include/*.h)

NTT: $(OBJ) $
	$(CXX) -std=c++11 -o $@ $^ -Iinclude

%.o: %.cpp $(DEPS)
	$(CXX) -std=c++11 -c -o $@ $< -Iinclude

clean:
	rm -rfv NTT $(OBJ)
