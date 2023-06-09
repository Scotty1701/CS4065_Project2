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
          textual
        ] ++ extra_packages);
      in
      rec {

        packages =
          let
            pyspamDerivation = import ./nix/pyspam.nix { inherit inputs; inherit pkgs; };
          in
          flake-utils.lib.flattenTree
            {
              pyspam = pyspamDerivation.pyspam;
              server = pyspamDerivation.server;
              client = with pkgs.python3.pkgs; buildPythonPackage {
                name = "client-1.0.0";
                src = ./Client;
                format = "pyproject";
                propagatedBuildInputs = [ setuptools packages.pyspam rich-click pysimplegui kivy rich getkey ];
              };
              docker = with pkgs; dockerTools.buildImage {
                name = "CSProject";
                tag = "latest";

                copyToRoot = buildEnv {
                  name = "image-root";
                  paths = [ packages.client packages.server bashInteractive coreutils less ];
                  pathsToLink = [ "/bin" ];
                };
                config = {
                  Cmd = [ "${bashInteractive}/bin/bash" ];
                };
              };

            };
        apps = {
          client = with pkgs; {
            type = "app";
            program = "${packages.client}/bin/client";
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
