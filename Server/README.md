# Message Board Server

Follow the instructions below to build/run standalone. You probably should follow the instructions in the top level README instead though.

# Building

1. First build the protocol package as described in its respective README.
1. Starting from the root of this package:
1. `mkdir build`
1. `cd build`
1. `cmake ..`
1. `cmake --build . --config Release`
1. `cmake --install . --prefix ../install`

# Running
1. Navigate to the install directory
1. Run the board_server executable
