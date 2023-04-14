![](logo.png)

`PLIP` (Physical LassoLars Interactions Potential) is a Python package for machine learning interatomic potentials for materials modelling. Physical Lasso-Lars Interaction Potential (PLIP) has been developed to address the lack of interpretability of MLIPs. PLIP uses a physically motivated mathematical formulation for the potential and a constrained linear regression to fit the linear coefficients that determine the preponderance of each contribution to the potential. The MLIP is constructed as a linear combination of classical interactions, including two-body, three-body, and many-body interactions. These interactions are made up of simple functions such as Lennard-Jones, Gaussian, and Slatter-type orbitals, as well as explicit angular functions and embedded atom-like interactions. The Lasso-Lars regression used in PLIP imposes most coefficients to be equal to 0, resulting in a simplified final expression that retains only the main physical nature of the interactions.

# Short description of PLIP code

 The method consists of 3 steps:

* **XMAT** 

    Construct matrices with each row corresponding to a force (X,Y,Z) on a specific atom and each column corresponding to a descriptor. The input arguments required are:

  1. The file **input.txt** that summarizes the list of descriptors. 
 
  2. **Input/** folder with all the input files for the database  
 
* **Lasso**

    Read the matrices and the DFT forces to construct the PLIP using LassoLars. For that purpose, the "lammpsstrj" (forces) and the "bin" (descriptor) files must be moved respectively to "Input/" and "Input_1/" and one needs to run "./xLasso". We test different values for alpha and results are moved in a folder "Output_1/"

* **GenPot**

    Read the obtained linear coefficients and construct the LAMMPS file for the potential. The "Coeff???.txt" file that is given by "2_Lasso" must be moved in the root of the folder and the code is the run with "./xGen"

Once the potential is obtained, one can start running LAMMPS simulations. For that purpose, the files obtained with GenPot are moved in "4_MD/zPOT".



# Documentation
The full documentation is available here.

# Authors
The PLIP package was  developed by Julien Lam, Akshay Krishna AK, Aurélien Roux

# Citing
```
@article{benoit2020measuring,
  title={Measuring transferability issues in machine-learning force fields: the example of gold--iron interactions with linearized potentials},
  author={Benoit, Magali and Amodeo, Jonathan and Combettes, S{\'e}gol{\`e}ne and Khaled, Ibrahim and Roux, Aur{\'e}lien and Lam, Julien},
  journal={Machine Learning: Science and Technology},
  volume={2},
  number={2},
  pages={025003},
  year={2020},
  publisher={IOP Publishing}
}
```
# License
The PLIP package  is distributed under terms of [GNU GPLv3](https://github.com/LAM-GROUP/PLIP/LICENSE).