import pytest

def test_plip_module_import():
    try:
        import plip
    except ImportError:
        pytest.fail("Failed to import 'plip' module")
