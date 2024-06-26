{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    systems.url = "github:nix-systems/default";

    # Dev tools
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs = inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import inputs.systems;
      imports = [
        inputs.treefmt-nix.flakeModule
      ];
      perSystem =
        { config
        , self'
        , pkgs
        , lib
        , system
        , ...
        }:
        {
          packages.default = pkgs.stdenv.mkDerivation {
            name = "dijkstra";
            version = "1.0";

            src = ./.;
            nativeBuildInputs = with pkgs; [
              gccStdenv
            ];

            buildInputs = with pkgs; [
              SDL2
              SDL2_ttf
              SDL2_gfx
            ];


            buildPhase = ''
              make dijkstra
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp dijkstra $out/bin/
            '';
          };




          devShells.default = pkgs.mkShell {
            inputsFrom = [
              config.treefmt.build.devShell
            ];

            packages = with pkgs; [
              SDL2
              SDL2_ttf
              SDL2_gfx
              gdb
              valgrind
            ];
          };


          treefmt.config = {
            projectRootFile = "flake.nix";
            programs = {
              nixpkgs-fmt.enable = true;
              clang-format.enable = true;
            };
          };
        };
    };
}
