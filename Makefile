.SUFFIXES:
.SUFFIXES: .o .cpp
#============================================================
TARGET	=  shortest

C_SOURCES =  Shortest_paths.cpp
C_OBJS     =  Shortest_paths.o
MY_INCLUDES =
# merg.h


CCX = g++
CXXFLAGS = -g -std=c++11
# -Wall

#============================================================
all: $(TARGET)

Shortest_paths.o: Shortest_paths.cpp
	$(CCX)  -c $(CXXFLAGS) $<

$(TARGET) :   $(C_OBJS)
	$(CCX) $(CXXFLAGS)  $^ $(LIBDIRS)  -o $@

# Implicit rules: $@ = target name, $< = first prerequisite name, $^ = name of all prerequisites
#============================================================

ALL_SOURCES = Makefile $(C_SOURCES) $(MY_INCLUDES)

NOTES =
%= otherstuff.np

clean:
	rm -f $(TARGET) $(C_OBJS) core
