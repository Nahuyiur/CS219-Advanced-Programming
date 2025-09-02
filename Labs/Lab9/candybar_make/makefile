CXX = g++
CFLAGS = -Wall -c
LDFLAGS = -Wall
TARGET = candybar
OBJ = main.o candybar.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ)

main.o: main.cpp candybar.h
	$(CXX) $(CFLAGS) main.cpp

candybar.o: candybar.cpp candybar.h
	$(CXX) $(CFLAGS) candybar.cpp

.PHONY: clean
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)