Installation
============

The PLIP software, in its latest version, is compatible with Python versions >= 3.8 
. As for the operating system requirements, 
currently, we only support **Linux** distribution.

From source
-----------
To install the latest development version from source, clone the source code
from github and install with pip in **editable mode** (-e flag):

.. code-block:: sh

    git clone https://github.com/LAM-GROUP/PLIP.git
    cd  plip
    pip install -e .

The editable mode is currently required for correct compilation of C++ modules. If the package is not installed in editable mode, you would need to set up the ``PYTHONPATH`` environment variable to point to the location of the cloned package from Git. In the later versions, we plan to move to a complete ``pip`` set up for installation. 
To check successful installation::
    
    plip_fit -h

LAMMPS for PLIP
---------------
#. First clone the custom LAMMPS repo::

    git clone https://github.com/LAM-GROUP/LammpsWithPLIP.git

#. Next install LAMMPS:: 

    cd LammpsWithPLIP/src
    make -j 8 mpi

#. To check successful installation::

    lmp_mpi -h

Ignore the warnings ``-Weffc++`` and ``-Wunused-result``  during ``make``.
