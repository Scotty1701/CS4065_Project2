from threading import Event, Thread
import socket
import json


class Client:

    def __init__(self):
        listening = Event()
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self, String: address, String: port):
        self.server_socket.connect((address, port))

    def join(self, String: username, String: groupname = "public"):
        message =
        self.server_socket.sendall(message)
