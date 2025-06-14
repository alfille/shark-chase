TARGET = shark
LIBS = -lgsl -lm
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
ifeq ($(MAKELEVEL),0)
	$(CC) $(OBJECTS) -Wall -o $@ $(LIBS)
else
	$(CC) $(OBJECTS) -Wall -o ../$@ $(LIBS)
endif

clean:
	-rm -f *.o
	-rm -f $(TARGET)

