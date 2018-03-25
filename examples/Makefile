CXX = clang++-5.0
CXXFLAGS = -std=c++14 -I ../include

EXES = with_log without_log simple_accum

all : $(EXES)

with_log: logtofile.cpp
	$(CXX) $(CXXFLAGS) -DBENCH=true -o with_log logtofile.cpp

without_log: logtofile.cpp
	$(CXX) $(CXXFLAGS) -DBENCH=false -o without_log logtofile.cpp

simple_accum: simple_accum.cpp
	$(CXX) $(CXXFLAGS) -DBENCH=true -o simple_accum simple_accum.cpp

PHONY: all clean new

clean:
	rm -rf *.o $(EXES)

new:
	make clean
	make all
