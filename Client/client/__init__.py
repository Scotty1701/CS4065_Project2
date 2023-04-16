import rich_click as click
from .gui import gui as G
from .tui import tui as T

# @click.group()
# def cli():
#    pass
#
#
# @cli.command(help="You can start the cli by not passing the --help flag!")


@click.group()
@click.option('--debug/--no-debug', default=False)
def cli(debug):
    click.echo(f"Debug mode is {'on' if debug else 'off'}")


@cli.command()  # @cli, not @click!
def tui():
    click.echo('Syncing')
    c = T()
    c.run()
