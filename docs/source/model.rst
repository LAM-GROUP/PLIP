PLIP model
==========
The total potential energy of a configuration made of N atomic positions is first given by :math:`E_{\textrm{tot}} = \sum_{i = 0}^N E_{i}` where Ei is the atomic energy. 
For Ei , we considered a weighted linear combination of descriptors indexed by n:

.. math::

    E^{(i)} = \sum_n \omega_n X_n^{(i)}

where :math:` \omega_n` is the linear coefficient associated with the descriptor   :math:`X_n^{(i)}`.

Descriptors
-----------
The descriptor space is divided into two-body, three-body and N-Body components with  following equations:

.. math::

    
    &[2B]_n^i = \sum_j f_n(r_{ij}) \times f_c(r_{ij}), \\
    &[3B]_{n,l}^i = \sum_j \sum_k f_n(r_{ij})f_c(r_{ij}) f_n(r_{ik})f_c(r_{ik})cos^l(\theta_{ijk}),  \\ 
    &[NB]_{n,m}^i = \left( \sum_j f_n(r_{ij}) \times f_c(r_{ij}) \times f_s(r_{ij}) \right)^m,

where :math:`r_{ij}` is the distance between atoms :math:`i` and :math:`j`,
:math:`\theta_{ijk}` is the angle centered around the atom :math:`i`. The cut-off function is as follows:

.. math::

    f_c = \frac{1}{2}\left(1+\cos(\pi(R_{ij}/R_{\textrm{cut}})) \right)

Switch function:

.. math::

    u = (R_{ij}-r_1)/(r_2-r_1)\\
    f_s(u) = 6{u}^5-15{u}^4+11^3

Commonly, gaussian descriptors are used with the following equation:

.. math::

    f_n(R_{ij}) = \exp(-a_n(R_{ij}-b_n)^2)

where :math:`a_{n}` and :math:`b_{n}` are **parameters** to optimise. 


Lasso Model
-----------

The set of descriptors are optimised using the Lasso regression method as follows:

.. math::

    \chi^2 = \chi^2_{\textrm{OLS}}+\alpha\sum_{n} \vert\omega_{(n)} \vert

where :math:`\alpha` is a **parameter** to control regularization  and :math:`\chi^2_{\textrm{OLS}}` is  ordinary least square objective function
