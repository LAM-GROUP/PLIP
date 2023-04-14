# Short description of PLIP code

This section contains a brief description on the usage of PLIP code as of 13 January 2022. The method consists of 3 steps:


* **XMAT** 

    Construct matrices with each row corresponding to a force (X,Y,Z) on a specific atom and each column corresponding to a descriptor. The input arguments required are:

  1. The file **input.txt** that summarizes the list of descriptors. 
 
  2. **Input/** folder with all the input files for the database  
 
* **Lasso**

    Read the matrices and the DFT forces to construct the PLIP using LassoLars. For that purpose, the "lammpsstrj" (forces) and the "bin" (descriptor) files must be moved respectively to "Input/" and "Input_1/" and one needs to run "./xLasso". We test different values for alpha and results are moved in a folder "Output_1/"

* **GenPot**

    Read the obtained linear coefficients and construct the LAMMPS file for the potential. The "Coeff???.txt" file that is given by "2_Lasso" must be moved in the root of the folder and the code is the run with "./xGen"

Once the potential is obtained, one can start running LAMMPS simulations. For that purpose, the files obtained with GenPot are moved in "4_MD/zPOT".

For details on the methodology, please refer to this paper with a particular emphasis on the appendix:
<https://iopscience.iop.org/article/10.1088/2632-2153/abc9fd>


