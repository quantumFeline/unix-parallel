#include <iostream>
#include <random>

typedef std::vector< std:: vector<float> > Matrix;

void fill_matrix(int w, int h, Matrix & matrix,
                 std::uniform_real_distribution<float>& distribution,
                 std::mt19937& generator) {
    matrix.resize(w);
    for (int x = 0; x < w; x++) {
        matrix[x].resize(h);
        for (int y = 0; y < h; y++) {
            matrix[x][y] = distribution(generator);
        }
    }
}

float cell_value(const Matrix & matrix_nm, const Matrix& matrix_mk, int x, int y) {
    if (matrix_nm.empty() || matrix_mk.empty()) {
        printf("cell value - error: empty matrix\n");
        return 0;
    }
    if (x < 0 || y < 0 || y >= matrix_nm[0].size() || x >= matrix_mk.size()) {
        printf("cell value - error: index out of bounds\n");
        return 0;
    }
    float res = 0;
    int m = matrix_nm[0].size();
    for (int i = 0; i < m; i++) {
        res += matrix_nm[i][y] * matrix_mk[x][i];
    }
    return res;
}

void print_matrix(const Matrix & matrix) {
    if (matrix.empty()) {
        printf("print_matrix - error: empty matrix\n");
        return;
    }
    for (int y = 0; y < matrix[0].size(); y++) {
        for (int x = 0; x < matrix.size(); x++) {
            printf ("%f ", matrix[x][y]);
        }
        printf("\n");
    }
}

inline int read_int(char* arr) {
    return int(strtol(arr, nullptr, 10));
}

void print_usage() {
    printf("usage:\n"
           "three numbers n, m, k, for matrices sizes (n x m) and (m x k) + mode\n"
           "1 - auto-generate matrices\n"
           "2 - enter matrices from keyboard\n"
           "default mode is 1\n");
}

int main(int argc, char**argv) {
    std::uniform_real_distribution<float> random_numbers(0,1);
    std::random_device rd;
    std::mt19937 mt(rd());

    if (argc < 4 || argc > 5) {
        print_usage();
        return 0;
    }
    int n = read_int(argv[1]),
         m = read_int(argv[2]),
         k = read_int(argv[3]);
    Matrix matrix_nm, matrix_mk;
    int mode = 1;
    if (argc == 5) mode = read_int(argv[4]);

    if (mode == 1) {
        fill_matrix(n, m, matrix_nm, random_numbers, mt);
        fill_matrix(m, k, matrix_mk, random_numbers, mt);
    } else if (mode == 2) {
        // TODO
    } else {
        print_usage();
        return 0;
    }
    printf("Matrix 1:\n");
    print_matrix(matrix_nm);
    printf("Matrix 2:\n");
    print_matrix(matrix_mk);
    printf("%f\n", cell_value(matrix_nm, matrix_mk, 0, 0));
    return 0;
}
