/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
// #include "list/XArrayList.h"
#include<chrono>
using namespace std;

template<typename DType, typename LType>
class DataLoader{ 
public:
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    xt::xarray<unsigned long> indices;
    size_t current_idx;
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false) 
    {
        /*TODO: Add your code to do the initialization */
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->current_idx = 0;
        int dataset_size = ptr_dataset->len();
        indices = xt::arange<unsigned long> (0, dataset_size);
        if (shuffle)
        {
            // %%%%%%%%%%%%% BO SINH SO DUNG THOI GIAN HE THONG %%%%%%%%%%%%%%%%
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            xt:random:shuffle(indices, seed);
            // mt19937 engine(seed);
            // shuffleList(engine);
        }
    }
    virtual ~DataLoader(){}

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
    
    /*TODO: Add your code here to support iteration on batch*/

    // void shuffleList(mt19937 &engine) {
    //     for (int i = indices.size() - 1; i > 0; i--)
    //     {
    //         uniform_int_distribution<int> distribte(0,i);
    //         int j = distribte(engine);
    //         int temp = indices.get(i);
    //         indices.get(i) = indices.get(j);
    //         indices.get(j) = temp;
    //     }
        
    // }
    class Iterator {

    private: 
        DataLoader<DType, LType>& loader;
        size_t batch_start;

    public:
        Iterator(DataLoader<DType, LType>& loader, size_t start) {
            this->loader = loader;
            this->batch_start = start;
        }
              
        bool operator!=(const Iterator& other) const {
            return batch_start != other.batch_start;
        }

        Iterator& operator++() {
            batch_start += loader.batch_size;
            return *this;
        }

        Batch<DType, LType> operator*() {
            size_t batch_end = min(batch_start + loader.batch_size, loader.ptr_dataset->len());
            xt::xarray<DType> batch_data;
            xt::xarray<LType> batch_label;

            for (size_t i = batch_start; i < batch_end; ++i) {
                int index = loader.indices[i];
                auto data_label = loader.ptr_dataset->getitem(index); 
                batch_data = xt::concatenate(xt::xtuple(batch_data, data_label.getData()));
                batch_label = xt::concatenate(xt::xtuple(batch_label, data_label.getLabel()));
            }

            return Batch<DType, LType>(batch_data, batch_label);
        }
    };
    
    Iterator begin() {
        current_idx = 0;
        return Iterator(*this, 0);
    }

    Iterator end() {
        size_t end_idx = ptr_dataset->len();
        if (drop_last && (end_idx % batch_size != 0))
        {
            end_idx = (end_idx / batch_size) * batch_size;
        }
        return Iterator(*this ,end_idx);
    }
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */