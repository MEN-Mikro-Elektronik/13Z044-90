CC ?= gcc
TARGET = fb16z044_256x64_test
SRCS=$(TARGET).c
CFLAGS=-g -lm -Wall -Wextra 

all: $(TARGET)

$(TARGET): $(SRCS) logos.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean
clean:
	-rm -f $(TARGET) *.o core

