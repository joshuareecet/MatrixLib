#include <iostream>
#include <iomanip>
#include "matrix.hpp"

void check(const bool condition, const std::string& name) {
    if (condition) {
        std::cout << "PASS: " << name << '\n';
    }
    else {
        std::cout << "FAIL: " << name << '\n';
    }
}

int main() {
    const matlib::Matrix a {2,2,{1,2,3,4}};
    const matlib::Matrix b {2,2,{5,6,7,8}};
    const matlib::Matrix diff {2,2,{4,4,4,4}};
    const matlib::Matrix non_square {2,3,{1,2,3,4,5,6}};
    const matlib::Matrix non_square_t {3,2,{1,4,2,5,3,6}};

    check(a*matlib::identity(a.cols()) == a,"identity");
    check(a + diff == b,"addition");
    check(b - diff == a,"subtraction");
    check(2*a == matlib::Matrix (2,2,{2,4,6,8}),"Multiplication");
    check(a.transpose().transpose() == a,"a^T^T = a");
    check(non_square.transpose() == non_square_t,"transpose non square");
    check((a-b).transpose() == (-1*(b-a)).transpose(),"Mixed properties");
    check(a * b == matlib::Matrix(2,2,{19,22,43,50}), "matrix multiplication");

    try {
        matlib::Matrix c{a+non_square};
        std::cout << "FAIL: did not catch mismatch dimension error!" << '\n';
    }
    catch (const std::exception& e){
        std::cout << "PASS: threw length error for adding matrices of different sizes:\t" << e.what() <<'\n';
    }
    try {
        std::cout << a;
        std::cout << "PASS: printing matrix" << '\n';
    }
    catch (std::exception) {
        std::cout << "FAIL: exception thrown for printing a matrix" << '\n';
    }
}