import pytest
import os
import numpy as np
from plip.Lasso import *


@pytest.fixture(scope="session", autouse=True)
def cleanup_generated_data(request):
    # Set up
    yield

    # Teardown: Clean up the generated data
    generated_files = [
        "test_forces.txt",
        "test_forces_with_nan.txt",
        "input.txt",
        "Coeff1e-07_1.txt",
        "Output_1.log",
        "ResultsTest1e-07_1.txt",
        "ResultsTrain1e-07_1.txt",
    ]
    [os.remove(file) for file in generated_files if os.path.exists(file)]


@pytest.mark.parametrize(
    "num, output",
    [("testdata/test_data.forces", 4), ("testdata/test_data_w_b_l.forces", 6)],
)
def test_getNatoms(num, output):
    num_lines = getNatoms(num)
    assert num_lines == output


def test_readForces(cleanup_generated_data):
    input_file = "test_forces.txt"
    forces = np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 9.0]])
    np.savetxt(input_file, forces, header="force")

    # read forces
    result = readForces(input_file)

    assert result.all() == forces.all()

    input_file_with_nan = "test_forces_with_nan.txt"
    forces_with_nan = np.array(
        [[1.0, np.nan, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, np.nan]]
    )
    np.savetxt(input_file_with_nan, forces_with_nan)

    with pytest.raises(ValueError):
        readForces(input_file_with_nan)


def test_getBinaryFiles():
    input_file = "testdata/Refs/100.forces"
    type = "1"
    expected_result = ["poscar_f1_2B.bin", "poscar_f1_3B.bin", "poscar_f1_NB.bin"]
    result = getBinaryFiles(input_file, type)
    result = [string.replace("testdata/Input/100.", "") for string in result]
    assert result == expected_result


def test_readBinaryFiles():
    input_file = "testdata/Refs/100.forces"
    type = "1"
    list_bin = getBinaryFiles(input_file, type)

    r_xmat = readBinaryFiles(list_bin)
    print(f'results" {r_xmat}')
    expected_xmat = np.zeros((3 * 1, 3 * 2))  # 3*N, total_bin_files*Nc of bin file
    assert np.array_equal(r_xmat, expected_xmat)


def test_runLasso():
    inputArgs = "1"
    alpha = 1e-07
    Refs = "testdata/fit_pot/Refs"
    score_train, score_test = runLasso(inputArgs, alpha, ref_dir=Refs)
    assert 0.93 <= score_train <= 1
    assert 0.9 <= score_test <= 1
