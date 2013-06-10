TARGET=newspaper
TEST=test
CC=gcc
CFLAGS=-I./src/. -Wall
LFLAGS=`ld -lwiringPi 2>/dev/null && echo "-lwiringPi" || echo ""`
BUILD_PATH=build/
SRC_PATH=src/
SRC= $(wildcard $(SRC_PATH)*.c)
OBJ=$(subst $(SRC_PATH),$(BUILD_PATH),$(SRC:.c=.o))


compile: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(BUILD_PATH)test.o: testing/test.c
	$(CC) -c -o $@ $^ $(CFLAGS) -Isrc/.

$(BUILD_PATH)%.o: $(SRC_PATH)%.c
	if [ ! -e build ]; then mkdir build; fi
	$(CC) -c -o $@ $^ $(CFLAGS)

mrproper: clean
	if [ -e $(TARGET) ]; then rm $(TARGET); fi
	if [ -e $(TEST) ]; then rm $(TEST); fi

clean:
	if [ -e $(BUILD_PATH) ]; then rm -r $(BUILD_PATH); fi

$(TEST): $(BUILD_PATH)bit_array.o $(BUILD_PATH)font.o $(BUILD_PATH)ledmatrix.o $(BUILD_PATH)test.o $(BUILD_PATH)animate.o $(BUILD_PATH)animationpool.o $(BUILD_PATH)display.o $(LFLAGS)
	$(CC) -o $@ $^


toto:
	echo $(TOTO)
