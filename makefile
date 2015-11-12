CC=g++
CFLAGS = -Wall -c
LDFLAGS = -lGL -lGLU -lglut -lm
SRCS = Game.cpp Robot.cpp QuadMesh.cpp Room.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = Game

all: $(SRCS) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm $(OBJS) $(EXEC) *~ *#
