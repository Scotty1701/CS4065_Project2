import PySimpleGUI as sg
from threading import Thread, Event
from .client import Client
from .client import event_list
from rich import print
from rich.panel import Panel


class gui:

    def __init__(self, theme):
        # set of groups, each containing a list of messages
        self.group_id = None

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
        ], [sg.Text("Subject")
            ], [
                sg.InputText(key="GUI_subject", expand_x=True),
        ], [sg.Text("Message")],
            [sg.InputText(key="GUI_content", expand_x=True)],
            [sg.Button("Send", key="GUI_post")]],
            expand_x=True,
            expand_y=True)
        groups = sg.Column([
            [
                sg.Text("Address"),
                sg.InputText("127.0.0.1", key="address", size=(20, 0)),
            ],
            [sg.Text("Port"),
             sg.InputText("42000", key="port", size=(20, 0))],
            [sg.Text("Username"),
             sg.InputText(key="username", size=(20, 0))],
            [sg.Button("Connect", key="GUI_connect")],
            [
                sg.Frame(title="Groups",
                         layout=[[
                             sg.Listbox([],
                                        expand_x=True,
                                        expand_y=True,
                                        no_scrollbar=True,
                                        enable_events=True,
                                        pad=5,
                                        key="GUI_join")
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
        self.write_event = self.window.write_event_value
        self.client = Client(self.write_event)
        self.username = None
        self.__event_loop()
        self.window.close()

    def __event_loop(self):
        while True:
            event, values = self.window.read()
            if event is not None:
                ogevent = event
                event = event.split("_")
            if event == sg.WIN_CLOSED:
                self.client.exit()
                return
            if event[0] == "GUI":
                if event[1] in self.client.command_list:
                    f = getattr(self, ogevent)
                    f(values)
            elif ogevent in event_list:
                f = getattr(self, ogevent)
                f(values[ogevent])

            # rint(event, values)

    def GUI_connect(self, values):
        address = values["address"]
        port = values["port"]

        def connect_and_ask_for_groups(address, port):
            self.client.connect(address, port)
            self.client.getgroups()
            self.write_event("GUI_join", ["0"])

        Thread(target=connect_and_ask_for_groups, args=(address, port)).start()

    def GUI_join(self, values):
        username = values["username"]
        if username == "":
            sg.popup_ok("you need to enter a username to join a group")
            return
        self.group_id = values["GUI_join"][0]
        self.window["GUI_join"].update(set_to_index=self.group_id)

        self.window["messages"].update("")
        Thread(target=self.client.join, args=(username, self.group_id)).start()

    def GUI_post(self, values):
        subject = values["GUI_subject"]
        content = values["GUI_content"]
        Thread(target=self.client.post, args=(subject, content)).start()

    def log(self, value):
        print(value)

    def join(self, value):
        print(value)

    def connect(self, value):
        print(value)

    def getgroups(self, value):
        groups = value["groups"]
        self.window["GUI_join"].update(groups)
        if self.group_id:
            self.window["GUI_join"].update(set_to_index=self.group_id)

    def post(self, value):
        print(value)

    def leave(self, value):
        print(value)

    def exit(self, value):
        print(value)
        print("server exited")

    def message(self, value):
        username, subject, content, id, date, group = value["sender"], value[
            "subject"], value["content"], value["message_id"], value[
                "post_date"], value["group_id"]
        self.window["messages"].print("",
                                      username,
                                      text_color=self.colors["TEXT_INPUT"],
                                      justification="c",
                                      background_color=self.colors["INPUT"],
                                      end="\n")
        self.window["messages"].print("",
                                      "Subject:" + subject,
                                      text_color=self.colors["TEXT_INPUT"],
                                      background_color=self.colors["INPUT"],
                                      end="\n")
        self.window["messages"].print("",
                                      content,
                                      text_color=self.colors["TEXT"],
                                      end="\n\n")
        self.window["GUI_content"].update("")
        self.window["GUI_subject"].update("")


if __name__ == "__main__":
    gui()
