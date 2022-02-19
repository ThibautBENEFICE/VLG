# VLG rattrapages S8 2022 - Thibaut BENEFICE

How to build the program:
    commands: 
        simple release build:
            ./run.sh
        simplified testsuite + leak_checks + debugmode:
            ./run_tests.sh

warning: some include locations may differ depending on where you installed
igraph. Please adapt path values in CMakelists.txt for include_directories
and link_directories.
