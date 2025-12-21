# Compiler and Flags
CC = gcc
CFLAGS = -Wall -g

# Target Executable
TARGET = scheduler

# Source Files (We will add the algorithm .c files here as we create them)
SRCS = main.c fcfs.c sjf.c rr.c priority.c

# Object Files (generated automatically from SRCS)
OBJS = $(SRCS:.c=.o)

# Default Rule: Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c scheduler.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Rule: Remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)