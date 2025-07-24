

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src
SRC_DIR = src
BIN_DIR = bin
TARGET = kakuro

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

run: $(BIN_DIR)/$(TARGET)
	./$(BIN_DIR)/$(TARGET) resources/puzzle.txt simple

clean:
	rm -rf $(BIN_DIR)
