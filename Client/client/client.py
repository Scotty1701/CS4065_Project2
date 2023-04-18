from threading import Event, Thread
import socket
import json
import time
import re
import pyspam
from datetime import datetime
import inspect

event_list = [
    "log", "group", "message", "exit", "post", "join", "connect", "leave"
]


def threaded(func):

    def f(*args, **kwargs):
        return Thread(target=func, args=args, kwargs=kwargs, daemon=True)

    return f


class Client:
    left_bracket = re.compile(r"{")
    right_bracket = re.compile(r"}")
    command_list = [
        "connect", "join", "post", "exit", "help", "message", "leave",
        "getusers", "getgroups"
    ]

    def __init__(self, write_event):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.write_event = write_event
        self.recieve_thread = self.recieve()
        self.username = None

    def connect(self, address: str = "127.0.0.1", port: str = "42000"):
        """ connect address port """
        self.server_socket.connect((address, int(port)))
        self.recieve_thread.start()
        message = pyspam.gen.request.connect(address, port)
        self.server_socket.sendall(message.encode())

    @threaded
    def recieve(self):
        message = ""
        while True:
            message += self.server_socket.recv(1024).decode()
            left = message.count("{")
            right = message.count("}")
            if right == left:
                self.dispatch(message)
                message = ""

    def dispatch(self, message):
        parsed = pyspam.parse(message)
        ogmessage = json.loads(message)
        message_type = parsed["message_type"]
        if message_type in event_list and ogmessage["success"]:
            self.write_event(message_type, parsed)
        else:
            self.write_event("log", parsed)

    def join(self, username: str, groupid: str = "1"):
        """ join the server (join username groupid"""
        message = pyspam.gen.request.join(username, groupid)
        self.username = username
        self.server_socket.sendall(message.encode())

    def exit(self):
        """ exit the client"""
        self.server_socket.close()
        self.write_event("exit")

    def post(self, subject: str, content: str, groupname: str = "1"):
        if not self.username:
            self.write_event("log",
                             "You can't post messages without a username!!!")
        username = self.username
        now = datetime.now().strftime("%H:%M%S")
        message = pyspam.gen.request.post(groupname, username, now, subject,
                                          content)
        self.server_socket.sendall(message.encode())

    def message(self, group_id, message_id):
        message = pyspam.gen.request.message(group_id, message_id)
        self.server_socket.sendall(message.encode())

    def leave(self, group_id="1"):
        message = pyspam.gen.request.leave(group_id)
        self.server_socket.sendall(message.encode())

    def getusers(self, group_id="1"):
        message = pyspam.gen.request.getusers(group_id)
        self.server_socket.sendall(message.encode())

    def getgroups(self, *args):
        message = pyspam.gen.request.getgroups()
        self.server_socket.sendall(message.encode())

    def help(self, command=None):
        if command:
            f = getattr(self, command)
            sig = inspect.signature(f)
            args = list(sig.parameters.values())
            args = [str(arg) for arg in args]
            self.write_event("log", f"{f.__doc__}" + f"\nargs:\n{args}")
            return
        self.write_event("log", f"Available commands:{self.command_list}")
