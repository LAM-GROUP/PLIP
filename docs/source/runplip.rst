Run PLIP
========
Input for ML descriptors
------------------------
The ``input.txt`` file contains the parameters for generating descriptors.

.. code-block:: Bash

    Rcut=     6                     |DUMP= Input |RAA= 2.1        |RBB= 2.3       |RAB= 2.4



    #1:GAUSSIAN     3:SW    4:STO   6:GTO   7:Lorentz       8:Asssymetric lognormal
    |-----------------------|-------------------------------|-----------------------|-----------------------|-------------------------------|-------------------------------|-------------------------------|
    |   ~C~X~T~T~A~T~       |       0=False 1=True          |PARAM  :       MIN     MAX     STEP                                            |                               |                               |
    |-----------------------|-------------------------------|-----------------------|-----------------------|-------------------------------|-------------------------------|-------------------------------|
    |       i_nature        |       INCLUDED                |       PARAMETER 1     |       PARAMETER 2     |               2B              |               3B              |               NB              |
    |-----------------------|-------------------------------|-----------------------|-----------------------|-------------------------------|-------------------------------|-------------------------------|
    |       1            |               1               |   0.5     1.5   1.0   |  0.5      6.0      0.5  |               1                |               0               |               0               |

The details on the keywords can be found in the :ref:`yaml`. 

CLI
---
The ``plip`` package offers a command line interface (CLI) and yaml interface. The CLI interface is as follows:

.. code-block:: Bash

    plip_fit -h
    
    usage: PLIP [-h] [--yaml {input.yaml}] [--compile] [--genbin {input.txt}] [--train {1}] [--alpha ALPHA] [--genpot] [--version]

    Physical Lassolars interaction potentials

    optional arguments:
    -h, --help            show this help message and exit
    --yaml {input.yaml}, -i {input.yaml}
                            PLIP workflow
    --compile, -c         Compile C++ modules
    --genbin {input.txt}, -g {input.txt}
                             Generate binary files
    --train {1}, -t {1}      Run the lassolars
    --alpha ALPHA, -a ALPHA  Value of alpha
    --genpot, -p          Generate potential for all coefficient files
    --version, -v         version of the package

.. _yaml:

YAML workflow
-------------
The ``YAML`` workflow is the preferred way to generate the plip potentials. The usage is as follows::

    plip_fit -i input.yaml

The ``YAML`` file must contain the following five sections :

.. code-block:: yaml

    Input:                                
        # Parameters to generate ML descriptors.
        "Rcut": float,                    # Cut-off distance
        "DUMP": str,                      # Folder location of structure files
        "RAA": float,                     # Nbody cut-off
        "RBB": float,                     # Nbody cut-off
        "RAB": float,                     # Nbody cut-off
        "i_nature": int,                  # Descriptor  1- gaussian
        "included": int,
        "a": {                            # Width parameter gaussian
            "MIN": float, 
            "MAX": float, 
            "STEP": float},  
        "b": {                            # Position parameter gaussian
            "MIN": float, 
            "MAX": float, 
            "STEP": float},   
        "2B": int,                        # Generate Twobody descriptor (value = 1)
        "3B": int,                        # Generate threebody descriptor(if present value=5)
        "NB": int,                        # Generate threebody descriptor(if present value=7)
    
    compile: bool
        # Compile the C++ modules

    genBin: bool
        # Generate the descriptor files with *.bin format

    train:
        # Train the LassoLARS model. 
        i_nature: int                      # Type of decriptor 
        alpha: float                       # Regularization parameter in LAssolARS
        
    genPot: bool
        # Generate LAMMPS potential 



The ``input.txt`` file is automatically generated from the ``Input`` block in yaml file. An example for the yaml file can be found in the ``example`` folder.

