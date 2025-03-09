server.exe: server.cpp
	g++ server.cpp -o server -lws2_32
# g++ server.cpp -o server -lraylib -lgdi32 -lwinmm -lws2_32


client.exe: client.cpp clientnetworking.cpp
	g++ client.cpp clientnetworking.cpp -o client -lraylib -lgdi32 -lwinmm -lws2_32