{ pkgs ? import <nixpkgs> {} }:

let
    pwd = builtins.getEnv "PWD";
    env_secret_path = "${pwd}/.env.secret";

    cses_nick_pass = if builtins.pathExists env_secret_path then
        toString (
            map (x: "export " + toString x + ";") (
                builtins.split "\n" (
                    builtins.readFile env_secret_path
                )
            )
        )
    else 
        "";

    cses_warning = if builtins.pathExists env_secret_path then
        "''"
    else
        "'`.env.secret` not found. Create it with variables CSES_USERNAME and CSES_PASSWORD to let new_problem.py download test cases from CSES.\n'";
in
pkgs.mkShell {    
    nativeBuildInputs = with pkgs.buildPackages; [
        gcc14
        python312Full

        python312Packages.beautifulsoup4
        python312Packages.requests
    ];

    shellHook = ''
        export PATH=$PATH:${pwd};
        echo -n ${cses_warning};
        ${cses_nick_pass}
    '';
}