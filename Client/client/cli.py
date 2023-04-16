from .client import Client


class CLI:

    def __init__(self):
        self.events = []
        self.client = Client(self.write_event)
        __event_loop()

    def __event_loop(self):
        while True:
            for k in self.events.keys():
                event, value = self.events.pop(k)

    def write_event(self, key, value):
        self.events[key] = value
