#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

#include "tensor/xtensor_lib.h"


void tensor_status(){
  xt::xarray<double> data;
  double d = 123;

  cout << "NOT BEEN INITIALIZED/CREATED: Tensor is a scalar (dimension==0)" << endl;
  cout << "data: " << data << endl;
  cout << "data.shape(): " << shape2str(data.shape()) << endl;
  cout << "data.dimension(): " << data.dimension() << endl;
  cout << "data.size(): " << data.size() << endl;
  cout << "d + data: " << d + data << endl;
  cout << "d + data[0]: " << d + data[0] << endl << endl;

  data = 10;
  cout << "INITIALIZED/CREATED WITH SCALAR: Tensor is a scalar (dimension==0)" << endl;
  cout << "data: " << endl << data << endl;
  cout << "data.shape(): " << shape2str(data.shape()) << endl;
  cout << "data.dimension(): " << data.dimension() << endl;
  cout << "data.size(): " << data.size()  << endl;
  cout << "d + data: " << d + data << endl;
  cout << "d + data[0]: " << d + data[0] << endl << endl;

  cout << "INITIALIZED/CREATED WITH A TENSOR: dimension!=0, shape is not empty " << endl;
  data = xt::arange(10).reshape({2,5});
  cout << "A non-empty tensor: (dimension!=0)" << endl;
  cout << "data: " << endl << data << endl;
  cout << "data.shape(): " << shape2str(data.shape()) << endl;
  cout << "data.dimension(): " << data.dimension()<< endl;
  cout << "data.size(): " << data.size() << endl << endl;
}

int main(int argc, char** argv) {
    tensor_status();

    return 0;
}