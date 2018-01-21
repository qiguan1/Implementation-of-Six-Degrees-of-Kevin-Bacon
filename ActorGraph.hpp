/*
 * ActorGraph.hpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * Date: 3/7/2016
 * PA4
 * ActorGraph is class that holds the entire graph. Contains the method stubs
 * for 
 * ActorGraph.hpp, needed for pathfinder.cpp to work. 
 * Also has the overloaded operator for the Djikstra's weighted shortest
 * path find. 
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include "Node.hpp"
#include <utility>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include "UnionFind.hpp"
#include "Movie.hpp"

class ActorGraph {
  //make member variables public for easier access and maintainance 
  public:
    std::vector<Node *> vertices;//to store all of the vertex
    std::unordered_set<int> map;
    /*getter for vertices variable
     *returns a vector of Node pointer*/  
    std::vector<Node *> getVertices(){return vertices;}
    /*UnionFind members*/
    UnionFind *unionfind;
    Movie *movieObj;

    //constructor
    ActorGraph(){unionfind = new UnionFind(); movieObj = new Movie();}    
    //destructor delete the verices vector
    ~ActorGraph(){
      for(auto it = vertices.begin(); it != vertices.end(); ++it){
	delete *it;
      }
      delete unionfind;
      delete movieObj;
    }

    /* An unordered mutilmap with moviename-year as key and actor 
     * index as element. 
     * Used when making edges between two actors if they're in same movie.   
     */
    std::unordered_multimap<std::string, int> movie_cast_map; 

    /*Function to find the unweighted shortest path using BFS.
     *returns: a stack of strings and stack of ints, paired together.
     *parameters: the whole graph, and the indexes of the actors to find. 
     */
    std::pair<std::stack<int>,std::stack<std::string>> u_findShortestPath
      (std::vector<Node*>& theGraph, int from, int to);

    /*Function to find the weighted cheapest path using Djikstra's Algorithm.
     *returns: a stack of strings and stack of ints, paired together
     *parameters: the whole graph, and the indexes of the actors to find*/
    std::pair<std::stack<int>,std::stack<std::string>> w_findShortestPath
      (std::vector<Node*>& theGraph,  int from, int to);

    /* Load the graph from a tab-delimited file of actor->movie relationships.
     * in_filename - input filename
     * Works for both weighted and unweighted option. 
     * Return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename);

    /* This is the method that loads from the file 
     * Returns: true or false, whether it successfully loaded file
     * Parameters: the filename that was passed in as an argument
     * */
    bool loadByYear(const char* in_filename, int year);

    /*A function used in part2, it checks if there is a path between
     *the two actor.
     *Input parameters: graph vertices vector, source actor index,
     *destination actor index.
     *Return true, if there is a path between the two actors
     *Return false, if there is no path between the two actors
     */      
    bool BFSByYear(std::vector<Node*>& theGraph, int from, int to);

    /*Union find methods*/
    /* This is the method that joins movies year by year; joins all the member
     * of a cast of a movie together
     * Returns: void
     * Parameters: year to load things from
     * */
    void loadByYearUnionFind(int year); 
    
    /*This method makes a bunch of actor nodes and movie node objects.
     * The actor nodes are not connected to each other at this point 
     * parameters: const char* in_filename, the filename to be read in 
     * return: bool, indicating success or failure of reading file 
     */
    bool MakeActorsAndMovies(const char* in_filename);
};


#endif // ACTORGRAPH_HPP
