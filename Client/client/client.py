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

# list of events that the frontend can process
event_list = [
    "log", "getgroups", "message", "exit", "post", "join", "connect", "leave"
]


def threaded(func):

    def f(*args, **kwargs):
        return Thread(target=func, args=args, kwargs=kwargs, daemon=True)

    return f


class Client:
    """
    this class does most of the processing
    for the client server interaction.
    It passes its results to the front-end using the write_event function.
    """

    # list of commands that the client can process from the front-end
    command_list = [
        "connect", "join", "post", "exit", "help", "message", "leave",
        "getusers", "getgroups"
    ]

    def __init__(self, write_event):
        """ set up the needed variables and create signal handler for Ctrl-C"""
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,
                                      1)
        self.write_event = write_event
        self.recieve_thread = self.recieve()
        self.username = None
        self.group_id = None
        signal.signal(signal.SIGINT, self.signal_handler)

    def signal_handler(self, sig, frame):
        self.exit()

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
            self.write_event(
                "log", "can't connect to socket, maybe server isn't running")
            return
        except ValueError:
            self.write_event(
                "log",
                "Those don't look like the right kind of arguments, you should try again!"
            )
            return
        self.recieve_thread.start()
        message = pyspam.gen.request.connect(address, port)
        self.sendall(message)

    @threaded
    def recieve(self):
        """ listens for messages at all times, calling dispatch when it recieves a valid message"""
        message = ""
        while True:
            try:
                message += self.server_socket.recv(1024).decode()
            except OSError:
                self.write_event("log", "Lost connection to server")
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
        """ calls an appropriate functino based on the message """
        parsed = pyspam.parse(message)
        try:
            ogmessage = json.loads(message)
        except json.decoder.JSONDecodeError as e:
            self.write_event("log",
                             "server sent an invalid message, maybe it died?")
        message_type = parsed["message_type"]
        if message_type == "":
            self.write_event("log", "Lost connection to server")
            self.server_socket.close()
            self.server_socket = socket.socket(socket.AF_INET,
                                               socket.SOCK_STREAM)
        if message_type in event_list and ogmessage["success"]:
            self.write_event(message_type, parsed)
        else:
            self.write_event("log", parsed)

    def join(self, username: str, groupid: str = "0"):
        """ join the server (join username groupid"""
        try:
            message = pyspam.gen.request.join(username, str(int(groupid)))
        except ValueError:
            self.write_event(
                "log",
                "Those don't look like the right kind of arguments, you should try again!"
            )
            return

        self.username = username
        if not self.group_id:
            self.group_id = [groupid]
        else:
            self.group_id += [groupid]
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
        try:
            groupname = int(subject)
            subject = content[0]
        except ValueError as e:
            groupname = self.group_id[0]

        username = self.username
        now = datetime.now().strftime("%H:%M%S")
        message = pyspam.gen.request.post(str(groupname), username, now,
                                          subject, " ".join(content))
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

    def leave(self, group_id=None):
        """ leave the group """

        if not self.group_id:
            self.write_event("log", "you can't leave without joining first!!!")
            return
        if group_id:
            group_id = [group_id]
        else:
            group_id = self.group_id
        for id in group_id:
            message = pyspam.gen.request.leave(id)
            self.sendall(message)
        self.group_id = None

    def getusers(self, group_id):
        """ get the users """
        if not self.group_id:
            self.write_event(
                "log", "you can't get users from a group you aren't in!!!")
            return
        if group_id:
            pass
        else:
            group_id = self.group_id[0]
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
