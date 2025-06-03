Boost.Asio Chat Server ! 


Boost.asio tcp chatServer . It allows multiple clients to connect and exchange message
in readl-time using asynchronous networking . 
Clients can connect using telnet or client application .
In server you can see the clients id and ip  , you ca delete them from the server .
in

## Requirements

-C++ compiler
-CMake
-Boost (install with 'sudo apt install libboost-dev' o manually download)

## Build & Run

'''sh
mkdir build && cd build
cmake ..
make
./Boost.Asio_P
