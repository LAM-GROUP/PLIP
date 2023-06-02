import glob
import os
import pathlib
import shutil
import sys
import yaml
from yaml.loader import SafeLoader

from plip.Lasso import runLasso
from plip.utils import compile_program, move_files, run_program




def xmat_input(config):
    content = f"\nRcut=     {config['Rcut']}                     |DUMP= {config['DUMP']} |RAA= {config['RAA']}        |RBB= {config['RBB']}       |RAB= {config['RAB']}\n\n\n\n"
    content += "#1:GAUSSIAN     3:SW    4:STO   6:GTO   7:Lorentz       8:Asssymetric lognormal\n"
    content += "|-----------------------|-------------------------------|-----------------------|-----------------------|-------------------------------|-------------------------------|-------------------------------|\n"
    content += "|   ~C~X~T~T~A~T~   |       0=False 1=True          |PARAM  :       MIN     MAX     STEP                                            |                               |                               |\n"
    content += "|-----------------------|-------------------------------|-----------------------|-----------------------|-------------------------------|-------------------------------|-------------------------------|\n"
    content += f"|       i_nature        |       INCLUDED                |       PARAMETER 1     |       PARAMETER 2     |               2B              |               3B              |               NB              |\n"
    content += "|-----------------------|-------------------------------|-----------------------|-----------------------|-------------------------------|-------------------------------|-------------------------------|\n"
    content += f"|       {config['i_nature']}            |               {config['included']}               |   {config['a']['MIN']}     {config['a']['MAX']}   {config['a']['STEP']}   |  {config['b']['MIN']}      {config['b']['MAX']}      {config['b']['STEP']}  |               {config['2B']}               |               {config['3B']}               |               {config['NB']}               |\n"

# Write the content to input.txt
    with open("input.txt", "w") as file:
        file.write(content)

    print("input.txt file has been generated.")

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

def yaml_reader(filename):
 #   filename="input.yaml"
    with open(filename) as f:
        data = yaml.load(f, Loader=SafeLoader)
        print(data)
    if data['Input']:
        xmat_input(data['Input'])
    if data['compile']:
        compile()
    if data['genBin']:
        genBin()
    if data['train']:
        runLasso(data['train']['i_nature'])
    if data['genPot']:
        pass