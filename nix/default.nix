{ stdenv
, lib
, meson
, ninja
, pkg-config
, gtk4
, libsigcxx
, glibmm
, cairomm
, pangomm
, gtkmm4
, unixtools

, debug ? false
}:
let
  version = with lib; elemAt
    (pipe (readFile ../meson.build) [
      (splitString "\n")
      (filter (hasPrefix "  version : "))
      head
      (splitString " : ")
      last
      (splitString "'")
    ]) 1;
in
stdenv.mkDerivation {
  pname = "barcalc";
  inherit version;
  src = lib.cleanSource ./..;

  nativeBuildInputs = [
    meson
    ninja
    pkg-config
    gtkmm4
    libsigcxx
    glibmm
    cairomm
    pangomm
    unixtools.xxd
  ];

  buildInputs = [
    gtk4
  ];

  mesonBuildType = if debug then "debug" else "release";
}
