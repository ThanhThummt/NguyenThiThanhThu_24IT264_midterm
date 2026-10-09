CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = ls
SRCS = main.c options.c entry.c sort.c display.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
