# Authors

David Lewis, Issac Poplin

# Usage

Some of the usage instructions for the client are different from
what was given in the assignment, refer to the README in the `Client` directory for more info.

To start the server:

```
server
```

To start the client (it should walk you through it):

```
client
```

# Getting up and Running

To run the programs in the repo, you can either go the nix route (probably easier), docker route (also fairly easy), or the manual route.

## Nix route

> Note: the .nix files replace the tradition Makefile, the only thing you need to install is nix, it will handle all dependencies

- Install [nix](https://nixos.org/download.html#nix-install-linux) for your favorite
  Unix/wsl2 operating system. (macOS is untested).
- Enable flakes by running `echo experimental-features = nix-command flakes >> ~/.config/nix/nix.conf` or
  by adding the `--experimental-features 'nix-command flakes'` flag right after `nix` in the following commands.
- Assuming that you are in the root of this repo, run `nix run .#client -- <cli or gui>` to run the client
- run `nix run .#server` to run the server

## Docker route

> note that the docker image can be generated using the `nix build .#docker` command if you really want the container separately

Get the docker file [here](https://github.com/Scotty1701/CS4065_Project2/releases/download/v1.0.0/project)

> Assuming you have the docker image file

- `docker load -i image`

If you want the gui to work you'll need wsl2 on Windows 11 (untested) or a Linux environment:

- you'll need `xhost` (`sudo apt install x11-utils`)

```bash
xhost +
docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix \
                -v /mnt/wslg:/mnt/wslg \
                -e DISPLAY \
                -e WAYLAND_DISPLAY \
                -e XDG_RUNTIME_DIR \
                -e PULSE_SERVER \
                csproject:latest
```

Otherwise, you can just do:

```bash
run -it csproject:latest
```

You can connect to an existing instance of the docker container with

```bash
docker exec -it <container name> bash
client cli
client gui
```

While in the container you can run `server` to start the server and `client` to run the client.

If you want server and clients in the same shell, do something like:

```bash
server > serverlog.txt&
# gui
client gui&
# cli (only one per shell)
client cli
```

> note: I do not recommend this

## Manual route

> note that this method is untested, but it should work "in theory"

### Prerequisites

- At least python 3.9 (tested with python 3.10)
- `cmake` (tested with version 3.25.1)
- Some `cmake` compatible C++ compiler for the operating system of choice

### Building/Running

- First follow the build instructions in the README of the `Protocol` directory.
- Then follow the build/run instructions in each of the `Client` and `Server` directories

# Major struggles

- Making the client and server mature at the same time was very difficult.
  It involved a lot of back and forth between teammates to ensure we were implementing the right kind of communication

- client gui was more difficult than anticipated,
  I also chose a very limited gui framework by accident,
  and haven't really worked with GUIs a lot before. I'm pretty happy with the final result though.

- Lots of work was due to different development environments (windows 10 vs linux+nix) and the associated compatibility issues
  Eventually settled on updating the socket abstractin library we created to work for windows and linux
  Also used docker+wsl2 to test on windows 10

- The communication protocol was the first part of the project developed and implemented, so several times we had to go back
  and change a major part after realizing it wouldn't work well when implementing some part of the client or server.
