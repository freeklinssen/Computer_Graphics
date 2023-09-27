
CC        = arch -x86_64 g++

INCFLAGS = -I vecmath/include/

FRAMEWORKS = -framework Glut
FRAMEWORKS += -framework OpenGL

LINKFLAGS += -L vecmath/lib/ -lvecmath
LINKFLAGS += -L. -lRK4

CFLAGS    = -g -O2 -Wall -ansi 
CFLAGS    += -DSOLN
SRCS      = $(wildcard *.cpp)
#SRCS      = bitmap.cpp camera.cpp MatrixStack.cpp modelerapp.cpp modelerui.cpp ModelerView.cpp Joint.cpp SkeletalModel.cpp Mesh.cpp main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a3

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(ARCH_FLAGS) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(FRAMEWORKS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)


########################################################### original

#INCFLAGS  = -I vecmath/include
#INCFLAGS += -I /usr/include/GL

#LINKFLAGS = -L. -lRK4 -lglut -lGL -lGLU
#CFLAGS    = -g -Wall -ansi
#CC        = g++
#SRCS      = $(wildcard *.cpp)
#SRCS     += $(wildcard vecmath/src/*.cpp)
#OBJS      = $(SRCS:.cpp=.o)
#PROG      = a3

#all: $(SRCS) $(PROG)

#$(PROG): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

#.cpp.o:
#	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

#depend:
#	makedepend $(INCFLAGS) -Y $(SRCS)

#clean:
#	rm $(OBJS) $(PROG)
