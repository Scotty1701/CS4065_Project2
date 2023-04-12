import rich_click as click
from .gui import gui


@click.command(
    help="Hi there! You can start the gui by not passing the --help flag!")
@click.option("--theme",
              default="DarkGreen2",
              help="colors got you down?" +
              "Change the theme to something you like better!" +
              "Pass 'preview' to this flag to view possible themes")
def cli(theme):
    gui(theme)
