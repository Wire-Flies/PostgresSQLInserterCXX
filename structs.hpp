#pragma once
#include <string>
typedef long long int ull;
struct flight_item{
    ull flight_id;
    ull aircraft_id;
    std::string reg;
    std::string equip;
    std::string callsign;
    std::string flight;
    std::string schd_from;
    std::string schd_to;
    std::string real_to;
};

struct data_item{
    ull flight_id;
    ull snapshot_id;
    ull altitude;
    ull heading;
    double latitude;;
    double longitude;
    ull radar_id;
    ull speed;
    ull squawk;
};

