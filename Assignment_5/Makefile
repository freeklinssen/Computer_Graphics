
CC = arch -x86_64 g++

INCFLAGS = -I vecmath/include/

FRAMEWORKS = -framework Glut
FRAMEWORKS += -framework OpenGL

LINKFLAGS += -L vecmath/lib/ -lvecmath


CFLAGS    = -g -O2 -Wall -Wextra -ansi 
CFLAGS    += -DSOLN
SRCS      = $(wildcard *.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a4

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(ARCH_FLAGS) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(FRAMEWORKS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

#################################### Original make file for Linux
#SRCS = $(wildcard *.cpp)
#SRCS += $(wildcard vecmath/src/*.cpp)

#OBJS = $(SRCS:.cpp=.o)
#PROG = a4
#CFLAGS = -O2 -Wall -Wextra
#INCFLAGS = -Ivecmath/include

#all: $(PROG)

#$(PROG): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

#.cpp.o:
#	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

# maybe needed to use the *.bak from this line but not sure
#clean:
#	rm -f *.bak vecmath/src/*.o *.o core.* $(PROG) 
