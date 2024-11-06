{ self, lib, inputs }: {
  default = final: prev: rec {
    barcalc = final.callPackage ./default.nix { };
    barcalc-debug = barcalc.override { debug = true; };
  };
}
