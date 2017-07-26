CXX       = gcc
CXXFLAGS  = -Wall -Werror -I. -Winline -O3 -I../include/
LDFLAGS =  
LIBNAME       = pad
LIBPATH     = ../lib
SRCS      = pad_time.o
OBJS      = $(SRCS:.c=.o)

.SUFFIXES: .c .o

.c.o: 
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: ar

ar: $(OBJS)
	ar rcs lib$(LIBNAME).a $(SRCS)
	[ -d $(LIBPATH) ] || mkdir -p $(LIBPATH)
	mv lib$(LIBNAME).a $(LIBPATH)
clean:
	/bin/rm -f $(OBJS) ../lib/lib$(LIBNAME).a *~ ../include/*~
