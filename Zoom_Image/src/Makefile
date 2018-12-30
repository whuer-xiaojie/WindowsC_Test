CC              = gcc
RM              = rm -f

CFLAGS  = -O2 -Wall
LDFLAGS = -s
LIBS    =-lSDL2 -lGLESv2
OBJECTS =graphics.o zoom.o

PROGRAM = zoom-image
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(PROGRAM): $(OBJECTS)
	-$(RM) $@
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

clean:
	-$(RM) $(OBJECTS) $(PROGRA)