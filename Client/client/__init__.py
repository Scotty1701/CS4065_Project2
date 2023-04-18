import rich_click as click
from .gui import gui as G
from .cli import CLI

# @click.group()
# def cli():
#    pass
#
#
# @cli.command(help="You can start the cli by not passing the --help flag!")


@click.group()
def cli():
    pass


@cli.command()
@click.option('--theme', default="Dark Green")
def gui(theme):
    g = G(theme)


@cli.command(name="cli")
def C():
    c = CLI()
