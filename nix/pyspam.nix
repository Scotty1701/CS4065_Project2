{ inputs, pkgs, ... }:
let

  spam_api = with pkgs; stdenv.mkDerivation rec {
    name = "spam_api";
    src = ../Protocol;
    nativeBuildInputs = [ cmake ];
    cmakeFlags = [
      "-DFETCHCONTENT_SOURCE_DIR_JSON-CPP=${inputs.json-cpp}"
    ];
    configurePhase = ''
      mkdir build && cd build
      cmake ${(pkgs.lib.concatStrings cmakeFlags)} ..
    '';
    buildPhase = ''
      cmake --build . --config Release
    '';
    installPhase = ''
      cmake --install . --prefix $out
    '';
  };

  pyspam-files = with pkgs; stdenv.mkDerivation rec{
    name = "pyspam-files";
    src = ../Protocol;
    nativeBuildInputs = [ cmake spam_api python3 ];
    cmakeFlags = [
      "-DFETCHCONTENT_SOURCE_DIR_PYBIND11=${inputs.pybind11}"
    ];
    configurePhase = ''
      cp -r ${spam_api} install
      cd pyspam
      mkdir build && cd build
      cmake ${(pkgs.lib.concatStrings cmakeFlags)} ..
    '';
    buildPhase = ''
      cmake --build . --config Release
    '';
    installPhase = ''
      cmake --install . --prefix $out
      cp $src/pyproject.toml $out/pyproject.toml
    '';
  };
in
{
  pyspam = with pkgs.python3Packages; buildPythonPackage rec {
    name = "pyspam-1.0.0";
    src = "${pyspam-files}";
    format = "pyproject";
    propagatedBuildInputs = [ setuptools ];
  };

  server = with pkgs; stdenv.mkDerivation rec {
    name = "server";
    src = ../Server;
    nativeBuildInputs = [ cmake spam_api ];
    cmakeFlags = [
    ];
    configurePhase = ''
      mkdir ../Protocol
      cp -r ${spam_api} ../Protocol/install
      mkdir build && cd build
      cmake ${(pkgs.lib.concatStrings cmakeFlags)} ..
    '';
    buildPhase = ''
      cmake --build . --config Release
    '';
    installPhase = ''
      cmake --install . --prefix $out
      mv $out/bin/board_server $out/bin/server
    '';
  };
}

