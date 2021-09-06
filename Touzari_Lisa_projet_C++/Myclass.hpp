#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include"Generic_mapper.hpp"


using namespace travel;
   
class Myclass: public Generic_mapper {
public:
    
    void read_stations(const std::string& _filename) override;

    void read_connections(const std::string& _filename) override;

    std::vector<std::pair<uint64_t, uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override;
    std::vector<std::pair<uint64_t, uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override;

};

