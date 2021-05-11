#include <iostream>
#include <random>
#include <thread>

/* Here, matrices are stored BY COLUMNS. */

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

float cell_value(const Matrix & matrix_nm, const Matrix& matrix_mk, Matrix& matrix_out, int x, int y) {
    if (matrix_nm.empty() || matrix_mk.empty()) {
        printf("cell value - ERROR: empty matrix\n");
        return 0;
    }
    if (x < 0 || y < 0 || y >= matrix_nm[0].size() || x >= matrix_mk.size()) {
        printf("cell value - ERROR: index out of bounds\n");
        return 0;
    }
    float res = 0;
    int m = matrix_nm.size();
    for (int i = 0; i < m; i++) {
        res += matrix_nm[i][y] * matrix_mk[x][i];
    }
    if (matrix_out[0].size() <= y || matrix_out.size() <= x) {
        printf("cell value - ERROR: incorrect output matrix size\n");
        printf("matrix size: %zu %zu, x = %d, y = %d\n", matrix_out[0].size(), matrix_out.size(), x, y);
    } else {
        matrix_out[x][y] = res;
    }
    printf("cell value (%d %d) - result: %f\n", x, y, res);
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
        fill_matrix(m, n, matrix_nm, random_numbers, mt);
        fill_matrix(k, m, matrix_mk, random_numbers, mt);
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

    Matrix matrix_nk;
    matrix_nk.resize(k);
    for (std::vector<float>& line : matrix_nk) line.resize(n);

    //printf("%f\n", cell_value(matrix_nm, matrix_mk,matrix_nk, 0, 0));

    std::vector <std::thread> threads;
    threads.reserve(n*k);

    for (int x = 0; x < k; x++) {
        for (int y = 0; y < n; y++) {
            auto call = [&matrix_nm, &matrix_mk, &matrix_nk, x, y]{cell_value(matrix_nm, matrix_mk,matrix_nk, x, y);};
            threads.emplace_back(std::thread(call));
        }
    }
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    print_matrix(matrix_nk);
    return 0;
}
