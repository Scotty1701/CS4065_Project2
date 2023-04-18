import time
from .client import Client
from .client import threaded
from .client import event_list
from rich import print as print
from rich.table import Table
from getkey import getkey, keys
import sys
from threading import Event
from threading import Thread
import signal
from rich.console import Console
from rich.text import Text
from rich.table import Column
from rich.align import Align
import os
import json

os.system("")


class Exit(Exception):
    pass


class CLI:

    def __init__(self):
        self.console = Console()
        self.output = ""
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

                def thread_wrapper(*args):
                    try:
                        f(*args)
                    except TypeError as e:
                        self.write_event("log", "you goofed!!")

                Thread(target=thread_wrapper, args=args[1:]).start()
            elif len(args) > 0 and args[0] == "output":
                self.print_event.wait(2)
                with self.console.pager(styles=False):
                    self.console.print(Text.from_ansi(self.output))
            else:
                print("No command by that name")

            if len(args) > 0 and args[0] == "help":
                self.print_event.wait(2)
                with self.console.pager(styles=False):
                    self.console.print(
                        "you can view this output again by typing [bold blue]output[/bold blue]"
                    )
                    self.console.print(Text.from_ansi(self.output))

    def safe_print(self, s, log=False):
        with self.console.capture() as capture:
            if log:
                self.console.log(s)
            else:
                self.console.print(s)
        self.output += capture.get()
        self.print_event.set()

    def write_event(self, key, value=None):
        self.events[key] = value

    def log(self, content=""):
        self.safe_print(content, log=False)

    def exit(self, *args):
        self.on = False

    def post(self, value):
        self.safe_print("message delivered successfully")

    def connect(self, value):
        self.safe_print("connected successfully")

    def join(self, value):
        self.safe_print("joined successfully")

    def leave(self, value):
        self.safe_print("left successfully")

    def message(self, value):
        username, subject, content, id, date, group = value["sender"], value[
            "subject"], value["content"], value["message_id"], value[
                "post_date"], value["group_id"]
        t = Table(
            Column(header=subject, justify="center"),
            title=f"from:{username} id:{id} group:{group}",
            expand=True,
        )
        t.add_row(Align(content, align="left"))
        self.safe_print(t)
