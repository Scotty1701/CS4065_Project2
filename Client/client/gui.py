import PySimpleGUI as sg
from threading import Thread, Event
from .client import Client


class gui:

    def __init__(self, theme):
        # set of groups, each containing a list of messages
        self.groups = {}
        self.current_group = None

        # display theme preview
        if theme == "preview":
            sg.preview_all_look_and_feel_themes()
            return

        # create gui
        sg.theme(theme)

        self.colors = sg.LOOK_AND_FEEL_TABLE[sg.theme()]
        for i in range(1, 4):
            a = f"ACCENT{i}"
            if a not in self.colors:
                self.colors[a] = list(self.colors.values())[i]
        print(self.colors)
        messager = sg.Column([[
            sg.Frame(title="messages",
                     layout=[[
                         sg.Multiline(
                             default_text="",
                             write_only=True,
                             disabled=True,
                             expand_x=True,
                             expand_y=True,
                             no_scrollbar=True,
                             key="messages",
                             background_color=self.colors["BACKGROUND"],
                             size=(10, None),
                             border_width=0),
                     ]],
                     expand_x=True,
                     expand_y=True,
                     size=(600, 400))
        ], [sg.Text("Enter your message:")],
            [sg.InputText(key="message", expand_x=True)],
            [sg.Button("Send")]],
            expand_x=True,
            expand_y=True)
        groups = sg.Column([
            [
                sg.Text("Server Address"),
                sg.InputText(key="address", size=(20, 0))
            ],
            [sg.Text("Username"),
             sg.InputText(key="username", size=(20, 0))],
            [sg.Button("Connect")],
            [
                sg.Frame(title="Groups",
                         layout=[[
                             sg.Listbox(self.groups,
                                        expand_x=True,
                                        expand_y=True,
                                        no_scrollbar=True,
                                        enable_events=True,
                                        pad=5,
                                        key="groups")
                         ]],
                         expand_x=True,
                         expand_y=True)
            ],
        ],
            expand_x=True,
            expand_y=True)

        self.layout = [[
            sg.Pane([groups, messager],
                    orientation="horizontal",
                    show_handle=False,
                    border_width=0,
                    expand_y=True,
                    expand_x=True)
        ]]
        self.__start_gui()

    def __start_gui(self):
        # TODO group joining
        self.current_group = "main"

        self.window = sg.Window('Window Title', self.layout, resizable=True)
        self.client = Client(self.window.write_event_value)
        self.username = None
        while True:
            self.__event_loop()
        self.window.close()

    def __event_loop(self):
        event, values = self.window.read()
        if event == sg.WIN_CLOSED:
            return
        if event == "Send":
            message = values["message"]
            self.window["message"]("")
            if self.username and self.current_group:
                self.create_message(self.username, message)
            else:
                print("no username")
        if event == "Connect":
            self.username = values["username"]
            self.client.connect(values["address"])

    def create_message(self, username, message):
        self.window["messages"].print("",
                                      username,
                                      text_color=self.colors["TEXT_INPUT"],
                                      justification="c",
                                      background_color=self.colors["INPUT"],
                                      end="\n")
        if self.current_group in self.groups:
            self.groups[self.current_group].append((username, message))
        else:
            self.groups[self.current_group] = [(username, message)]
        self.window["messages"].print("",
                                      message,
                                      text_color=self.colors["TEXT"],
                                      end="\n\n")

    def listen(self, command=None):
        if command:
            Thread(target=getattr(client, command))
            return


if __name__ == "__main__":
    gui()
