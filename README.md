# Calvision Pi

Project for controlling the Calvision Raspberry Pi peripherals and communication. The `server` is
built and run on the Raspberry Pi (as it depends on `gpiod`), and sits listening on an open port
for client connections. The `client` can be built on any Linux machine and connects to the open server,
sending it TCP messages over a socket. 

## Install and Build

### Client-side
No dependencies. Normal git-clone and cmake build steps
```bash
git clone https://github.com/hhollenb/CalvisionPi
cd CalvisionPi
mkdir build && cd build
cmake -DBUILD_SERVER=OFF ..
cmake --build . --target client
```

### Server-side (Raspberry Pi)
Dependencies (can just be installed via package manager):
 - [fmt](https://github.com/fmtlib/fmt)
 - [gpiod](https://github.com/brgl/libgpiod)

Normal git-clone and cmake build steps
```bash
git clone https://github.com/hhollenb/CalvisionPi
cd CalvisionPi
mkdir build && cd build
cmake -DBUILD_SERVER=ON ..
cmake --build . --target server
```
Client can also be built with the same config if desired (but likely not used)
```bash
cmake --build . --target client
```


## Running

### Client-side

The client requires the IP address and port of the server to be specified.
```bash
./client/client ip-address port [command file]
```
The command file is optional. If no command file is specified, then input is read from stdin and interpreted
the same way. If an error value is returned from the server, then script execution is paused and waits for
user confirmation.

Commands consist of an identifier listed as enum entries of `CommandCode` in `common/Commands.h`. Command
scripts have one line per command, with the format
```
CommandIdentifier [parameter 1] [parameter 2] ...
```
Pararameters should be separated by a single space, and can be any integer or float that can be parsed by
`stoi` or `stof` (command implementation dependent). Blank lines don't execute anything. 

### Server-side

The server just requires an open port to be specified.
```bash
./server/server port
```
Runs until either a fatal error occurs or a quit message is sent. Only accepts one client at a time, and
accepts commands until the connection is closed by the client. After the connection is closed, it waits
for the next connection (so the server should only need to be started once and can stay running).


## Extending

There following are steps for adding a new command called `MyCommand`:
 1. Add a new entry `MyCommand` to the `CommandCode` enum in `common/Commands.h`. The identifier is
    the same as the command string used in parsing command files.
 2. Create a class called `CommandMyCommand` in `Command.h` with the following outline:
```cpp
class CommandMyCommand : public Command {
public:
    CommandMyCommand()
    {}

    virtual void write(Socket&) override {}
    virtual void read(Socket&) override {}

    ClassFields(MyCommand)
    static size_t size() { return 0; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) override;
#endif

private:
    // add parameters here
};
```
Size should equal the number of parameters of the command. The `write` and `read` functions are responsible
for writing and reading the parameters in the same order to/from the socket.
 3. In `Command.cpp`, add the following line to the switch in `create_command`:
```cpp
CommandCase(MyCommand)
```
Inside the `#ifdef BUILD_SERVER` block, also add the implementation of `CommandMyCommand::execute`:
```cpp
#ifdef BUILD_SERVER
...
ErrorCode CommandMyCommand::execute(Context& context) {
    return ErrorCode::Success;
}
...
#endif
```
 4. In `client.cpp` add the parsing statement to `run_commands`:
```cpp
ElseIfName(MyCommand)(/*std::stof(tokens[1]), std::stoi(tokens[2]), etc. */);
```

Just rebuild and you're done! Changes do need to be transfered to both the server and the client to
be consistent.
