# Running

To run the programs in the repo, you can either go the nix route (probably easier), or the manual route.

## Nix route

- Install [nix](https://nixos.org/download.html#nix-install-linux) for your favorite
  Unix/wsl2 operating system. (macOS is untested).
- Enable flakes by running `echo experimental-features = nix-command flakes >> ~/.config/nix/nix.conf` or
  by adding the `--experimental-features 'nix-command flakes'` flag right after `nix` in the following commands.
- Assuming that you are in the root of this repo, run `nix run .#client` to run the client
- run `nix run .#server` to run the server

## Manual route

### Prerequisites

- At least python 3.9 (tested with python 3.10)
- `cmake` (tested with version 3.25.1)
- Some `cmake` compatible C++ compiler for the operating system of choice

### Building/Running

- First follow the build instructions in the README of the `Protocol` directory.
- Then follow the build/run instructions in each of the `Client` and `Server` directories
