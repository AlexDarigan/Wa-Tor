# Author: Joseph Kehoe
# Modified: David Darigan

# Using the g++ compiler
CXX = g++

# Enabling all errors
CPPFLAGS= -Wall

# Linking sfml graphics library, sfml window & sfml system for graphics, linking openmp (lgomp & fopenmp) for concurrency
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lgomp -fopenmp

# Our resulting executable's name
EXE= watorApp

# Detailing our sources, and using them to detail our Objects
SRCS=wator.cpp Semaphore.cpp Semaphore.h
OBJS=$(subst .cpp,.o,$(SRCS))

# Our standard build rule bringing all our variable variables together
ALL: $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)