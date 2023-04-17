import time
from .client import Client
from .client import threaded
from .client import event_list
from rich import print
from rich.table import Table
from getkey import getkey, keys
import sys
import curses
import readline
from threading import Event


class CLI:

    def __init__(self):
        self.events = {}
        self.client = Client(self.write_event)
        self.print_event = Event()
        self.print_event.set()
        self.__event_loop().start()
        self.__input()

    @threaded
    def __event_loop(self):
        while True:
            time.sleep(0.05)
            for event in self.events.copy().keys():
                value = self.events.pop(event)
                if event in event_list:
                    f = getattr(self, event)
                    f(value)

    def __input(self):
        while True:
            self.print_event.wait()
            i = input("> ")
            if i.split()[0] in self.client.command_list:
                f = getattr(self.client, i.split()[0])
                self.print_event.clear()
                f(*(i.split()[1:]))
            else:
                self.write_event("log", "No command by that name")
            self.print_event.clear()

    def safe_print(self, s):
        print("\n")
        print(s)
        self.print_event.set()

    def write_event(self, key, value=None):
        self.events[key] = value

    def log(self, content=""):
        self.safe_print(content)

    def message(self, value):
        username, content = value
        t = Table()
        t.add_column(username)
        t.add_row(content)
        self.safe_print(t)
