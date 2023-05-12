#!/usr/bin/env python3
import argparse
from importlib import import_module

def main():

    parser = argparse.ArgumentParser(
                        prog='PLIP',
                        description='Physical Lassolars interaction potentials'
                        )

    parser.add_argument("input.txt")

    #XMAT compilation option
    parser.add_argument('--compile', '-c',action='store_true',help='compilation of XMAT code')

    # Parse the arguments
    args = parser.parse_args()
    print(args)

    # Check value of flag

    if args.compile:
        print('h')


if __name__ == "__main__":
    main()
