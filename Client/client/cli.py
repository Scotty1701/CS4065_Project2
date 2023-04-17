import time
from .client import Client
from .client import threaded
from .client import event_list
from rich import print as richprint
from rich.table import Table
from getkey import getkey, keys
import sys
import curses
import readline
from threading import Event
from threading import Thread
import signal
from rich.console import Console
import os

os.system("")


class Exit(Exception):
    pass


class CLI:

    def __init__(self):
        self.console = Console()
        self.output = self.console.capture()
        with self.output:
            self.console.print("welcome to the output")
        self.pager = self.console.pager()
        self.events = {}
        self.client = Client(self.write_event)
        self.print_event = Event()
        self.print_event.set()
        self.on = True
        self.event_loop = self.__event_loop()
        self.event_loop.start()
        t = self.__input()
        t.start()
        self.event_loop.join()

    @threaded
    def __event_loop(self):
        while True:
            time.sleep(0.05)
            for event in self.events.copy().keys():
                value = self.events.pop(event)
                if event in event_list:
                    f = getattr(self, event)
                    f(value)

    @threaded
    def __input(self):
        while self.on:
            i = input("> ")

            args = i.split()
            if len(args) > 0 and args[0] in self.client.command_list:
                f = getattr(self.client, args[0])
                self.print_event.clear()
                Thread(target=f, args=args[1:]).start()
            elif len(args) > 0 and args[0] == "output":
                self.print_event.wait(2)
                with self.pager:
                    self.console.print(self.output.get())
            else:
                print("No command by that name")

            if len(args) > 0 and args[0] == "help":
                self.print_event.wait(2)
                with self.pager:
                    self.console.print(self.output.get())

    def safe_print(self, s):
        with self.output:
            self.console.print(s)
        self.print_event.set()

    def write_event(self, key, value=None):
        self.events[key] = value

    def log(self, content=""):
        self.safe_print(content)

    def exit(self, *args):
        self.on = False
        sys.exit(0)

    def message(self, value):
        username, content = value
        t = Table()
        t.add_column(username)
        t.add_row(content)
        self.safe_print(value)
