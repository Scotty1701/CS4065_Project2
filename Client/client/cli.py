from .client import Client
from .client import threaded
import time


class CLI:

    def __init__(self):
        self.events = {}
        self.client = Client(self.write_event)
        self.__event_loop().start()
        while True:
            i = input("> ")
            if i.split()[0] in self.client.command_list:
                f = getattr(self.client, i.split()[0])
                f(*(i.split()[1:]))
            else:
                print("No command by that name")

    @threaded
    def __event_loop(self):
        while True:
            time.sleep(0.05)
            for k in self.events.keys():
                print(k)
                event, value = self.events.pop(k)

    def write_event(self, key, value):
        self.events[key] = value
