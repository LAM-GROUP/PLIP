""" This package contains the LASSO-LARS implementation for PLIP. """

import glob
import os
import random
from decimal import Decimal


import numpy as np
from sklearn import linear_model

# from sklearn.pipeline import make_pipeline
from sklearn.metrics import mean_absolute_error, mean_squared_error


# Read number of atoms
def getNatoms(input_file):
    """Counts number of atoms in a structure file

    Args:
        input_file (str): Input structure file

    Returns:
        int: total number of atoms
    """
    return sum(1 for line in open(input_file))


# Read forces
def readForces(input_file):
    """Read forces from a reference file

    Args:
        input_file (str): Input force file

    Returns:
        np.array: forces per atom
    """
    tmp = np.genfromtxt(input_file, invalid_raise=False, usecols=[0, 1, 2], dtype=float)
    if np.isnan(tmp).any():
        raise ValueError(f"NaN value encountered in {input_file} file")
    return tmp


def getBinaryFiles(input_file, list_itype):
    """Get the list of binary files based on the input file and descriptor types

    Args:
        input_file (str): Input structure file
        list_itype (list): List of descriptor types

    Returns:
        list: List of binary files
    """
    list_bin = []
    for i_type in list_itype:
        search = input_file[:-6] + "poscar_f" + str(i_type) + "_*bin"
        search = search.replace("Refs", "Input")
        print(search)
        list_bin.extend(
            glob.glob(search)
        )  # list_bin = np.append(list_bin, sorted(glob.glob(search)))
    return sorted(list_bin)


def readBinaryFiles(list_bin):
    """Read and concatenate data from binary files

    Args:
        list_bin (list): List of binary files

    Returns:
        np.array: Concatenated data from binary files
    """
    print(list_bin)
    XMAT = np.genfromtxt(list_bin[0])
    XMAT = np.transpose(XMAT)

    for input_bin in list_bin[1:]:
        XMAT_tmp = np.genfromtxt(input_bin)
        XMAT_tmp = np.transpose(XMAT_tmp)
        XMAT = np.concatenate((XMAT, XMAT_tmp), axis=1)

    return XMAT


def read_File(input_file, list_itype):
    """Read the input structure file and binary files

    Args:
        input_file (str): Input structure file
        list_itype (list): List of descriptor types

    Returns:
        np.array: Fingerprint matrix
        np.array: Forces per atom
    """
    Y = readForces(input_file)
    Y = Y.reshape(1, 3 * np.shape(Y)[0])
    Y = Y[0]

    list_bin = getBinaryFiles(input_file, list_itype)

    XMAT = readBinaryFiles(list_bin)

    return XMAT, Y


# Read Input files
def oldread_File(input_file, list_itype):
    """

    Args:
        input_file (str): input structure file
        list_itype (list): type of descriptor

    Returns:
        np.array: fingerprint matrix
    """
    input_lmp = input_file
    print(input_file)
    Y = readForces(input_lmp)
    Y = Y.reshape(1, 3 * np.shape(Y)[0])
    Y = Y[0]

    #    N_atoms = getNatoms(input_lmp)

    list_bin = []
    for i_type in list_itype:
        search = (
            input_file[:-6] + "poscar_f" + str(i_type) + "_*bin"
        )  # changing poscar fomat
        search = search.replace("Refs", "Input")
        print(search)
        list_bin = np.append(list_bin, sorted(glob.glob(search)))

    print(list_bin)

    XMAT = np.genfromtxt(list_bin[0])
    XMAT = np.transpose(XMAT)

    for input_bin in list_bin[1:]:
        XMAT_tmp = np.genfromtxt(input_bin)
        XMAT_tmp = np.transpose(XMAT_tmp)
        XMAT = np.concatenate((XMAT, XMAT_tmp), axis=1)

    return (XMAT, Y)


def setup_data(inputArgs, ref_dir):
    """Build fingerprint matrix and references
    Args:
        inputArgs (string): Descriptor type
        ref_dir (str, optional): Folder with reference forces. Defaults to "Refs".

    Raises:
        ValueError: When references are empty
    """
    list_itype = []
    str_type = ""
    for i in np.arange(len(inputArgs)):
        list_itype = np.append(list_itype, inputArgs[i])
        str_type = str_type + str(inputArgs[i])
    print(list_itype)
    print(str_type)

    #    N_sample = 100
    #    i_select = 1

    if not os.path.exists(ref_dir):
        raise FileNotFoundError(f"The directory '{ref_dir}' does not exist.")
    # Data reading
    search = f"{ref_dir}/*.forces"
    input_file = sorted(glob.glob(search))[0]
    print(f"input files:{input_file}")
    if len(input_file) == 0:
        raise ValueError(
            "The reference directory is empty or  references not in correct format"
        )
    XMAT, Yin = read_File(input_file, list_itype)

    print(XMAT.shape, Yin.shape)

    for input_file in sorted(glob.glob(search))[1:]:
        XMAT_tmp, Yin_tmp = read_File(input_file, list_itype)
        XMAT = np.concatenate((XMAT, XMAT_tmp), axis=0)
        Yin = np.append(Yin, Yin_tmp)
        if np.shape(XMAT)[0] != np.shape(Yin)[0]:
            print("WARNING!!! ", input_file, np.shape(XMAT)[0], np.shape(Yin)[0])

    filter_ = abs(Yin) < 15
    XMAT = XMAT[filter_]
    Yin = Yin[filter_]

    return XMAT, Yin, str_type


