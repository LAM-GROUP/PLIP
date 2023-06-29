[![Continuous Integration](https://github.com/LAM-GROUP/PLIP/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/LAM-GROUP/PLIP/actions/workflows/ci-cd.yml)
[![Documentation Status](https://readthedocs.org/projects/plip/badge/?version=latest)](https://plip.readthedocs.io/en/latest/?badge=latest)

`PLIP` (Physical LassoLars Interactions Potential) is a Python package for machine learning interatomic potentials for materials modelling. Physical Lasso-Lars Interaction Potential (PLIP) has been developed to address the lack of interpretability of MLIPs. PLIP uses a physically motivated mathematical formulation for the potential and a constrained linear regression. The MLIP is constructed as a linear combination of classical interactions, including two-body, three-body, and many-body interactions. These interactions are made up of simple functions such as Lennard-Jones, Gaussian, and Slatter-type orbitals, as well as explicit angular functions and embedded atom-like interactions. 
# Installation
```
git clone https://github.com/LAM-GROUP/PLIP.git
cd  plip
pip install -e .
```
# Documentation
The full documentation is available at [plip.readthedocs.io](plip.readthedocs.io).

# Authors
The PLIP package was  developed by Julien Lam, Akshay Krishna AK, Aurélien Roux

# License
The PLIP package  is distributed under terms of [GNU GPLv3](https://github.com/LAM-GROUP/PLIP/LICENSE).

