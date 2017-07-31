CXX       = g++
CXXFLAGS  =  -I. -I /usr/local/include -I /home/vitor/opt/opencv2.4.11/include -Wall -Winline #-O2
export PKG_CONFIG_PATH=/home/vitor/opt/opencv2.4.11/lib/pkgconfig
export LD_LIBRARY_PATH=/home/vitor/opt/opencv2.4.11/lib/:/usr/local/cuda-7.5/lib64
LDFLAGS = `pkg-config opencv --cflags --libs` -L  /home/vitor/opt/opencv2.4.11/lib -L /usr/local/cuda-7.5/lib64/
PROGRAM   = app
SRCS      = main.o
OBJS      = $(SRCS:.c=.o)
PKG_CONFIG_PATH=/home/vitor/opt/opencv2.4.11/lib/pkgconfig

.SUFFIXES: .c .o

.c.o: 
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(PROGRAM) 

run: all
	./$(PROGRAM)

debug: all
	gdb $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(SRCS) $(CXXFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~
