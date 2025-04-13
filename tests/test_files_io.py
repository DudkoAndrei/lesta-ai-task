import pytest
import matrix_mul
from matrix_lib import Matrix

@pytest.mark.parametrize("file_content,matrix,error", [
    (
        "2\n2\n1 2 3 4",
        Matrix(2, 2, [1, 2, 3, 4]),
        None
    ),
    (
        "0\n0\n",
        Matrix(0, 0),
        None
    ),
    (
        "2\n2\n",
        None,
        "Size of data is invalid"
    ),
    (
        "2\n2\n1 2 3",
        None,
        "Size of data is invalid"
    ),
    (
        "2\n2\na b c d",
        None,
        "Invalid input"
    ),
    (
        "",
        None,
        "Invalid input"
    ),
    (
        "2 2 1 2 3 4",
        None,
        "Invalid input"
    ),
])
def test_read_matrix(monkeypatch, file_content, matrix, error):
    mock_file = iter(file_content.split("\n"))
    
    class MockFile:
        def __enter__(self):
            return self
        
        def __exit__(self, *args):
            pass
        
        def readline(self):
            try:
                return next(mock_file) + "\n"
            except StopIteration:
                return ""
        
        def read(self):
            return "\n".join(mock_file)
    
    def mock_open(*args, **kwargs):
        return MockFile()
    
    monkeypatch.setattr("__builtin__.open", mock_open)
    
    if error:
        with pytest.raises(ValueError, match=error):
            matrix_mul.read_matrix("dummy.txt")
    else:
        assert matrix == matrix_mul.read_matrix("dummy.txt")

@pytest.mark.parametrize("matrix,expected_content", [
    (
        Matrix(2, 2, [1, 2, 3, 4]),
        "2\n2\n1.0 2.0 3.0 4.0"
    ),
    (
        Matrix(0, 0),
        "0\n0\n"
    )
])
def test_write_matrix(monkeypatch, matrix, expected_content):
    written_data = []
    
    class MockFile:
        def __enter__(self):
            return self
        
        def __exit__(self, *args):
            pass
        
        def write(self, data):
            written_data.append(data)
    
    def mock_open(filename, mode):
        return MockFile()
    
    monkeypatch.setattr("__builtin__.open", mock_open)
    
    matrix_mul.write_matrix(matrix, "dummy.txt")
    
    assert "".join(written_data).strip() == expected_content.strip()
