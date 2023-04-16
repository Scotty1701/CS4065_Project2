from textual.app import App, ComposeResult
from textual.widgets import TextLog
from textual import events
from textual.message import Message, MessageTarget
from textual.widgets import Input, Static
from textual.color import Color
from contextlib import redirect_stdout
import io
from .client import Client


class tui(App):
    CSS_PATH = "tui.css"

    def __init__(self):
        super().__init__()
        self.client = Client(self.write_event)

    def write_event(self, key, value):
        pass

    def compose(self) -> ComposeResult:
        yield TextLog()
        yield Input(placeholder="Enter Command Here")

    def on_input_submitted(self, message: Input.Submitted) -> None:
        textlog = self.query_one(TextLog)
        textlog.write(message.value)
        input = self.query_one(Input)
