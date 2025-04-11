SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard include/*.h)

client.exe: $(SOURCES) $(HEADERS)
	g++ $(SOURCES) -o client -I include/ -I ../shared/include/ -L lib/ -lraylib -lgdi32 -lwinmm -lws2_32