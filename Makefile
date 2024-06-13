CXX=clang++
CXXFLAGS=-std=c++17 -Werror -Wsign-conversion
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --error-exitcode=99

SOURCES=Catan.cpp \
        Board.cpp \
        Player.cpp \
        DevelopmentCard.cpp \
        CardDeck.cpp \
        DevelopmentCardManagement.cpp \
        ResourceManagement.cpp \
        RoadAndSettlementManagement.cpp \
        Vertex.cpp \
        Edge.cpp \
        Tile.cpp

TESTSOURCES=TestCounter.cpp Test.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TESTOBJECTS=$(TESTSOURCES:.cpp=.o)

HEADERS=Catan_API.hpp

rundemo: main
	./main

runtests: test
	./test

main: main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TESTOBJECTS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

tidy:
	clang-tidy $(SOURCES) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

demovalgrind: main
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./main 2>&1 | { egrep "lost| at " || true; }

testvalgrind: test
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test 2>&1 | { egrep "lost| at " || true; }

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o main test
