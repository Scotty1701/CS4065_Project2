import rich_click as click
from .gui import gui as G
from .cli import CLI
import signal


@click.group()
def cli():
    pass


@cli.command(help="Start the Graphical user interface!")
@click.option('--theme', default="Dark Green", help="Set the theme!")
def gui(theme):
    g = G(theme)


@cli.command(name="cli", help="Start the command line interface!")
def C():
    c = CLI()
