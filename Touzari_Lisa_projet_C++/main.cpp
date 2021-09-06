#include <iostream>
#include <string.h>
#include <unordered_map>

#include "Myclass.hpp"
#include "Grade.hpp"

using namespace travel;
int main(void) {
    Myclass new_object ; 
    
    std::string file1 = "s.csv"; 
    std::string file2 = "c.csv";
    
    std::uint64_t stat_depart;
    std::uint64_t stat_arrivee;
    std::uint64_t test;
    
    new_object.read_stations(file1);
    new_object.read_connections(file2);

    // test de read_stations
    std::unordered_map<uint64_t, Station> stations_hashmap;
    std::cout<<"test de la méthode read_stations"<<std::endl;    
    stations_hashmap=new_object.get_stations_hashmap();
    

    // test de read_connections
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > connections_hashmap; 
    std::cout<<"test de la méthode read_connections"<<std::endl;    
    connections_hashmap = new_object.get_connections_hashmap();
    

    // test de compute_and_display_travel
    std::cout<< "donner la station de depart"<<std::endl;
    std::cin>> stat_depart;
    
    std::cout<< "donner la station arrivee"<<std::endl;
    std::cin>> stat_arrivee;
    
    std::vector<std::pair<uint64_t, uint64_t> > best_path;    
    best_path = new_object.compute_and_display_travel(stat_depart, stat_arrivee);  
   
    std::cout<<"voulez vous tester le grade: 0, 1"<<std::endl;
    std::cin>>test; 
    
    if (test==1){
    	Grade grader(false);
    	grader.dijkstra(new_object);
    }
    return 0;
}

