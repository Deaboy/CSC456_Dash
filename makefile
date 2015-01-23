
CC = gcc
CPP = g++
LIBS = 
FLAGS = -g -Wall -std=c++11
EXECS = dash

all : $(EXECS)

dash: dash.o
	$(CPP) $(FLAGS) -o $@ $? $(LIBS)

