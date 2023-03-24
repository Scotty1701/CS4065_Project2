{
  description = "";
  inputs = { flake-utils.url = "github:numtide/flake-utils"; };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        python-with-my-packages = pkgs.python3.withPackages (p: with p; [
          click
          setuptools
          pysimplegui
          autopep8
          python-lsp-server
        ]);
      in
      rec {
        devShells = flake-utils.lib.flattenTree {
          default = with pkgs; mkShell {
            buildInputs = [ python-with-my-packages ];
          };
        };
      }
    );
}
