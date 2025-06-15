TARGET = shark
LIBS = -lgsl -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

DIR := $(notdir $(CURDIR))

OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS := $(wildcard *.h)

%.o :%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -o $@_$(DIR) $(LIBS)
ifeq ($(MAKELEVEL),1)
	cp -f $@_$(DIR) ..
endif

clean:
	-rm -f *.o
	-rm -f $(TARGET)

