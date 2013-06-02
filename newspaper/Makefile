TARGET=newspaper
CC=gcc
CFLAGS=
LFLAGS=-lwiringPi
BUILD_PATH=build/
SRC_PATH=src/
SRC= $(wildcard $(SRC_PATH)*.c)
OBJ=$(subst $(SRC_PATH),$(BUILD_PATH),$(SRC:.c=.o))

compile: folders $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(BUILD_PATH)test.o: testing/test.c
	$(CC) -c -o $@ $^ $(CFLAGS) -Isrc/.

$(BUILD_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

folders:
	if [ ! -e build ]; then mkdir build; fi

mrproper: clean
	rm $(TARGET)
	
clean:
	rm -r $(BUILD_PATH)

test: $(BUILD_PATH)bit_array.o $(BUILD_PATH)font.o $(BUILD_PATH)ledmatrix.o $(BUILD_PATH)test.o
	$(CC) -o $@ $^
