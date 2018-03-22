CXX = clang++5.0
CXXFLAGS = -std=c++14 -I ../include

all: with_log without_log

with_log: logtofile.cpp
	$(CXX) $(CXXFLAGS) -DBENCH=true -o with_log logtofile.cpp

without_log: logtofile.cpp
	$(CXX) $(CXXFLAGS) -DBENCH=false -o without_log logtofile.cpp


PHONY: all clean new

clean:
	rm -rf *.o with_log without_log
