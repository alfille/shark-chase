TARGET = shark_binary
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS := $(wildcard *.h)

%.o :%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -o $@ $(LIBS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)

