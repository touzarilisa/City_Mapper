#include<fstream>
#include "Myclass.hpp"
#include <string.h>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <new>
const uint64_t INF  = 100000;
using namespace travel;

// methode read_stations
void Myclass::read_stations(const std::string& _filename) {
    std::ifstream fichier;
    fichier.open(_filename, std::ios::in);

    std::string ligne;    
    std::string delim = ",";
    std::vector<std::string> sub_ligne;

    if (fichier) {
        int num_ligne = 0;

        while (getline(fichier, ligne)) {
            
            if (num_ligne != 0) {
                struct Station new_station;
                                
                int i = 0;
                auto start = 0U;
                auto end = ligne.find(delim);
                    
                while (end != std::string::npos) {
                    i = end;
                    sub_ligne.push_back( ligne.substr(start, end - start));
                    start = end + delim.length();
                    end = ligne.find(delim, start);
                    
                }
                sub_ligne.push_back( ligne.substr(i+1, ligne.size()));

                new_station.name = sub_ligne.at(0);
                new_station.line_id = sub_ligne.at(2);
                new_station.address = sub_ligne.at(3);
                new_station.line_name =sub_ligne.at(4);
                this->stations_hashmap.insert(std::pair<uint64_t, Station>(std::stoi(sub_ligne.at(1)), new_station)) ;
            }
            num_ligne++;
            sub_ligne.clear();
        }
        fichier.close();
    }
    else {
        std::cout << "erreur de fichier" << std::endl;
    }
        
}

// methode read_connections

void Myclass::read_connections(const std::string& _filename) {
    std::ifstream fichier;
    fichier.open(_filename, std::ios::in);

    std::string ligne;
    std::vector<std::string> sub_ligne;
    std::string delim = ",";

    if (fichier) {
        int num_ligne = 0;

        while (getline(fichier, ligne)) {
            if (num_ligne != 0) {
                              
                int i = 0;
                auto start = 0U;
                auto end = ligne.find(delim);

                while (end != std::string::npos) {
                    i = end;
                    sub_ligne.push_back(ligne.substr(start, end - start));
                    start = end + delim.length();
                    end = ligne.find(delim, start);
                    
                }
                sub_ligne.push_back(ligne.substr(i + 1, ligne.size()));

                this->connections_hashmap[std::stoi(sub_ligne.at(0))][std::stoi(sub_ligne.at(1))]=std::stoi(sub_ligne.at(2));
            }
            num_ligne++;
            sub_ligne.clear();
        }
        fichier.close();
    }
    else {
        std::cout << "erreur de fichier" << std::endl;
    }

}

std::vector<std::pair<uint64_t, uint64_t> > Myclass::compute_travel(uint64_t _start, uint64_t _end) {

    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > graphe(this->connections_hashmap);
    std::vector<std::pair<uint64_t, uint64_t> > best_path; // (numero station,distance_start)
    std::unordered_map<uint64_t, uint64_t> distance; //  (numero station,distance_start)
    std::unordered_map<uint64_t, uint64_t> Q; //  (numero station,distance_start)

    std::unordered_map<uint64_t, uint64_t> voisins; //  (numero station_voisine,distance)
    std::unordered_map<uint64_t, uint64_t> predec; //  (numero station,predecesseur)

    std::pair<uint64_t, uint64_t> source(_start, 0);  // (numero station,distance_start)
    std::pair<uint64_t, uint64_t> destination(_end, INF); // (numero station,distance_start)

    std::pair<uint64_t, uint64_t> u(source); //le noeud avec la plus petite distance à la source
    std::uint64_t dist; // variable de calcule de distance


    // initialisation de toutes les distances vers la source à l'infini
    // initilisation de l'nesemble des noeuds
    try {
        if (_start == _end)
            throw std::string("la station de depart est la meme que la station d'arrivee");
        else if (this->stations_hashmap.find(_start) == this->stations_hashmap.end() || this->stations_hashmap.find(_end) == this->stations_hashmap.end())
            throw std::string("la station n'existe pas");
            	
    }
    catch (std::string const& erreur) {
        std::cerr << "Erreur : " << erreur << std::endl;
        std::exit(1);

    }
    
    catch (std::bad_alloc const& e) {
    	std::cerr << "Erreur : plus assez de mémoire !" << std::endl;
    	exit (EXIT_FAILURE);
	}
        
    for (std::unordered_map<uint64_t, Station> ::iterator i(this->stations_hashmap.begin()); i != this->stations_hashmap.end(); ++i) {
        distance.insert(std::pair<uint64_t, uint64_t>(i->first, INF));
        Q.insert(std::pair<uint64_t, uint64_t>(i->first, INF));
    }


    // distance de la source à zero
    distance[_start]= 0;
    Q[_start]= 0;

    while (!Q.empty() && u.first != _end) {

        //recuperer le noeud avec la plus petite distance
        u.second = INF;
        for (std::unordered_map<uint64_t, uint64_t>::iterator i(Q.begin()); i != Q.end(); ++i) {
            if (i->second < u.second) {
                u = std::make_pair(i->first, i->second);
            }
        }

        // supprimer le noeud
        Q.erase(u.first);
        //voisins du noeud u
        for (std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > ::iterator i(graphe.begin()); i != graphe.end(); ++i) {
            if (i->first == u.first) {
                for (std::unordered_map<uint64_t, uint64_t>::iterator j(i->second.begin()); j != i->second.end(); ++j) {
                    voisins[j->first] = j->second;                    
                    graphe[j->first][u.first] = INF;
                }
            }
        }

        // supprimer les connections deja vu
        graphe.erase(u.first);
        // mise a jour des distance par rapport à la source
        for (std::unordered_map<uint64_t, uint64_t>::iterator i(voisins.begin()); i != voisins.end(); ++i) {
            dist = u.second + i->second;
            if (dist < distance[i->first]) {
                
                distance[i->first] = dist;
                Q[i->first] = dist;
                predec[i->first] = u.first;
            }
        }
        voisins.clear();

    }

 
    destination.second = u.second; // le noeud d'arrivee, temps écoulé final
    best_path.push_back(std::make_pair(u.first, u.second)); // on initialise avec la station d'arrivée 

    std::uint64_t num_noeud(predec[destination.first]);

    while (num_noeud != _start) {
        best_path.push_back(std::make_pair(num_noeud, distance[num_noeud])); // on recupere l'ensmble des predecesseur 
                                                                             // de la station d'arrivee  jusqu'au depart
        num_noeud = predec[num_noeud];
    }
    best_path.push_back(source);
    std::reverse(best_path.begin(), best_path.end()); // on inverse les stations pour aller de _start à _end

    return best_path;
}

