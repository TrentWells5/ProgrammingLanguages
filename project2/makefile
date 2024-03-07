CXX = g++
CXX_FLAGS = -g -Wall

main: main.o scanner.o parser.o
	$(CXX) $(CXX_FLAGS) -o $@ $^
%.o:%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
clean:
	rm -rf *.o main *.rpn