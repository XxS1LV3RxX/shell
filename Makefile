CC = gcc

CFLAGS = -Wall -Wextra

TARGET = mi_shell

SOURCES = shell.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
