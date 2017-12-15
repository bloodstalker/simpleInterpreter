TARGET=qlabInterpreter
CXX=clang++
CXX?=clang++
# you should change the include path to one that is right for you
CXX_FLAGS=-I/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include -std=c++11

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
	@echo "the makfile for the bruiser test file."
	@echo "there is the taget itself, all and clean."

