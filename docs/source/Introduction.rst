Introduction
============
Overview of PLIP
-------------------

The Physical LassoLars Interaction Potential
(PLIP) is a python package for generating machine learning interatomic potetnials (MLIP) for atomistic simulations.
A key issue related to the current MLIP methods that are often designated as "black boxes" is the lack of physical
and chemical interpretability of the obtained potential. The PLIP method combines a physically motivated mathematical formulation for the
potential `PLIP descriptors <plip_>`_ and a constrained linear regression.

The resulting machine learned models therefore predict interatomic interaction forces as a 
function of these geometric descriptors. Once these models are obtained from PLIP, they may be
used directly in `LAMMPS <lammps_>`_ to perform high-performance molecular dynamics simulations.

.. _plip: https://journals.aps.org/prb/abstract/10.1103/PhysRevB.107.174106
.. _lammps: https://docs.lammps.org/

PLIP Components
------------------

