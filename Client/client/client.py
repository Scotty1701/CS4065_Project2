from threading import Event, Thread
import socket
import json
import time
import re
import pyspam
from datetime import datetime

event_list = ["log", "group", "message", "exit"]


def threaded(func):

    def f(*args, **kwargs):
        return Thread(target=func, args=args, kwargs=kwargs, daemon=True)

    return f


class Client:
    left_bracket = re.compile(r"{")
    right_bracket = re.compile(r"}")
    command_list = ["connect", "join", "post", "exit", "help"]

    def __init__(self, write_event):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.write_event = write_event
        self.recieve_thread = self.recieve()

    def connect(self, address: str = "127.0.0.1", port: str = "42000"):
        """ connect address port """
        self.server_socket.connect((address, int(port)))
        self.recieve_thread.start()
        message = pyspam.gen.request.connect(address, port)
        self.server_socket.sendall(message.encode())
        self.write_event("log", "successfully connected")

    @threaded
    def recieve(self):
        message = ""
        while True:
            message += self.server_socket.recv(1024).decode()
            left = message.count("{")
            right = message.count("}")
            if right == left:
                self.dispatch(message)

    def dispatch(self, message):
        self.write_event("log", message)

    def join(self, username: str, groupid: str = "1"):
        """ join the server (join username groupid"""
        message = pyspam.gen.request.join(username, groupid)
        self.server_socket.sendall(message.encode())

    def exit(self):
        """ exit the client"""
        self.server_socket.close()
        self.write_event("exit")

    def post(self,
             username: str,
             subject: str,
             content: str,
             groupname: str = "1"):
        now = datetime.now().strftime("%H:%M%S")
        message = pyspam.gen.request.post(groupname, username, now, subject,
                                          content)
        self.server_socket.sendall(message.encode())

    def help(self, command=None):
        if command:
            self.write_event("log", getattr(self, command).__doc__)
            return
        self.write_event("log", f"Available commands:{self.command_list}")
        import time
        time.sleep(2)
        self.write_event("message", ["hi", "world"])
