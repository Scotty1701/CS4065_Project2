{ inputs, pkgs, ... }:
let

  jsoncpp = with pkgs; stdenv.mkDerivation rec {
    name = "jsoncpp";
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
    nativeBuildInputs = [ cmake jsoncpp python3 ];
    cmakeFlags = [
      "-DFETCHCONTENT_SOURCE_DIR_PYBIND11=${inputs.pybind11}"
    ];
    configurePhase = ''
      cp -r ${jsoncpp} install
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
with pkgs.python3Packages;
buildPythonPackage rec {
  name = "pyspam-1.0.0";
  src = "${pyspam-files}";
  format = "pyproject";
  propagatedBuildInputs = [ setuptools ];
}

