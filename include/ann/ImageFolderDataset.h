#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include "list/XArrayList.h"
#include "ann/dataset.h"
using namespace std;
namespace fs = filesystem;

template <typename DType, typename LType>
class ImageFolderDataset : public Dataset<DType, LType>
{
private:
    string root_direction;
    XArrayList<string> image_paths;
    XArrayList<LType> labels;
    xt::svector<unsigned long> data_shape;

    // phuong thuc tai hinh anh va nhan
    void load_images()
    {
        int label_idx = 0;
        for (const auto &entry : fs::directory_iterator(root_direction))
        {
            if (fs::is_directory(entry.path()))
            {
                for (const auto &image_file : fs::directory_iterator(entry.path()))
                {
                    image_paths.add(image_file.path().string());
                    labels.add(label_idx);
                }
                label_idx++;
            }
        }
    }

public:
    // Ham khoi tao nhan vao thu muc chua image
    ImageFolderDataset(const string &root_direction) : root_direction(root_direction)
    {
        load_images();
        // image cung kich thuoc
        data_shape = {100, 100};
    }

    int len() override
    {
        return image_paths.size();
    }

    // return nhan va image
    DataLabel<DType, LType> getitem(int index) override
    {
        xt::xarray<DType> img_data = xt::zeros<DType>(data_shape); // Giả định mỗi ảnh có kích thước 100x100
        LType label = labels.get(index);
        return DataLabel<DType, LType>(img_data, xt::xarray<LType>{label});
    }

    xt::svector<unsigned long> get_data_shape() override
    {
        return data_shape;
    }

    xt::svector<unsigned long> get_label_shape() override
    {
        return {1};
    }
};
