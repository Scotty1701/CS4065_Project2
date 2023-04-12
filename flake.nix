{
  description = "";
  inputs = { flake-utils.url = "github:numtide/flake-utils";
             json-cpp = {
               url = "github:Scotty1701/jsoncpp";
               flake = false;
             };
             pybind11 = {
               url = "github:pybind/pybind11";
               flake = false;
             };
           };
  outputs = { self, nixpkgs, flake-utils, json-cpp, pybind11 }:
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

        packages = flake-utils.lib.flattenTree rec {
          default = with pkgs; stdenv.mkDerivation rec{
            name = "pyspam";
            src = ./Protocol;
            nativeBuildInputs = [cmake jsoncpp python3];
            cmakeFlags = [
              "-DFETCHCONTENT_SOURCE_DIR_JSON-CPP=${json-cpp}"
              " -DFETCHCONTENT_SOURCE_DIR_PYBIND11=${pybind11}"
            ];
            configurePhase = ''
              mkdir build && cd build
              cmake ${(pkgs.lib.concatStrings cmakeFlags)} ..
              cmake --build . --config Release
              cmake --install . --prefix ../install
              cd ../python
              mkdir build && cd build
              cmake ${(pkgs.lib.concatStrings cmakeFlags)} ..
              cmake --build . --config Release
              cmake --install . --prefix ../install
            '';
            buildPhase = ''
            '';
            installPhase = ''
              mkdir -p $out
              cp -r ../../install $out/
              mkdir $out/python
              cp -r ../install $out/python/
            '';
          };
        };
        devShells = flake-utils.lib.flattenTree {
          default = with pkgs; mkShell {
            buildInputs = [ python-with-my-packages ];
          };
        };
      }
    );
}
