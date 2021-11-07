CC = gcc
CFLAGS =
LDFLAGS =
DEPS =
OBJ = main.o

sdl_cflags := $(shell pkg-config --cflags gtk+-3.0)
sdl_libs := $(shell pkg-config --libs gtk+-3.0)
override CFLAGS += $(sdl_cflags)
override LIBS += $(sdl_libs)

%.o: %.cpp $(DEPS)
	$(CC) $(LDFLAGS) -c -o $@ $< $(LIBS)

main: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f main *.o
.PHONY: all clean
