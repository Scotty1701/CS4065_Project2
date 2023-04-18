""" This is the click interface, it handles how to start the cli/gui """
import rich_click as click
from .gui import gui as G
from .cli import CLI
import signal


@click.group()
def cli():
    pass


@cli.command(help="Start the Graphical user interface!")
@click.option(
    '--theme',
    default="Dark Green",
    help="Set the theme!, view possible themes with preview, set theme with quotes around name"
)
@click.option('--debug',
              is_flag=True,
              default=False,
              help="print raw server messages")
def gui(theme, debug):
    g = G(theme, debug)


@cli.command(name="cli", help="Start the command line interface!")
@click.option('--debug',
              is_flag=True,
              default=False,
              help="print raw server messages")
def C(debug):
    c = CLI(debug)
