from threading import Event, Thread
import socket
import json
import time
import re
import pyspam
from datetime import datetime
import inspect
import signal
import sys

event_list = [
    "log", "getgroups", "message", "exit", "post", "join", "connect", "leave"
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
        self.group_id = None
        signal.signal(signal.SIGINT, self.signal_handler)

    def signal_handler(self, sig, frame):
        print("Bye bye")
        self.exit()
        sys.exit()

    def sendall(self, message):
        try:
            self.server_socket.sendall(message.encode())
        except OSError:
            pass

    def connect(self, address: str = "127.0.0.1", port: str = "42000"):
        """ connect address port """
        try:
            self.server_socket.connect((address, int(port)))
        except OSError:
            print("can't connect to socket, maybe server isn't running?")
            return
        self.recieve_thread.start()
        message = pyspam.gen.request.connect(address, port)
        self.sendall(message)

    @threaded
    def recieve(self):
        message = ""
        while True:
            try:
                message += self.server_socket.recv(1024).decode()
            except OSError:
                self.exit()
                return
            left = message.count("{")
            right = message.count("}")
            if right == left:
                if left > 1:
                    self.dispatch(message.split("}")[0] + '}')
                    self.dispatch(message.split("}")[1] + '}')
                else:
                    self.dispatch(message)
                message = ""

    def dispatch(self, message):
        parsed = pyspam.parse(message)
        try:
            ogmessage = json.loads(message)
        except json.decoder.JSONDecodeError as e:
            print(message)
        message_type = parsed["message_type"]
        if message_type == "":
            self.exit()
        if message_type in event_list and ogmessage["success"]:
            self.write_event(message_type, parsed)
        else:
            self.write_event("log", parsed)

    def join(self, username: str, groupid: str = "0"):
        """ join the server (join username groupid"""
        message = pyspam.gen.request.join(username, groupid)
        if self.group_id:
            self.leave()
        self.username = username
        self.group_id = groupid
        self.sendall(message)

    def exit(self):
        """ exit the client"""
        if self.group_id:
            self.leave()
        self.server_socket.close()
        self.write_event("exit", "")

    def post(self, subject: str, *content: str):
        """ send messages !!"""
        if not self.group_id:
            self.write_event(
                "log", "You can't post messages without joining a group!!!")
            return
        username = self.username
        groupname = self.group_id
        now = datetime.now().strftime("%H:%M%S")
        message = pyspam.gen.request.post(groupname, username, now, subject,
                                          " ".join(content))
        self.sendall(message)

    def message(self, message_id):
        """ get messages !!"""
        if not self.group_id:
            self.write_event(
                "log", "you can't get messages from a group you aren't in!!!")
            return
        group_id = self.group_id
        message = pyspam.gen.request.message(group_id, message_id)
        self.sendall(message)

    def leave(self):
        """ leave the group """
        if not self.group_id:
            self.write_event("log", "you can't leave without joining first!!!")
            return
        group_id = self.group_id
        message = pyspam.gen.request.leave(group_id)

        self.sendall(message)
        self.group_id = None

    def getusers(self):
        """ get the users """
        if not self.group_id:
            self.write_event(
                "log", "you can't get users from a group you aren't in!!!")
            return
        group_id = self.group_id
        message = pyspam.gen.request.getusers(group_id)
        self.sendall(message)

    def getgroups(self, *args):
        """ get the groups """
        message = pyspam.gen.request.getgroups()
        self.sendall(message)

    def help(self, command=None):
        """ we know you need it """
        if command:
            f = getattr(self, command)
            sig = inspect.signature(f)
            args = list(sig.parameters.values())
            args = [str(arg) for arg in args]
            self.write_event("log", f"{f.__doc__}" + f"\nargs:\n{args}")
            return
        self.write_event("log", f"Available commands:{self.command_list}")
