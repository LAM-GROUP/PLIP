import pytest
import os
from plip.main import *
from unittest.mock import Mock



def test_xmat_input():
     
    config = {
        'Rcut': 10,
        'DUMP': 'Input',
        'RAA': 0.5,
        'RBB': 0.9,
        'RAB': 0.6,
        'i_nature': '1',
        'included': 1,
        'a': {'MIN': 1, 'MAX': 5, 'STEP': 0.5},
        'b': {'MIN': 0.1, 'MAX': 0.9, 'STEP': 0.2},
        '2B': 1,
        '3B': 5,
        'NB': 7
    }
     
    xmat_input(config)

    assert os.path.isfile('input.txt')

@pytest.fixture
def sample_yaml(tmpdir):
    """Create a sample YAML file with test data"""
    data = {
        "Input": {
            "Rcut": 6,
            "DUMP": "Input",
            "RAA": 2.1,
            "RBB": 2.3,
            "RAB": 2.4,
            "i_nature": 1,
            "included": 1,
            "a": {"MIN": 0.5, "MAX": 1.5, "STEP": 0.5},
            "b": {"MIN": 0.5, "MAX": 6, "STEP": 0.5},
            "2B": 1,
            "3B": 5,
            "NB": 7,
        },
        "compile": 0,
        "genBin": 0,
        "train": {"i_nature": "1","alpha": None},
        "genPot": True,
    }
    yaml_file = tmpdir.join("test.yaml")
    with open(yaml_file, "w") as f:
        yaml.dump(data, f, sort_keys=False)
    return str(yaml_file)

def test_yaml_reader(sample_yaml,monkeypatch):
    
    mock_xmat_input = Mock()
    mock_compile = Mock()
    mock_genBin = Mock()
    mock_runLasso = Mock()
    mock_genPot = Mock()

    monkeypatch.setattr("plip.main.xmat_input", mock_xmat_input)
    monkeypatch.setattr("plip.main.compile", mock_compile)
    monkeypatch.setattr("plip.main.genBin", mock_genBin)
    monkeypatch.setattr("plip.main.genPot", mock_genPot)
    monkeypatch.setattr("plip.main.runLasso", mock_runLasso)

    yaml_reader(sample_yaml)

    assert mock_xmat_input.called_once
    assert mock_compile.called_once
    assert mock_genBin.called_once
    assert mock_genPot.called_once



