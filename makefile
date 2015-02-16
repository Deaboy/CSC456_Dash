
CC = gcc
CPP = g++
LIBS = 
FLAGS = -g -Wall -std=c++11
EXECS = dash

all : $(EXECS)

dash: dash.o commands.o commands2.o
	$(CPP) $(FLAGS) -o $@ $? $(LIBS)

