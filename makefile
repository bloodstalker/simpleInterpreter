TARGET=qlabInterpreter
CXX=clang++
CXX?=clang++
CXX_FLAGS=-std=c++11
CXX_EXTRA?=
CXX_FLAGS+=$(CXX_EXTRA)

.DEFAULT:all clean

.PHONY:all clean $(TARGET) help

all:$(TARGET)

.cpp.o:
	$(CXX) $(CXX_FLAGS) -c $< -o $@ 

$(TARGET): qlabInterpreter.o
	$(CXX) $^ $(LD_FLAGS) -o $@

clean:
	rm -f *.o *~ $(TARGET)

help:
	@echo "the makfile for a very simple stack-machine interpreter."
	@echo "there is the taget itself, all and clean."
