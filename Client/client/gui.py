import PySimpleGUI as sg
from threading import Thread, Event


class gui:

    def __init__(self, theme):
        # set of groups, each containing a list of messages
        groups = {}

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
                sg.Frame(
                    title="Groups", layout=[[]], expand_x=True, expand_y=True)
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
        self.window = sg.Window('Window Title', self.layout, resizable=True)
        self.username = None
        while True:
            # event loop
            event, values = self.window.read()
            if event == sg.WIN_CLOSED:
                break
            if event == "Send":
                message = values["message"]
                self.window["message"]("")
                if self.username:
                    self.create_message(self.username, message)
                else:
                    print("no username")
            if event == "Connect":
                self.username = values["username"]
        self.window.close()

    def create_message(self, username, message):
        self.window["messages"].print(
            "",
            username,
            text_color=self.colors["TEXT"],
            justification="c",
            background_color=self.colors["TEXT_INPUT"],
            end="\n")
        self.window["messages"].print("",
                                      message,
                                      text_color=self.colors["TEXT"],
                                      end="\n\n")


if __name__ == "__main__":
    gui()
