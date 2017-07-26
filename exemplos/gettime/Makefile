CXX       = gcc
CXXFLAGS  = -Wall -I. -Winline -fopenmp -I../../include/ # -O3
LDFLAGS =  ../../lib/libpad.a

PROGRAM   = app
SRCS      = main.o
OBJS      = $(SRCS:.c=.o)

.SUFFIXES: .c .o

.c.o: 
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(PROGRAM) 

run: all
	./$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(SRCS) $(CXXFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~
