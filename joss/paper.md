---
title: 'PLIP: A Python package for machine learning interatomic potentials'
tags:
  - Python
  - Chemistry
  - Material Science
  - Machine learning
  - force fields
  - MLIP
authors:
  - name: Akshay Krishna Ammothum Kandy
    orcid: 0000-0002-9880-4482
    equal-contrib: true
    affiliation: 1 # (Multiple affiliations must be quoted)
  - name: Julien Lam
    corresponding: true
    equal-contrib: true # (This is how you can denote equal contributions between multiple authors)
    affiliation: 1
affiliations:
  - name: Center for Materials Elaboration and Structural Studies, Toulouse, France
    index: 1
date: 13 April 2023
bibliography: paper.bib
---

# Summary
Molecular Dynamics (MD) simulations play a crucial role in comprehending and predicting the structure and properties of both existing and novel materials. These simulations involve studying the motion of atoms by integrating Newton's equation of motion for a group of atoms. However, the accuracy of the MD heavily relies on the interatomic potential (force field) employed, which has been a significant challenge to develop with high reliability and precision. In recent years, machine-learned interatomic potentials (MLIP) have gained popularity due to their superior accuracy. Nevertheless, MLIPs often lack physical interpretability, leading to them being perceived as "black boxes" that require careful fine-tuning.  An accurate and interpretable interatomic potential can lead to better insights into the behavior of materials and help design new materials with desired properties for various applications.

In this work, we have developed an alternative MLIP called Physical LassoLars Interaction Potential (PLIP) which combines a physically motivated mathematical formulation for the potential with a constrained linear regression algorithm. The primary goal of PLIP  package is to provide a transparent and interpretable machine-learning interaction potential while maintaining accuracy and efficiency.


# Statement of need
Machine-learning interaction potentials (MLIP) have recently been proposed as a means to bridge the gap between quantum accurate calculations and fast empirical modeling. The main principle behind MLIP involves utilizing a large set of quantum-accurate calculations to adjust parameters within a universal mathematical formulation representing the interaction potential. Various approaches have been suggested, including Artificial Neural Networks, Gaussian approximation potentials, Linearized potentials, Spectral Neighbor Analysis Potential, Symmetric Gradient Domain Machine learning, and Moment Tensor Potentials. These MLIP methods have shown success in studying a wide range of materials, including pure metals, organic molecules, water, amorphous materials, and hybrid perovskites. However, one key issue with current MLIP methods is they lack physical and chemical interpretability of the obtained potential.


PLIP is a Python package for constructing interpretable MLIP for atomic simulation of materials. The PLIP interaction potential is constructed by mapping the local atomic environment to a set of  translational, rotational and permutationally invariant descriptors. The detailed information about the construction of PLIP potential can be found [@Benoit2020Dec].








# Functionality
The PLIP software package can be primarily divided into three parts.  Firstly, the generation of machine learning finger print (descriptors) for structures provided in the data set. The PLIP package makes use of efficieint C++ routine to generate the ML descriptors, and this is achieved with the `XMAT` class. PLIP currently supports two-body, three-body and N-body descriptors. The basis functions for the descriptors can be chosen from functions like Gaussians, Lorentzian or asymmetric log-normal functions. The three-body descriptor formulation is very similar to the Stillinger–Weber potential [@Stillinger1985Apr] and N-body descriptors are generalized form of the embedded atom potential (EAM) [@Daw1984Jun] .

In the second part, the fitting procedure is performed using Lasso regression with the Least Angle Regression Scheme (together LassoLars). The PLIP package makes use of the efficient LassoLars implementation in scikit-learn python machine learning package to optimize the coeffecients. 

In the third and final part, the optimised coffecients are converted to LAMMPS compatable potentials. This is performed by the `GENPOT` routine written in C++. The two-body potentials are tabulated as spline tables. For three-body and N-body potentials a custom in-house made version of LAMMPS is required.  A general layout of the functioning of plip package is shown in \autoref{fig:plip}
![Interface for PLIP \label{fig:plip}](/images/plip.png)
# Funding




# References
