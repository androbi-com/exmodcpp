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

typedef std::complex<float> complex;

// window in complex plane from z1 to z2 with ascii chars as display
class ascii_view {
  private:
    size_t cols_, rows_;
    complex z1_, z2_;
    // each point has an associated char ('*' for non-escaping, '0'-'9' for escaping)
    std::vector<char> data_;
  public:
    void set_data(const int* h_iter) {
        for (size_t i = 0; i < win.cols_ * win.rows_; ++i) {
            data_[i] = (h_iter[i] == 10 ? char(42) : char(h_iter[i]+48));
        }
    }
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

// set win.dat_ member from h_d

void usage(const std::string& name) {
    std::cout << "Usage: " << std::endl << "  " << name << " cols rows+1 [-noprint]" << std::endl;
    std::cout << "     where: cols >= 2, rows+1 >=3" << std::endl;
    std::cout << "I.e: " << std::endl << "  " << name << " $(tput cols) $(tput lines)" << std::endl;
}

// kernel to iterate one point in the c-plane, returns iterations to escape
__global__ void point_iterate(int* area, float zr1, float zc1, float zr2, float zc2, const int nx, const int ny, const int max_iter) {
    int i = 0;
    unsigned int ix = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int iy = blockIdx.y * blockDim.y + threadIdx.y;
    unsigned int idx = iy * nx + ix;
    //printf(" %d %d \n", ix, iy);
    if (ix >= nx || iy >= ny) {
        return;
    }
    float zr = (zr2 - zr1) * ix / (nx - 1.0);
    float zc = (zc2 - zc1) * iy / (ny - 1.0));
    float cr = z1r + zr; 
    float cc = z1c + zc;

    //printf(" %e %e \n", cuCrealf(c), cuCimagf(c));
    while ( (zr*zr + zc*zc) < 4.0 && i < max_iter) {
        zr = zr*zr - zc*zc + cr;
        zc = 2.0*zr*zc + cc;
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
    a.add<int>("bsize", 'b', "block size", false, 8, cmdline::range(1, 32));
    a.add("noprint", '\0', "supress printing");
    a.parse_check(argc, argv);

    size_t cols = a.get<int>("cols");
    size_t rows = a.get<int>("rows");
    size_t bsize = a.get<int>("bsize");
    bool noprint = a.exist("noprint");

    std::cout << "Calculating mandelbrot set with size "  << cols 
        << " * " << rows  << std::endl;

    // set up 2D block sizes
    dim3 block(bsize, bsize);
    dim3 grid((cols + block.x - 1) / block.x, (rows + block.y - 1) / block.y);
    std::cout << "block: " << block.x << " " << block.y << std::endl;
    std::cout << "grid: " << grid.x << " " << grid.y << std::endl;

    // view window for mandelbrot set
    complex z1{-2.0,-1.0};
    complex z2{1.0,1.0};

    // malloc host memory for result of mandelbrot set and initialize to 0
    int* h_m = (int *)malloc(sizeof(int)*rows*cols);
    memset(h_m, 0, sizeof(int)*rows*cols);

    int* d_m;
    // malloc device memory for result of mandelbrot set (number of iterations for each point)
    cudaMalloc((void **) &d_m, sizeof(int)*rows*cols);

    // call kernel
    point_iterate<<< grid, block >>>(d_m, z1.real(), z1.imag(), z2.real(), z2.imag(), cols, rows, 100);
    cudaDeviceSynchronize();

    // copy result from device to host and free device memory
    cudaMemcpy(h_m, d_m, sizeof(int)*rows*cols, cudaMemcpyDeviceToHost);
    cudaFree(d_m);

    // print result
    if (!noprint) {
        ascii_view win(cols,rows,z1,z2);
        win.set_data(h_m);
        win.print();  
    }
    // free host memory
    free(h_m);
    return 0;
}
