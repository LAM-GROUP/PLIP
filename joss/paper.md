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
PLIP is a Python package for constructing interpretable MLIP for atomic simulation of materials. The PLIP interaction potential is constructed by mapping the local atomic environment to a set of  translational, rotational and permutationally invariant descriptors. The detailed iinformation about construction of PLIP can be found [@Benoit2020Dec]

Machine-learning interaction potentials (MLIP) have recently been proposed as a means to bridge the gap between quantum accurate calculations and fast empirical modeling. The main principle behind MLIP involves utilizing a large set of quantum-accurate calculations to adjust parameters within a universal mathematical formulation representing the interaction potential. Various approaches have been suggested, including Artificial Neural Networks, Gaussian approximation potentials, Linearized potentials, Spectral Neighbor Analysis Potential, Symmetric Gradient Domain Machine learning, and Moment Tensor Potentials. These MLIP methods have shown success in studying a wide range of materials, including pure metals, organic molecules, water, amorphous materials, and hybrid perovskites. However, one key issue with current MLIP methods is their designation as "black boxes," lacking physical and chemical interpretability of the obtained potential.




# Components

# Citations



# Figures


# Acknowledgements


# References
