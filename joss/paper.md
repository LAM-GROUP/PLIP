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
Molecular Dynamics (MD) simulations play a crucial role in understanding and predicting the structure and properties materials. These simulations involve studying the motion of atoms by integrating Newton's equation of motion for a group of atoms. However, the accuracy of the MD heavily relies on the interatomic potential (force field) employed, which has been a significant challenge to develop with high reliability and precision. In recent years, machine-learned interatomic potentials (MLIP) have gained popularity due to their superior accuracy. The main principle behind MLIP involves utilizing a large set of quantum-accurate calculations to adjust parameters within a universal mathematical formulation representing the interaction potential. Despite their high accuracy, MLIP's   often lack physical interpretability, rendering them  as "black boxes" requiring careful fine-tuning. 

In this work, we have developed an alternative MLIP called Physical LassoLars Interaction Potential (PLIP) which combines a physically motivated mathematical formulation for the potential, optimised with a constrained linear regression algorithm. The primary goal of PLIP  package is to provide a transparent and interpretable machine-learning interaction potential while maintaining accuracy and efficiency. The development of an accurate and interpretable interatomic potential is highly useful, as it can provide deeper insights into material behaviors and facilitate the design of new materials with desired properties for diverse applications.

# Statement of need
PLIP is a python/C++ package for constructing interpretable MLIP for atomic simulation of materials. The construction of the PLIP interaction potential involves several key steps. First, the local atomic environment is represented using a combination of translational, rotational, and permutationally invariant descriptors. These descriptors encode crucial information about the positions and orientations of neighboring atoms around a central atom. Different models for descriptors have been proposed in the literature, such as moment tensors[@Novoselov2019Jun], atom-centered symmetry functions (ACSFs)[@Behler2011Feb], smooth overlap of atomic positions (SOAP)[@Bartok2013May], bispectrum components[@Bartok2013May,@Thompson2015Mar], among others. Among these alternatives, PLIP descriptors stand out for their simplicity, as they involve explicit two-body, three-body, and N-body interactions. A detailed comparison of transferability and accuracy of  PLIP descriptors and ACSFs can be found in [@Kandy2023May].

Once the descriptor space is defined, the next step is to optimize it effectively. For this purpose, PLIP employs a Lasso regression technique with the Least Angle Regression (LARS) algorithm. Lasso regression helps in selecting important descriptors and effectively reducing the complexity of the model by setting irrelevant descriptor coefficients to zero, making the final MLIP more interpretable. 




<!--
Machine-learning interaction potentials (MLIP) have recently been proposed as a means to bridge the gap between quantum accurate calculations and fast empirical modeling.  Various approaches have been suggested, including Artificial Neural Networks, Gaussian approximation potentials, Linearized potentials, Spectral Neighbor Analysis Potential, Symmetric Gradient Domain Machine learning, and Moment Tensor Potentials. These MLIP methods have shown success in studying a wide range of materials. However, one key issue with current MLIP methods is they lack physical and chemical interpretability for the obtained potential.
-->











# Functionality
The PLIP software package can be primarily divided into three parts.  Firstly, the generation of machine learning finger print (descriptors) for structures provided in the data set. The PLIP package makes use of efficieint C++ routine to generate the ML descriptors, and this is achieved with the `XMAT` class. PLIP currently supports two-body, three-body and N-body descriptors. The basis functions for the descriptors can be chosen from functions like Gaussians, Lorentzian or asymmetric log-normal functions. The three-body descriptor formulation is very similar to the Stillinger–Weber potential [@Stillinger1985Apr] and N-body descriptors are generalized form of the embedded atom potential (EAM) [@Daw1984Jun] .

In the second part, the fitting procedure is performed using Lasso regression with the Least Angle Regression Scheme (together LassoLars). The PLIP package makes use of the efficient LassoLars implementation in scikit-learn python machine learning package to optimize the coeffecients. 

In the third and final part, the optimised coffecients are converted to LAMMPS compatable potentials. This is performed by the `GENPOT` routine written in C++. The two-body potentials are tabulated as spline tables. For three-body and N-body potentials a custom in-house made version of LAMMPS is required.  A general layout of the functioning of plip package is shown in \autoref{fig:plip}
![Interface for PLIP \label{fig:plip}](/images/plip.png)
# Funding
This project has been funded in part by the ANR under the grant agreement ANR-21-CE09-0006



# References
