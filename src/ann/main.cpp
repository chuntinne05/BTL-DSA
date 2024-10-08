#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "list/listheader.h"

using namespace std;
#include "list/XArrayListDemo.h"
#include "list/DLinkedListDemo.h"
#include <list/XArrayListDemo.h>
// #include "include/ann/funtions.h"
// #include "include/ann/xtensor_lib.h"
// #include "include/ann/dataset.h"
// #include "include/ann/dataloader.h"
int main(int argc, char **argv)
{
    cout << "Assignment-1" << endl;
    // int nsamples = 100;
    // xt ::xarray<double> X = xt ::random ::randn<double>({nsamples, 10});
    // xt ::xarray<double> T = xt ::random ::randn<double>({nsamples, 5});
    // TensorDataset<double, double> ds(X, T);
    // DataLoader<double, double> loader(&ds, 30, true, false);
    // for (auto batch : loader)
    // {
    //     cout << shape2str(batch.getData().shape()) << endl;
    //     cout << shape2str(batch.getLabel().shape()) << endl;
    // }
    xlistDemo1();
    return 0;
}
