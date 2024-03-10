
server: server.cpp Command.h Command.cpp binary_io.h GpioManager.h GpioManager.cpp Socket.h Socket.cpp VoltageControl.h VoltageControl.cpp
	g++ -o server -DBUILD_SERVER server.cpp Command.cpp GpioManager.cpp Socket.cpp VoltageControl.cpp -lfmt -lgpiod

client: client.cpp Command.h Socket.h Socket.cpp binary_io.h
	g++ -o client client.cpp Socket.cpp 

clean:
	rm -rf client server
