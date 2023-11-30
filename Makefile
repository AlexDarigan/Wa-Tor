# Author: Joseph Kehoe
# Modified: David Darigan

CXX = g++
CPPFILES= wator.cpp
CPPFLAGS= -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lgomp -fopenmp
EXE= watorApp

SRCS=wator.cpp Semaphore.cpp Semaphore.h
OBJS=$(subst .cpp,.o,$(SRCS))



ALL: $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)