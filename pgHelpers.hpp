#pragma once
#include <pqxx/pqxx>
#include <vector>
#include <string>

#include "structs.hpp"

class pgconn{
    pqxx::connection * conn;
public:
    pgconn();
    ~pgconn();
    void insert_flights(const std::vector<flight_item> & flights);
    void insert_flights_data(const std::vector<data_item> & data);
    void reset_db();
    void add_indexes();
};

