# The Vircon Project

The Vircon Project transforms your smartphone into a virtual game controller for Linux games. The project includes three components:

- **Server**: Written in C++, uses `libevdev` to emulate virtual game controllers on Linux and `websockets` for communication with clients.
- **Client**: Written with Flutter, connects to the server, presenting a clean and intuitive game controller interface.
- **Editor**: Written in C++, uses `imgui` and allows customization of controller layouts to suit your play style.

## Vircon Server

#### Dependencies
The server is written in C++ and uses CMake. Ensure you have the following dependencies installed:

```bash
sudo apt install build-essential cmake libnotcurses-core-dev libevdev-dev libwebsocketpp-dev
```

#### Building
Clone this repository, and navigate to the server directory:
```bash
git clone https://github.com/RMZeroFour/vircon-project
cd vircon-project/server
```

Build the server with CMake:
```bash
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Running the Server

Once built, navigate to the `build` directory and start the server:

```bash
cd build
./vircon-server
```

## License

This project is licensed under the [MIT License](LICENSE.md).