def split_data(XMAT, Yin, split_ratio=0.95):
    """Split the data set for testing and training

    Args:
        XMAT (nd.array): Input descriptors
        Yin (nd.array): References
        split_ratio (float, optional): Test-Train split ratio. Defaults to 0.95.

    Returns:
        nd.array: Test and Training data sets
    """
    # Split data into train and test sets
    N_sample = np.shape(Yin)[0]
    list_Train = random.sample(range(N_sample), int(split_ratio * N_sample))
    list_Test = np.delete(np.arange(N_sample), list_Train)
    XMAT_Train = XMAT[list_Train]
    Y_Train = Yin[list_Train]
    XMAT_Test = XMAT[list_Test]
    Y_Test = Yin[list_Test]
    return XMAT_Train, Y_Train, XMAT_Test, Y_Test


def fit_model(alpha, XMAT_Train, Y_Train):
    """Fit the lasso lars model

    Args:
        alpha (float): Regularization parameter
        XMAT_Train (nd.array):  Training set of descriptors
        Y_Train (nd.array): Reference training forces

    Returns:
        nd.array: Optimal coeffecients
    """
    # Fit the LassoLars model
    clf = linear_model.LassoLars(alpha=alpha)
    clf.fit(XMAT_Train, Y_Train)
    return clf


def write_coefficients(coeff, str_alpha, str_type):
    """Write coeffecients to a file

    Args:
        coeff (nd.array): optimal coefficients
        str_alpha (float): regularization parameter
        str_type (str): Descriptor type
    """
    # Write coefficients to file
    output_file = "Coeff" + str_alpha + "_" + str_type + ".txt"
    with open(output_file, "w") as fw:
        for i in range(np.size(coeff)):
            fw.write("%g %g\n" % (i, coeff[i]))


def write_results(Y, Y_OUT, str_alpha, str_type, prefix):
    """

    Args:
        Y (nd.array): Reference values
        Y_OUT (nd.array): Predictted values
        str_alpha (float):  regularization parameter
        str_type (str):  Descriptor type
        prefix (str): Filename prefix
    """
    # Write results to file
    output_file = prefix + str_alpha + "_" + str_type + ".txt"
    with open(output_file, "w") as fw:
        for i in range(np.size(Y)):
            fw.write("%g %g %g \n" % (i, Y[i], Y_OUT[i]))


# LASSO PROCESS
def runLasso(inputArgs, alpha=None, ref_dir="Refs"):
    """Run lasso Lars

    Args:
        inputArgs (string): Descriptor type
        alpha (float, optional): Regularization parameter. Defaults to "None".
        ref_dir (str, optional): reference folder name. Defaults to "Refs".

    Returns:
        tuple: A tuple containing the following elements:
            - scoreTrain (float): R-squared score on the training data.
            - scoreTest (float): R-squared score on the test data.
    """
    print("Running LassoLars")
    XMAT, Yin, str_type = setup_data(inputArgs, ref_dir)

    # Split data in Train/Test
    XMAT_Train, Y_Train, XMAT_Test, Y_Test = split_data(XMAT, Yin)

    with open("Output_" + str_type + ".log", "w") as fw0:
        fw0.write(
            "alpha  scoreTrain scoreTest RMSE_Train RMSE_Test MAE_Train MAE_Test numNonZeroCoeff numTotalCoeff meanAbsYTrain\n"
        )
        alphas = []
        if alpha is not None:
            alphas = [alpha]
        else:
            print("Generating range of alpha values:\n")
            for exponent in np.arange(4.0, 8.0)[::-1]:
                for number in np.arange(1.0, 10.0)[::5]:
                    alpha = pow(10, -exponent) * number
                    alpha = round(alpha, int(exponent))
                    alphas.append(alpha)
        for alpha in alphas:
            str_alpha = str("%.0e" % Decimal(alpha))
            # Fit
            clf = fit_model(alpha, XMAT_Train, Y_Train)
            coeff = np.copy(clf.coef_)
            # Score/RMSE/MAS
            Y_OUT_Train = clf.predict(XMAT_Train)
            Y_OUT_Test = clf.predict(XMAT_Test)
            scoreTrain = clf.score(XMAT_Train, Y_Train)
            scoreTest = clf.score(XMAT_Test, Y_Test)
            RMSE_Train = np.sqrt(mean_squared_error(Y_OUT_Train, Y_Train))
            RMSE_Test = np.sqrt(mean_squared_error(Y_OUT_Test, Y_Test))
            MAS_Train = mean_absolute_error(Y_OUT_Train, Y_Train)
            MAS_Test = mean_absolute_error(Y_OUT_Test, Y_Test)
            print(
                alpha,
                scoreTrain,
                scoreTest,
                RMSE_Train,
                RMSE_Test,
                MAS_Train,
                MAS_Test,
                sum(abs(coeff) > 1e-10),
                np.size(coeff),
                np.mean(abs(Y_Train)),
            )
            write_coefficients(coeff, str_alpha, str_type)
            write_results(Y_Test, Y_OUT_Test, str_alpha, str_type, "ResultsTest")
            write_results(Y_Train, Y_OUT_Train, str_alpha, str_type, "ResultsTrain")
            fw0.write(
                "%g  %g  %g  %g  %g   %g   %g   %g   %g   %g  \n"
                % (
                    alpha,
                    scoreTrain,
                    scoreTest,
                    RMSE_Train,
                    RMSE_Test,
                    MAS_Train,
                    MAS_Test,
                    sum(abs(coeff) > 1e-10),
                    np.size(coeff),
                    np.mean(abs(Y_Train)),
                )
            )
        return (scoreTrain, scoreTest)
