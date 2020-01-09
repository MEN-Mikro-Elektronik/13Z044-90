CC ?= gcc
TARGET = fb16z044_256x64_test
SRCS=$(TARGET).c
CFLAGS=-g -Wall -Wextra

all: $(TARGET)

$(TARGET): $(SRCS) logos.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lm

.PHONY: clean
clean:
	-rm -f $(TARGET) *.o core

