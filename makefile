CXX = gcc
CXXFLAGS = -Wall -g
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(patsubst %.c, %.o, $(C_FILES))

all: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o WordReplacer $(OBJ_FILES)
src/%.o : src/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f src/*.o run
