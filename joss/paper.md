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
   index: 1
 - name: Center for Materials Elaboration and Structural Studies, Toulouse, France
   

date: 13 April 2023
bibliography: paper.bib

# Optional fields if submitting to a AAS journal too, see this blog post:
# https://blog.joss.theoj.org/2018/12/a-new-collaboration-with-aas-publishing
#aas-doi: 10.3847/xxxxx <- update this with the DOI from AAS once you know it.
#aas-journal: Astrophysical Journal <- The name of the AAS journal.
---

# Summary
PLIP (Physical LassoLars Interactions Potential) is a Python package for machine learning interatomic potentials for materials modelling. Physical Lasso-Lars Interaction Potential (PLIP) has been developed to address the lack of interpretability of MLIPs. PLIP uses a physically motivated mathematical formulation for the potential and a constrained linear regression to fit the linear coefficients that determine the preponderance of each contribution to the potential. The MLIP is constructed as a linear combination of classical interactions, including two-body, three-body, and many-body interactions. These interactions are made up of simple functions such as Lennard-Jones, Gaussian, and Slatter-type orbitals, as well as explicit angular functions and embedded atom-like interactions. The Lasso-Lars regression used in PLIP imposes most coefficients to be equal to 0, resulting in a simplified final expression that retains only the main physical nature of the interactions.


# Statement of need
Ab initio molecular simulations, rooted in quantum mechanics, have revolutionized the chemical and materials sciences by providing atomistic and electronic-level insights. These simulations have become increasingly popular with the advancement of computational power, including the development of exascale computing. However, certain systems and topics are still beyond the reach of pure quantum mechanical calculations due to their size or computational demands. To overcome these limitations, data-driven techniques, such as machine-learned interatomic potentials, have emerged. These techniques enable efficient exploration of computationally demanding areas while maintaining the accuracy of ab initio methods. Generating accurate potential energy surfaces (PES) is a key aspect of particle-based molecular and materials simulations, and high-quality PESs are typically obtained through costly quantum mechanical calculations. The computational cost increases exponentially with the complexity of the system, making large-scale simulations impractical. 

Machine-learned interatomic potentials offer a solution by learning the relationship between atomic/molecular features and accurate training data, allowing for interpolation and extrapolation of interaction energies with near ab initio accuracy and significantly reduced computational cost. 


# Mathematics
For Ei, we considered a weighted linear combination of descriptors indexed by n:

Equation (1)
<img src="https://latex.codecogs.com/png.latex?E_{i}%20=%20\sum_{n}%20\omega_{n}%20X_{n}^{(i)}" title="E_{i} = \sum_{n} \omega_{n} X_{n}^{(i)}" />

Until now, moment tensors [16], group-theoretical high-order rotational invariants [52], and bispectrum components [11, 12, 53] were previously proposed as descriptors for such linearized potentials. In this work, we favored a simpler formulation which consists of developing the descriptor space in explicit two-body, three-body, and N-body interactions:

Equation (2)
<img src="https://latex.codecogs.com/png.latex?X_{2B}^{(i)}%20=%20\sum_{j\neq%20i}%20f_c(R_{ij})" title="X_{2B}^{(i)} = \sum_{j\neq i} f_c(R_{ij})" />

Equation (3)
<img src="https://latex.codecogs.com/png.latex?X_{3B}^{(i)}%20=%20\sum_{j\neq%20i}\sum_{k\neq%20i,j}%20f_c(R_{ij})f_c(R_{ik})f_c(R_{jk})\cos(\theta_{ijk})" title="X_{3B}^{(i)} = \sum_{j\neq i}\sum_{k\neq i,j} f_c(R_{ij})f_c(R_{ik})f_c(R_{jk})\cos(\theta_{ijk})" />

Equation (4)
<img src="https://latex.codecogs.com/png.latex?X_{NB}^{(i)}%20=%20\sum_{j_1\neq%20i}\sum_{j_2\neq%20i,j_1}\ldots\sum_{j_{N-1}\neq%20i,j_1,\ldots,j_{N-2}}%20f_c(R_{ij_1})\ldots%20f_c(R_{ij_{N-1}})" title="X_{NB}^{(i)} = \sum_{j_1\neq i}\sum_{j_2\neq i,j_1}\ldots\sum_{j_{N-1}\neq i,j_1,\ldots,j_{N-2}} f_c(R_{ij_1})\ldots f_c(R_{ij_{N-1}})" />

# Citations



# Figures


# Acknowledgements


# References
