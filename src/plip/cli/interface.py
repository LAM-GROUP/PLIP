#!/usr/bin/env python3
import argparse

from plip.main import compile, genBin, genPot, train


def interface():
    parser = argparse.ArgumentParser(
        prog="PLIP", description="Physical Lassolars interaction potentials"
    )

    # parser.add_argument("input.txt")

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


if __name__ == "__main__":
    interface()
