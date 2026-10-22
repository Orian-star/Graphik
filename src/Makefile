CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
LDFLAGS ?=
LDLIBS ?= -lm

TARGET := graphik
SOURCES := main.c regex.c export.c
OBJECTS := $(SOURCES:.c=.o)
HEADERS := regex.h main.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJECTS) $(TARGET)
