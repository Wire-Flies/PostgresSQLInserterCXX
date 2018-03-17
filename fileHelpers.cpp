#include "fileHelpers.hpp"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


std::vector<std::string> split(std::string str, char delimiter){
    std::vector<std::string> ret;
    std::size_t cur_start = 0;
    for(std::size_t i = 0; i < str.size(); i++){
        if(str[i] == delimiter){
            ret.push_back(str.substr(cur_start, i - cur_start));
            cur_start = i;
        }
    }
    ret.push_back(str.substr(cur_start));
    return ret;
}

std::vector<std::string> readDir(const std::string & path, const std::string & type){
    std::vector<std::string> ret;
    for(auto & p : fs::directory_iterator(path)){
        if(fs::is_directory(p)){
            
            auto paths = readDir(p.path().c_str(), type);
            ret.insert(ret.end(), paths.begin(), paths.end());
        }else{
            if(p.path().extension() == type){
                ret.push_back(p.path().c_str());
            }
        }
    }

    return ret;
}
