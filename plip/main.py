import os
import shutil
import pathlib
import glob
import subprocess
import sys

from utils import compile_program,run_program,move_files
from Lasso import runLasso


def compile():
    """ Function to compile C++ modules
    """
    compile_program('XMAT', ['mod_fonction.cpp', 'mod_tool.cpp', 'mod_XYZ.cpp', 'mod_xmat.cpp', 'doXMAT.cpp'], 'XMAT')
    compile_program('GEN_POT', ['mod_fonction.cpp', 'mod_tool.cpp', 'mod_XYZ.cpp', 'mod_xmat.cpp', 'doRead.cpp'], 'GENPOT')


def genBin():
    """Function to generate bin files
    """
    input_file="input.txt"
    if not os.path.exists(input_file):
        print(f"Error: The file '{input_file}' does not exist in the run directory.")
        raise SystemExit(1)
    run_program('XMAT')

def train(args):
    """Train Lasso lars model"""
    ref_forces='Input'
    ref_bins=f'Input_{args}'
    
    for directory in (ref_forces, ref_bins):
        if not os.path.isdir(directory):
            print(f"Error: The folder '{directory}' does not exist in the run directory.")
            sys.exit(1)
  
    runLasso(args)

def genPot():
    """Generate Potential """

    file_pattern = 'Coeff*'
    destination='POTS'
    try:
        os.mkdir(destination)
    except FileExistsError:
        raise RuntimeError("The 'POTS' directory already exists. Please delete or move it.")
    
    shutil.copy2('input.txt',destination)
    if not os.path.exists(destination+'GenEAM.py'):
        genpy_path=str(pathlib.Path(__file__).parent.resolve()) +'/cpp/GEN_POT/GenEAM.py'
        shutil.copy2(genpy_path,destination)
    
    matching_files = glob.glob(file_pattern)
    for file_path in matching_files:
        coefficient_dir =  file_path[:-4]
        shutil.copy2(file_path,destination)
        try:
            os.chdir(destination)
            os.mkdir(coefficient_dir)
        except FileExistsError:
            raise RuntimeError("The coefficient directory exists")
        run_program('GENPOT',[file_path])
        move_files('.', coefficient_dir, '*.fs', 'out*.txt', '*.sw')
        os.chdir('..')       