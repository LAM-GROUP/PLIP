## 0.3.0 (2023-06-29)

### Feat

- Train a potential with single alpha value of choice of the user from yaml and cli interface
- Yaml workflow for training PLIP

### Fix

- Remove coefficient.txt files after generating potential
- Scikit-learn version dependency switched to 1.0.0

### Refactor

- RunLasso function divided into separate functions for better readability
- Broken down existing functions to ease testing

## 0.2.0 (2023-06-02)

### Feat

- version feature for the executable
- One step approach from generating ML fingerprint to potential
- Basic installation of package with pyproject.toml

### Fix

- Version updation in pyproject.toml file with commitzen
- Installation of extra dependency pytest via pyproject.toml

### Refactor

- Correct import setting for cli interface in bin folder
- Changed package layout to have  src folder

## 0.1.0 (2023-05-25)

### Feat

- A minimal cli interface for plip
- Checks existence of essential files and folders for training
- Main interface for PLIP package
- A main  interface for PLIP module functions
- Essential compilation, executaion and movement features in bash
- Combinied Sphinx doxygen via breathe extension for documentation

### Fix

- function call changed from main to interface
- Corrected indentation error

### Refactor

- pyproject.toml file via commitzen
- Lasso script changed to a module
- renamed src/ folder to cpp/
- Change src to package name plip
