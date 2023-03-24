import click

@click.command()
@click.option("--connect", default="localhost", help="connect to address of message server")
def cli(connect):
    print(connect)


if __name__ == "__main__":
    cli()
