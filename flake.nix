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
          packages.default = pkgs.stdenv.mkDerivation rec {
            name = "dijkstra";
            version = "1.0";

            src = ./.;
            nativeBuildInputs = with pkgs; [
              SDL2
              SDL
              SDL2_gfx
              SDL2_mixer
              SDL_ttf
            ];


            buildPhase = ''
              make
            '';

            installPhase = ''
            '';
          };

          devShells.default = pkgs.mkShell {
            inputsFrom = [
              config.treefmt.build.devShell
            ];
            nativeBuildInputs = with pkgs; [
              SDL2
              SDL
                            SDL2_gfx

              SDL2_mixer
              SDL_ttf
            ];
          };

          # Add your auto-formatters here.
          # cf. https://numtide.github.io/treefmt/
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
