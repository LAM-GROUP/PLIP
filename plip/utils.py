import os
import sys
import glob
import shutil
import subprocess
from pathlib import Path


def compile_program(program_name, source_files, export_name):
    """ Function to compile files

    Args:
        program_name (_type_): _description_
        source_files (_type_): _description_
        export_name (_type_): _description_
    """
    try:
         # Store the current directory
        old_path = os.getcwd()
        script_directory = Path(__file__).parent.absolute()
        print( script_directory)
        os.chdir(str(script_directory) + f'/cpp/{program_name}')
    except FileNotFoundError as e:
        print(f"Error: {program_name} directory not found {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
    
    export_path= str(script_directory.parent.absolute())+f'/bin/{export_name}'
    comp = subprocess.run(['g++', '-std=c++11'] + source_files + ['-o', export_path, '-fopenmp'],
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if comp.returncode == 0:
        print(f'Compilation of {program_name} successful.')
    else:
        print(f'Compilation of {program_name} failed:')
        print(comp.stderr.decode('utf-8'))
        sys.exit(1)

    # Return to the original directory
    os.chdir(old_path)

def run_program(program_name,program_args=None, **kwargs):
    """_summary_

    Args:
        program_name (_type_): _description_
        program_args (_type_, optional): _description_. Defaults to None.
    """
    script_directory = Path(__file__).parent.parent.absolute()
    program_path = str(script_directory) + f'/bin/{program_name}'
    try:
        subprocess.run([program_path]+ program_args, stderr=subprocess.PIPE, check=True)
    except FileNotFoundError:
        print(f'{program_name} executable not found. Please compile to get {program_name} or make sure the {program_name} code is in the bin folder')
        sys.exit(1)

def move_files(source_dir, destination_dir, *file_patterns):
    """_summary_

    Args:
        source_dir (_type_): _description_
        destination_dir (_type_): _description_
    """
    for pattern in file_patterns:
        files = glob.glob(os.path.join(source_dir, pattern))
        for file in files:
            shutil.move(file, destination_dir)