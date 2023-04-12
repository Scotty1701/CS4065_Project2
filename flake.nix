{
  description = "";
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    json-cpp = {
      url = "github:Scotty1701/jsoncpp";
      flake = false;
    };
    pybind11 = {
      url = "github:pybind/pybind11";
      flake = false;
    };
  };
  outputs = { self, nixpkgs, flake-utils, json-cpp, pybind11 }@inputs:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        mkpython = extra_packages: pkgs.python3.withPackages (p: with p; [
          rich-click
          setuptools
          pysimplegui
          autopep8
          python-lsp-server
        ] ++ extra_packages);
      in
      rec {

        packages =
          let
            pyspamDerivation = import ./nix/pyspam.nix { inherit inputs; inherit pkgs; };
          in
          flake-utils.lib.flattenTree
            {
              pyspam = pyspamDerivation;
              client = with pkgs.python3.pkgs; buildPythonPackage {
                name = "client-1.0.0";
                src = ./Client;
                format = "pyproject";
                propagatedBuildInputs = [ setuptools packages.pyspam rich-click pysimplegui ];
              };

            };
        devShells = flake-utils.lib.flattenTree {
          default = with pkgs; mkShell {
            packages = [ (mkpython [ packages.pyspam packages.client ]) ];
          };
        };
      }
    );
}
