import sys 
from matrix_lib import Matrix

def write_matrix(matrix, filename):
    with open(filename, 'w') as f:
        f.write("%d\n%d\n" % (matrix.rows, matrix.cols))
        f.write(" ".join(map(str, matrix.data)))

def read_matrix(filename):
    with open(filename) as f:
        try:
            rows = int(f.readline())
            cols = int(f.readline())
            data = map(float, f.readline().split())
        except ValueError as e:
            raise ValueError("Invalid input")
        return Matrix(rows, cols, data)

if __name__ == "__main__":
    matrix1 = read_matrix(sys.argv[1])
    matrix2 = read_matrix(sys.argv[2])
    result = matrix1.multiply(matrix1)
    write_matrix(result, sys.argv[3])
