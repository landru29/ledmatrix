TARGET=project1
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

$(BUILD_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

folders:
	if [ ! -e build ]; then mkdir build; fi

mrproper: clean
	rm $(TARGET)
	
clean:
	rm -r $(BUILD_PATH)
