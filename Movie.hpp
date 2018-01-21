/*
 * Movie.hpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * Date: 3/7/2016
 *
 * contains the Movie class, which was used to keep track movies. 
 * Has a struct MovieNope, which has the cast. For UnionFind only. 
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <limits> 
#include <utility>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include "UnionFind.hpp"
#include <unordered_map>
//this Class is really just a container for MovieNodes
class Movie {
  protected:
    //everything is public for easy access
  public:
    //each MovieNode 
    struct MovieNode {
      //the movie name and year, actually in the output format
      std::string movieNY;
      int year;
      //the cast of the movie. Consists of upTreeNode pointers! 
      std::unordered_map<std::string, UnionFind::upTreeNode *> cast;
    };

    Movie() {};
    ~Movie(){
      for(auto it = allMovies.begin(); it != allMovies.end(); ++it)
	delete it->second;
    };
    //keeps track of all the movies so that they can be searched by NAME 
    //Only used for the building of the graph
    std::unordered_map<std::string, MovieNode *> allMovies;
    //keeps track of all the movies so that they can be searched, 
    //multiples at a time, by YEAR. Only used for building by year
    std::unordered_multimap<int, MovieNode *> moviesByYear;
};

#endif // MOVIE_HPP
