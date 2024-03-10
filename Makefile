
server: server.cpp Command.h binary_io.h gpio_manager.h Server.h Socket.h voltage_control.h
	g++ -o server server.cpp -lfmt -lgpiod

client: client.cpp Command.h Socket.h binary_io.h
	g++ -o client client.cpp

