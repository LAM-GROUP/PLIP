#!/usr/bin/env python3
import argparse

from plip.main import compile, genBin, genPot, train, xmat_input, yaml_reader
from importlib.metadata import version


def interface():
    parser = argparse.ArgumentParser(
        prog="PLIP", description="Physical Lassolars interaction potentials"
    )

    # parser.add_argument("input.txt")
    # yaml
    parser.add_argument(
        "--yaml", "-i", type=str, choices=["input.yaml"], help="PLIP workflow"
    )

    # XMAT and GENPOT compilation option
    parser.add_argument(
        "--compile", "-c", action="store_true", help="Compile C++ modules"
    )

    # Generate  bin files
    parser.add_argument(
        "--genbin", "-g", type=str, choices=["input.txt"], help="Generate binary files"
    )

    # train a model
    parser.add_argument(
        "--train", "-t", type=str, choices=["1"], help="Run the lassolars"
    )

    # genpot
    parser.add_argument(
        "--genpot",
        "-p",
        action="store_true",
        help="Generate potential for all coefficient files",
    )

    parser.add_argument(
        "--version",
        "-v",
        action="store_true",
        help="version of the package",
    )

    # Parse the arguments
    args = parser.parse_args()
    print(args)

    # Check value of flag

    if args.compile:
        compile()
    if args.genbin:
        genBin()
    if args.train:
        train(args.train)
    if args.genpot:
        genPot()
    if args.version:
        print(version("plip"))
    if  args.yaml:
        yaml_reader(args.yaml)



if __name__ == "__main__":
    interface()
