# Running

To run the programs in the repo, you can either go the nix route (probably easier), docker route (easiest?), or the manual route.

## Nix route

- Install [nix](https://nixos.org/download.html#nix-install-linux) for your favorite
  Unix/wsl2 operating system. (macOS is untested).
- Enable flakes by running `echo experimental-features = nix-command flakes >> ~/.config/nix/nix.conf` or
  by adding the `--experimental-features 'nix-command flakes'` flag right after `nix` in the following commands.
- Assuming that you are in the root of this repo, run `nix run .#client -- <cli or gui>` to run the client
- run `nix run .#server` to run the server

## Docker route

Assuming you have the docker image file

- `docker load -i image`

If you want the gui to work:

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
```

While in the container you can run `server` to start the server and `client` to run the client.

if you want server and clients in the same shell, do something like

```bash
server > serverlog.txt&
# gui
client gui&
# cli (only one per shell)
client cli
```

## Manual route

### Prerequisites

- At least python 3.9 (tested with python 3.10)
- `cmake` (tested with version 3.25.1)
- Some `cmake` compatible C++ compiler for the operating system of choice

### Building/Running

- First follow the build instructions in the README of the `Protocol` directory.
- Then follow the build/run instructions in each of the `Client` and `Server` directories
