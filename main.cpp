#include <iostream>
#include <fstream>
//experimental/
#include <vector>
#include <map>
#include <thread>
#include "fileHelpers.hpp"
#include "pgHelpers.hpp"
#include "zipHelpers.hpp"

//With this configuration it uses about 3.7 GB of RAM
const unsigned int N_THREADS_CSV = 1;
const unsigned int N_THREADS_ZIP = 30;
const std::string DATA_FOLDER_PATH = "/home/jovi/school/mvk/secure.flightradar24.com/stockholm_1000km";

void processFlightCSV(pgconn & conn, const std::string & path){
    std::ifstream ifs(path);
    if(!ifs.is_open()){
        std::cout << path << " could not be opened" << std::endl;
        return;
    }

    std::map<std::string, std::vector<std::string>> parsed;

    std::string line;
    std::vector<flight_item> flight_items;
    while(std::getline(ifs, line)){
        auto split_data = split(line, ',');
        flight_item to_push{std::atoll(split_data[0].c_str()), std::atoll(split_data[1].c_str()), split_data[2], split_data[3], split_data[4], split_data[5], split_data[6], split_data[7], split_data[8]};
        flight_items.push_back(to_push);
        parsed[split_data[0]] = split_data;
    }

    conn.insert_flights(flight_items);
    ifs.close();
}

void processFlightDataZIP(pgconn & conn, const std::string & path){
    auto vec = readZipFile(path);
    conn.insert_flights_data(vec);
}

int zip_iterator = 0;
void thread_zip(std::vector<std::string> paths){
    pgconn conn;
    for(auto & s : paths){
        std::cout << zip_iterator++ << ": " << s << std::endl;
        processFlightDataZIP(conn, s);
    }
}

int csv_iterator = 0;
void thread_csv(std::vector<std::string> paths){
    pgconn conn;
    for(auto & s : paths){
        std::cout << csv_iterator << ": " << s << std::endl;
        processFlightCSV(conn, s);
    }
}

int main(){
    auto flightPaths = readDir(DATA_FOLDER_PATH, ".csv");
    auto zipPaths = readDir(DATA_FOLDER_PATH, ".zip");
    pgconn conn;
    conn.reset_db();

    std::vector<std::thread> threads;

    int org_size_csv = flightPaths.size();
    int tot_size_csv = 0;    
    std::size_t part_size_csv = (flightPaths.size()/N_THREADS_CSV);
    for(std::size_t i = 0; i < N_THREADS_CSV; i++){
        std::vector<std::string> paths;
        if(flightPaths.size() < part_size_csv){
            paths = std::vector<std::string>{flightPaths.begin(), flightPaths.end()};
            flightPaths = std::vector<std::string>();
        }else{
            paths = std::vector<std::string>{flightPaths.begin(), flightPaths.begin() + part_size_csv};
            flightPaths.erase(flightPaths.begin(), flightPaths.begin() + part_size_csv);
        }

        tot_size_csv += paths.size();
        threads.emplace_back(thread_csv, paths);
    }


    int org_size_zip = zipPaths.size();
    int tot_size_zip = 0;
    std::size_t part_size_zip = (zipPaths.size()/N_THREADS_ZIP) + 1;
    for(std::size_t i = 0; i < N_THREADS_ZIP; i++){
        std::vector<std::string> paths;
        if(zipPaths.size() < part_size_zip){
            paths = std::vector<std::string>{zipPaths.begin(), zipPaths.end()};
            zipPaths = std::vector<std::string>();
        }else{
            paths = std::vector<std::string>{zipPaths.begin(), zipPaths.begin() + part_size_zip};
            zipPaths.erase(zipPaths.begin(), zipPaths.begin() + part_size_zip);
        }

        tot_size_zip += paths.size();
        threads.emplace_back(thread_zip, paths);
    }

    std::cout << "CSV: org size: " << org_size_csv << "=" << tot_size_csv << ", left: " << flightPaths.size() << std::endl;
    std::cout << "ZIP: org size: " << org_size_zip << "=" << tot_size_zip << ", left: " << zipPaths.size() << std::endl;

    for(std::size_t i = 0; i < N_THREADS_CSV + N_THREADS_ZIP; i++){
        threads[i].join();
    }

    conn.add_indexes();
}
