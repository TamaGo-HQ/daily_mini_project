CC = gcc

CFLAGS = -Wall -Werror -g

TARGET = test_uart

SRC = tests/test_uart.c 

OBJ = $(SRC:.c=.o)

all : $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	del /F /Q tests\test_uart.o
	del /F /Q test_uart.exe



