import PySimpleGUI as sg
from threading import Thread, Event


def listener(Event: event):

    def wait_for_event(func):
        event.wait()
        func()

    return wait_for_event


class gui:
    messager = sg.Column([[
        sg.Frame(title="messages",
                 layout=[[sg.Text(expand_x=True, expand_y=True)]],
                 expand_x=True,
                 expand_y=True,
                 size=(600, 400))
    ], [sg.Text("Enter your message:")
        ], [sg.InputText(key="message", expand_x=True)], [sg.Button("Send")]],
        expand_x=True,
        expand_y=True)

    groups = sg.Column([
        [sg.Text("Server Address"),
         sg.InputText(key="address", size=(20, 0))],
        [sg.Button("Connect")],
        [sg.Text("Username"),
         sg.InputText(key="username", size=(20, 0))],
        [sg.Frame(title="Groups", layout=[[]], expand_x=True, expand_y=True)],
    ],
        expand_x=True,
        expand_y=True)
    layout = [[groups, sg.VSeparator(), messager]]

    def __init__(self):
        self.gui_process = Thread(self.__start_gui(), daemon=True)
        self.gui_process.start()

    def __start_gui(self):
        window = sg.Window('Window Title', self.layout, resizable=True)
        while True:
            # event loop
            event, values = window.read()
            if event == sg.WIN_CLOSED:
                break
            if event == "Send":
                print('You sent', values["message"])
                window["message"]("")
        window.close()


# tests
if __name__ == "__main__":
    gui()
