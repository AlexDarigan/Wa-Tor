# Author: David Darigan

CXX = g++ -fopenmp
CPPFILES= wator.cpp
CPPFLAGS= -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lgomp -fopenmp
EXE= watorApp

SRCS=wator.cpp Semaphore.cpp Semaphore.h
OBJS=$(subst .cpp,.o,$(SRCS))

#This rule says that each .o file depends on a .cpp file of the same name
#This is actually built into Make but anyways...
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CPPFLAGS)  $(DEBUGFLAGS)


ALL: $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)
