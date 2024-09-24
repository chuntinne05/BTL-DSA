#include <iostream>
// #include <random>
// #include <chrono>
// #include <ann/xtensor_lib.h>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

int main()
{

    fs::path p = "/Users/trinhtrantrungtin/Documents/HCMUT/DSA/xflens";
    cout << "file name : " << p.filename() << "\n";
    cout << p.parent_path();
    // vector<int> indices;
    // indices.push_back(1);
    // indices.push_back(2);
    // indices.push_back(3);
    // indices.push_back(4);
    // indices.push_back(5);
    // indices.push_back(6);

    // %%%%%%%%%%%%% BO SINH SO DUNG THOI GIAN HE THONG %%%%%%%%%%%%%%%%
    // unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    // mt19937 engine(seed);
    // shuffle(indices.begin(), indices.end(), engine);

    // %%%%%%%%%%%%% BO SINH SO DUNG SEED MAC DINH
    // mt19937 engine;
    // shuffle(indices.begin(), indices.end(), engine);

    // for(int i : indices) {
    //     cout<< i << " ";
    // }
    // return 0;
}
