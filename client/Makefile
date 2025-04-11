SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,build/%.o,$(SOURCES))
HEADERS := $(wildcard include/%.h)

-include $(OBJECTS:.o=.d)

BIN := client.exe

all: $(BIN)

$(BIN): $(OBJECTS)
	g++ $(OBJECTS) -o $@ -L lib/ -lraylib -lgdi32 -lwinmm -lws2_32

build/%.o: src/%.cpp
	g++ -c $< -o $@ -I include/ -I ../shared/include/ -MMD -MP

.PHONY: clean
clean:
	rm -rf build/*
