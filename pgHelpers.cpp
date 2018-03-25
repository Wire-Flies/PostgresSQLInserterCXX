#include "pgHelpers.hpp"
#include <iostream>

pgconn::pgconn(){
    conn = new pqxx::connection(
        "user=wireflies "
        "host=localhost "
        "password=wireflies "
        "dbname=wireflies");
}

pgconn::~pgconn(){
    delete conn;
}

void pgconn::reset_db(){
    pqxx::work work(*conn);
    work.exec("DROP TABLE IF EXISTS flights");
    work.exec("DROP TABLE IF EXISTS flight_data");
    work.exec("DROP INDEX IF EXISTS flight_id_index");
    work.exec("CREATE UNLOGGED TABLE IF NOT EXISTS flights ( flight_id bigint PRIMARY KEY,\
                aircraft_id bigint,\
                reg VARCHAR,\
                equip VARCHAR,\
                callsign VARCHAR,\
                flight VARCHAR,\
                schd_from VARCHAR,\
                schd_to VARCHAR,\
                real_to VARCHAR,\
                reserved VARCHAR)");
    work.exec("CREATE UNLOGGED TABLE IF NOT EXISTS flight_data (id bigserial PRIMARY KEY,\
                flight_id bigint,\
                snapshot_id bigint NOT NULL,\
                altitude int,\
                heading int,\
                latitude real,\
                longitude real,\
                radar_id int,\
                speed int,\
                squawk VARCHAR)");
    work.commit();
}

void pgconn::add_indexes(){
    pqxx::work work(*conn);
    work.exec("CREATE INDEX flight_id_index ON flight_data (flight_id);");
    work.exec("CREATE INDEX flight_snapshot_index ON flight_data (snapshot_id);");
    //work.exec("CREATE INDEX flight_schd_from_index ON flight_data (schd_from);")
    //work.exec("CREATE INDEX flight_schd_to_index ON flight_data (schd_to);");
    //work.exec("")
    work.commit();
}

void pgconn::insert_flights(const std::vector<flight_item> & info){
    pqxx::work work(*conn);
    std::cout << "size of insert: " << info.size() << std::endl;
    for(auto & i : info){
        std::string query = "INSERT INTO flights (flight_id,aircraft_id,reg,equip,callsign,flight,schd_from,schd_to,real_to) VALUES (" + std::to_string(i.flight_id) \
            + ", " + std::to_string(i.aircraft_id) + ", '" + i.reg + "', '" + i.equip + "', '" + i.callsign + "', '" + i.flight + "', '" + i.schd_from + "', '" + i.schd_to \
            + "', '" + i.real_to + "') ON CONFLICT DO NOTHING;";
        work.exec(query);
    }
    work.commit();
}

void pgconn::insert_flights_data(const std::vector<data_item> & data){
    pqxx::work work(*conn);
    std::cout << "size of data insert: " << data.size() << std::endl;
    for(auto & d : data){
        std::string query = "INSERT INTO flight_data (flight_id,snapshot_id,altitude,heading,latitude,longitude,radar_id,speed,squawk) VALUES ("
            + std::to_string(d.flight_id) + ", " + std::to_string(d.snapshot_id) + ", " + std::to_string(d.altitude) + ", " + std::to_string(d.heading) + ", "
            + std::to_string(d.latitude) + ", " + std::to_string(d.longitude) + ", " + std::to_string(d.radar_id) + ", " + std::to_string(d.speed) + ", "
            + std::to_string(d.squawk) + ") ON CONFLICT DO NOTHING;";
        work.exec(query);
    }
    work.commit();
}
