from threading import Event, Thread
import socket
import json
import time
import re


def threaded(func):

    def f(*args, **kwargs):
        return Thread(target=func, args=args, kwargs=kwargs, daemon=True)

    return f


class Client:
    left_bracket = re.compile(r"{")
    right_bracket = re.compile(r"}")
    command_list = ["connect", "join", "help"]

    def __init__(self, write_event):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.write_event = write_event

    def connect(self, address: str, port: str):
        """ connect address port """
        self.server_socket.connect((address, int(port)))

    @threaded
    def recieve(self):
        message = ""
        while True:
            message += self.server_socket.recv(1024).decode()
            left = left_bracket.findall(self.recieve.message)
            right = right_bracket.findall(self.recieve.message)
            if right == left:
                self.dispatch(message)

    def dispatch(self, message):
        pass

    def join(self, username: str, groupname: str = "public"):
        message = username
        self.server_socket.sendall(message)

    def help(self, command):
        print(getattr(self, command).__doc__)
