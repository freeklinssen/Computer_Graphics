UNAME := $(shell uname)

CC = arch -x86_64 g++

ifeq ($(UNAME),Darwin)
INCFLAGS = -I vecmath/include/

FRAMEWORKS = -framework Glut
FRAMEWORKS += -framework OpenGL

LINKFLAGS += -L vecmath/lib/ -lvecmath
else
INCFLAGS = -I /usr/include/GL
INCFLAGS += -I /mit/6.837/public/include/vecmath

LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -L /mit/6.837/public/lib -libvecmath
endif

CFLAGS    = -O2 -Wall -ansi -DSOLN
SRCS      = main.cpp parse.cpp curve.cpp surf.cpp camera.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a1

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(ARCH_FLAGS) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(FRAMEWORKS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)
