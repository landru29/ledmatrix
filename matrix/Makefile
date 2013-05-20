TARGET=matrix
CC=gcc
CFLAGS=
LFLAGS=-lwiringPi
BUILD_PATH=build/
SRC_PATH=src/
SRC= $(wildcard $(SRC_PATH)*.c)
OBJ=$(subst $(SRC_PATH),$(BUILD_PATH),$(SRC:.c=.o))

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(BUILD_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -c -o $@ $^ $(CFLAGS)
