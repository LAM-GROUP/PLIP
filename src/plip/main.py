import glob
import os
import pathlib
import shutil
import sys

from plip.Lasso import runLasso
from plip.utils import compile_program, move_files, run_program


def compile():
    """Function to compile C++ modules"""
    compile_program(
        "XMAT",
        [
            "mod_fonction.cpp",
            "mod_tool.cpp",
            "mod_XYZ.cpp",
            "mod_xmat.cpp",
            "doXMAT.cpp",
        ],
        "XMAT",
    )
    compile_program(
        "GEN_POT",
        [
            "mod_fonction.cpp",
            "mod_tool.cpp",
            "mod_XYZ.cpp",
            "mod_xmat.cpp",
            "doRead.cpp",
        ],
        "GENPOT",
    )


def genBin():
    """Function to generate bin files"""
    input_file = "input.txt"
    if not os.path.exists(input_file):
        print(f"Error: The file '{input_file}' does not exist in the run directory.")
        raise SystemExit(1)
    run_program("XMAT")


def train(args):
    """Train LassoLars model

    Args:
        args (str): Type of descriptor
    """
    ref_forces = "Refs"
    input_dir = "Input"

    for directory in (ref_forces, input_dir):
        if not os.path.isdir(directory):
            print(
                f"Error: The folder '{directory}' does not exist in the run directory."
            )
            sys.exit(1)

    runLasso(args)


def genPot():
    """Generate PLIP potential"""

    file_pattern = "Coeff*"
    destination = "POTS"
    try:
        os.mkdir(destination)
    except FileExistsError:
        raise RuntimeError(
            "The 'POTS' directory already exists. Please delete or move it."
        )

    shutil.copy2("input.txt", destination)
    if not os.path.exists(destination + "GenEAM.py"):
        genpy_path = (
            str(pathlib.Path(__file__).parent.resolve()) + "/cpp/GEN_POT/GenEAM.py"
        )
        shutil.copy2(genpy_path, destination)

    matching_files = glob.glob(file_pattern)
    for file_path in matching_files:
        coefficient_dir = file_path[:-4]
        shutil.copy2(file_path, destination)
        try:
            os.chdir(destination)
            os.mkdir(coefficient_dir)
        except FileExistsError:
            raise RuntimeError("The coefficient directory exists")
        run_program("GENPOT", [file_path])
        move_files(".", coefficient_dir, "*.fs", "out*.txt", "*.sw")
        os.chdir("..")