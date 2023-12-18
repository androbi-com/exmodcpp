#include <iostream>
#include <complex>
#include <cassert>
#include <vector>
#include <ranges>
#include <limits>
#include "cmdline.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <cuComplex.h>

//#include <stdio.h>
//#include <unistd.h>

typedef std::complex<float> complex;

// window in complex plane from z1 to z2 with ascii chars as display
class ascii_view {
  private:
    size_t cols_, rows_;
    complex z1_, z2_;
    // each point has an associated char ('*' for non-escaping, '0'-'9' for escaping)
    std::vector<char> data_;
    // return complex number in window for given row and column
    complex get_z(int row_index, int col_index) const {
        std::complex z = z2_-z1_;
        z.real(z.real() * col_index / (cols_ - 1.0));
        z.imag(z.imag() * row_index / (rows_ - 1.0));
        return z1_ + z;
    }
    // return complex number in window for given linear index
    complex get_z(int linear_index) const {
        int row_index = linear_index / cols_;
        int col_index = linear_index % cols_;
        return get_z(row_index, col_index);
    }
    // external calculator function
    friend void calculate_tile(size_t tile_size, size_t offset, ascii_view& win, auto fun);
  public:
    void print() const {
        for (int n = 0; n < rows_; ++n) {
            for (char c : std::views::counted(data_.begin()+n*cols_, cols_)) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }
    // construct complex plane window z1 to z2
    ascii_view(size_t cols, size_t rows, complex z1, complex z2)
        : cols_{cols}, rows_{rows}, z1_{z1}, z2_{z2} {
            assert(cols_ > 1);
            assert(rows_ > 1);
            data_.resize(cols_*rows_);
            for(char& c : data_) {
                c = '.';
            }
    }
};

// calculates tile n of all tiles with size tile_size of the mandelbrot set
void calculate_tile(size_t tile_size, size_t offset, ascii_view& win, auto fun) {
    //std::cout << "calculate_tile: " << tile_size << " " << offset << std::endl;
    for (size_t i = 0; i < tile_size; ++i) {
        auto c = win.get_z(offset + i);
        auto iter = fun(c, 100)/10;
        win.data_[offset + i] = (iter == 10 ? char(42) : char(iter+48));
    }
}

void usage(const std::string& name) {
    std::cout << "Usage: " << std::endl << "  " << name << " cols rows+1 tiles [-noprint]" << std::endl;
    std::cout << "     where: cols >= 2, rows+1 >=3, ntiles >= 1" << std::endl;
    std::cout << "I.e: " << std::endl << "  " << name << " $(tput cols) $(tput lines) 1" << std::endl;
}

// lambda to iterate one point in the c-plane, returns iterations to escape
__global__ void point_iterate(int* area, float zr1, float zc1, float zr2, float zc2, const int nx, const int ny, const int max_iter) {
    int i = 0;
    unsigned int ix = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int iy = blockIdx.y * blockDim.y + threadIdx.y;
    unsigned int idx = iy * nx + ix;
    //printf(" %d %d \n", ix, iy);
    cuFloatComplex z1 = make_cuFloatComplex(zr1, zc1);
    cuFloatComplex z2 = make_cuFloatComplex(zr2, zc2);
    cuFloatComplex z = cuCsubf(z2,z1);
    z = make_cuFloatComplex(cuCrealf(z) * ix / (nx - 1.0), cuCimagf(z) * iy / (ny - 1.0));
    cuFloatComplex c = cuCaddf(z1, z);

    //printf(" %e %e \n", cuCrealf(c), cuCimagf(c));
    while (cuCabsf(z) < 2.0 && i < max_iter) {
        z = cuCaddf(cuCmulf(z,z), c);
        i++;
    }
    printf(" %d \n", i);
    area[idx] = i;
};    

int main(int argc, char *argv[]) {
    // get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    cmdline::parser a;
    a.add<int>("cols", 'c', "number of columns", false, w.ws_col, cmdline::range(2, std::numeric_limits<int>::max()));
    a.add<int>("rows", 'r', "number of rows", false, w.ws_row-1, cmdline::range(2, 1000000));
    a.add("noprint", '\0', "supress printing");
    a.parse_check(argc, argv);

    size_t cols = a.get<int>("cols");
    size_t rows = a.get<int>("rows");
    bool noprint = a.exist("noprint");

    std::cout << "Calculating mandelbrot set with size "  << cols 
        << " * " << rows  << std::endl;

    // set up 2D block sizes
    dim3 block(32, 32);
    dim3 grid((cols + block.x - 1) / block.x, (rows + block.y - 1) / block.y);

    // view window for mandelbrot set
    complex z1{-2.0,-1.0};
    complex z2{1.0,1.0};
    // initialize ascii view
    ascii_view win(cols,rows,z1,z2);

    // malloc host memory for result of mandelbrot set
    int* h_m = (int *)malloc(sizeof(int)*rows*cols);
    // initialize memory to 0
    memset(h_m, 0, sizeof(int)*rows*cols);

    int* d_m;
    // malloc device memory for result of mandelbrot set
    cudaMalloc((void **) &d_m, sizeof(int)*rows*cols);

    point_iterate<<< grid, block >>>(d_m, z1.real(), z1.imag(), z2.real(), z2.imag(), cols, rows, 100);
    cudaDeviceSynchronize();

    // copy result from device to host
    cudaMemcpy(h_m, d_m, sizeof(int)*rows*cols, cudaMemcpyDeviceToHost);
    // free device memory
    cudaFree(d_m);
 
    // print result
    if (!noprint) {
        win.print();  
    }
    // free host memory
    free(h_m);
    return 0;
}