std::vector<std::pair<uint64_t, uint64_t> > Myclass::compute_and_display_travel(uint64_t _start, uint64_t _end) {
    std::vector<std::pair<uint64_t, uint64_t> > best_path;
    std::pair<uint64_t, uint64_t> station_preced(_start, 0);
    std::pair<uint64_t, uint64_t> station_debut_ligne;
    std::pair<uint64_t, uint64_t> station_fin_ligne;
    std::uint64_t count(0);
    best_path = compute_travel(_start, _end);

    std::cout << "Best way from " << this->stations_hashmap[_start].name << " (line " << this->stations_hashmap[_start].line_id << ") to "
        << this->stations_hashmap[_end].name << " (line " << this->stations_hashmap[_end].line_id << ")" << std::endl;

    for (std::vector<std::pair<uint64_t, uint64_t> >::iterator i(best_path.begin() + 1); i != best_path.end(); ++i) {

        if (this->stations_hashmap[i->first].name == this->stations_hashmap[station_preced.first].name) {
            std::cout << "Walk to " << this->stations_hashmap[i->first].name << " ,line " <<
                this->stations_hashmap[i->first].line_id << " ( "
                << i->second - station_preced.second << " secs ) " << std::endl;
        }

        if (this->stations_hashmap[i->first].line_id != this->stations_hashmap[station_preced.first].line_id) {
            station_debut_ligne.first = i->first;
            station_debut_ligne.second = i->second;

            station_fin_ligne = station_debut_ligne;

            std::cout << "Take line " << this->stations_hashmap[i->first].line_id << " " << this->stations_hashmap[i->first].line_name << std::endl;

            for (std::vector<std::pair<uint64_t, uint64_t> >::iterator j(best_path.begin() + count + 1); j != best_path.end(); ++j) {

                if (this->stations_hashmap[j->first].line_name != this->stations_hashmap[station_fin_ligne.first].line_name
                    && this->stations_hashmap[station_debut_ligne.first].line_id == this->stations_hashmap[station_fin_ligne.first].line_id) {

                    std::cout << "From " << this->stations_hashmap[station_debut_ligne.first].name << " to " 
                        << this->stations_hashmap[station_fin_ligne.first].name <<" ( "
                        <<station_fin_ligne.second-station_debut_ligne.second << " secs ) " <<std::endl;
                }
                station_fin_ligne.first = j->first;
                station_fin_ligne.second = j->second;

            }

        }
        station_preced.first = i->first;
        station_preced.second = i->second;
        count++;

    }
    std::cout << "After " << best_path.back().second << " secs, you have reached your destination!" << std::endl;
    return best_path;
}


    


