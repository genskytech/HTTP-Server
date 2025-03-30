sudo apt-get install libboost-all-dev  # Linux
vcpkg install boost-asio              # Windows (using vcpkg)








How it Works
Uses Boost.Asio for async networking.

Spawns coroutines to handle multiple clients concurrently.

Listens on port 8080 and responds with "Hello, World!".