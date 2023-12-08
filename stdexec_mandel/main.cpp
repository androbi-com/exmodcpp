#include <stdexec/execution.hpp>
#include "exec/static_thread_pool.hpp"
#include <iostream>
#include <complex>
#include <cassert>
#include <vector>
#include <ranges>
#include <limits>
#include "cmdline.h"
#include <sys/ioctl.h>
//#include <stdio.h>
//#include <unistd.h>

using namespace stdexec;
typedef std::complex<double> complex;

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

int main(int argc, char *argv[]) {
    auto num_cpus = std::thread::hardware_concurrency();
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    cmdline::parser a;
    a.add<int>("cols", 'c', "number of columns", false, w.ws_col, cmdline::range(2, std::numeric_limits<int>::max()));
    a.add<int>("rows", 'r', "number of rows", false, w.ws_row-1, cmdline::range(2, 1000000));
    a.add<int>("tiles", 't', "number of tiles", false, num_cpus, cmdline::range(1, 1000));
    a.add("noprint", '\0', "supress printing");
    a.parse_check(argc, argv);

    size_t cols = a.get<int>("cols");
    size_t rows = a.get<int>("rows");
    size_t ntiles = a.get<int>("tiles");
    bool noprint = a.exist("noprint");
    int tile_size = cols*rows / ntiles;

    std::cout << "Calculating mandelbrot set with size "  << cols 
        << " * " << rows << " using " << ntiles << " tiles" << std::endl;

    // view window for mandelbrot set
    ascii_view win(cols,rows,complex{-2.0,-1.0},complex{1.0,1.0});

    // lambda to iterate one point in the c-plane, returns iterations to escape
    auto mb_point_iterate = [=](const std::complex<double>& c, const int max_iter) {
        int i = 0;
        std::complex z = 0.0;
        while (abs(z) < 2.0 && i < max_iter) {
            z = z*z + c;
            i++;
        }
        return i;
    };

    // thread pool
    exec::static_thread_pool ctx{num_cpus};
    scheduler auto sch = ctx.get_scheduler();

    // work pipeline of senders
    sender auto work = schedule(sch)
        | bulk(ntiles, [&](std::size_t n) mutable {
            size_t offset = n*tile_size;
            if (n < ntiles-1)
                calculate_tile(tile_size, offset, win, mb_point_iterate);
            else // remainder goes to last tile
                calculate_tile(tile_size + cols*rows % ntiles, offset, win, mb_point_iterate);
          });
    
    // get work going now and wait for completion
    sync_wait(std::move(work)).value();  

    if (!noprint) {
        win.print();  
    }
    return 0;
}