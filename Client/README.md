# Running

Make sure you followed the instructions in Protocol and are in the a virtual environment

- `python -m client` will print the help
- `python -m client gui` will start the gui
- `python -m client cli` will start the cli

if using the nix or docker methods, `client` by itself should also work

## using the cli

The CLI has its output and input in different "windows".
The output window is reachable by the `output` command, and `help` will also take you there.

The output window is really your system pager, which is usually `less` on linux

- Run `help` to list commands
- run `help command` to describe its usage.
- connect, join, message, etc are all there, but have slightly different usages than what was given as an example in the homework writeup

# Building Manually?

Since this is a python module, as long as you followed the protocol directions, you should be able to do a `pip install .` to get the module in your virtual environment.
