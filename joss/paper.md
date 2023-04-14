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



# Mathematics


# Citations



# Figures


# Acknowledgements


# References
