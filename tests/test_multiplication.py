import pytest
from matrix_lib import Matrix

@pytest.mark.parametrize("a,b,expected,error", [
    (
        Matrix(2, 2, [1, 2, 3, 4]),
        Matrix(2, 2, [5, 6, 7, 8]),
        Matrix(2, 2, [19, 22, 43, 50]),
        None
    ),
    (
        Matrix(2, 3, [4, 7, 3, 6, 7, 1]),
        Matrix(3, 1, [7, 3, 6]),
        Matrix(2, 1, [67, 69]),
        None
    ),
    (
        Matrix(0, 0),
        Matrix(0, 0),
        Matrix(0, 0, []),
        None
    ),
    (
        Matrix(2, 2),
        Matrix(3, 3),
        None,
        "Wrong matrix dimensions"
    ),
    (
        Matrix(1, 3, [1, 2, 3]),
        Matrix(3, 1, [1, 2, 3]),
        Matrix(1, 1, [14]),
        None
    ),
    (
        Matrix(3, 1, [1, 2, 3]),
        Matrix(1, 3, [1, 2, 3]),
        Matrix(3, 3, [1, 2, 3, 2, 4, 6, 3, 6, 9]),
        None
    ),
    (
        Matrix(2, 2, [1, 2, 3, 4]),
        Matrix(2, 4),
        Matrix(2, 4),
        None
    ),
])
def test_matrix_mul(a, b, expected, error):
    if error:
        with pytest.raises(ValueError, match=error):
            a.multiply(b)
    else:
        result = a.multiply(b)
        assert result == expected
