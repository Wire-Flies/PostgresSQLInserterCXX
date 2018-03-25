#include "zipHelpers.hpp"
#include "fileHelpers.hpp"
#include <iostream>
#include <string>
#include <zip.h>

std::vector<data_item> readZipFile(const std::string & path){
    int err_flag;
    zip_t * zip = zip_open(path.c_str(), ZIP_RDONLY, &err_flag);
    if(!zip){
        std::cout << "Error when opening zip file: " << path << ", code: " << err_flag << std::endl;
        return {};
    }

    zip_int64_t n_entries = zip_get_num_entries(zip,  ZIP_FL_UNCHANGED);
    std::vector<data_item> data_items;
    for(zip_int64_t i = 0; i < n_entries; i++){
        zip_stat_t stat;
        zip_stat_index(zip, i, ZIP_FL_UNCHANGED, &stat);
        zip_file_t *file = zip_fopen_index(zip, i, ZIP_FL_UNCHANGED);
        char* data = new char[stat.size];
        zip_int64_t nb = zip_fread(file, data, stat.size);
        zip_fclose(file);
        if(nb == -1){
            std::cout << "Could not read from: " << stat.name << std::endl;
            continue;
        }

        std::string str(data, stat.size);
        std::vector<std::string> lines = split(str, '\n');
        std::string flight_id_str = split(stat.name, '_')[1];
        ull flight_id = std::atoll(flight_id_str.substr(0, flight_id_str.size() - 4).c_str());
        unsigned int line_n = 0;
        for(std::string line : lines){
            if(line_n++ == 0){
                continue; //Skip the first line
            }

            std::vector<std::string> split_data = split(line, ',');
            if(split_data.size() != 8){
                continue;
            }

            data_item item{flight_id, std::atoll(split_data[0].c_str()), std::atoll(split_data[1].c_str()), std::atoll(split_data[2].c_str()), 
                std::atof(split_data[3].c_str()), std::atof(split_data[4].c_str()), std::atoll(split_data[5].c_str()), 
                std::atoll(split_data[6].c_str()), std::atoll(split_data[7].c_str())};
            data_items.push_back(item);
        }
        delete[] data;
    }

    zip_discard(zip);

    return data_items;
}