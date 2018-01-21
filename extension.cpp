/*
 * extension.cpp
 * Author:Qihan Guan A92413483
 Duwei Wang A10169673
 * PA4
 * Date: 3/10/2016
 * For a given actor, we can find the actor in the database that 
 * can be connected to this actor with the fewest movies. 
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "ActorGraph.hpp"
#include <vector>
#include <queue>
#include <utility>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;
//overload comparison class for priority queue
class CompareAvg
{
  public:
    bool operator()(const std::pair<int, int>& p1, 
	const std::pair<int, int> &p2){
      return p1.second > p2.second;
    }
};
/* This is the main method that runs the extension. 
 * Returns: 0, otherwise vocareum complains 
 * Parameters: the 2 input parameters used to call on the program, like
 ./extension movie_casts.tsv "BACON, KEVIN (I)"
 * */
int main(int argc, char* argv[]){
  ActorGraph AGraph;
  std::ifstream in;
  std::ofstream out;

  //check for valid amount of arguments
  if (argc!=3) {
    std::cout<<"invalid number of arguments.\n";
    return -1;
  }
  //check for valid file
  in.open(argv[1], std::ios::in);
  if (!in.is_open()) {
    std::cout<<"invalid input file. \n";
    return -1;
  }
  in.seekg(0, ios_base::end);
  //check for empty file
  unsigned int len = in.tellg();
  if (len==0) {
    std::cout<<"file is empty. \n";
    return -1;
  }
  in.close();

  std::string actor = argv[2];
  std::cout<<"Loading file...\n";
  AGraph.loadFromFile(argv[1]);
  std::cout << "File loaded.\n";
  //min-heap to keep track all the shortest paths we found
  std::priority_queue<std::pair<int, int>, 
    std::vector<std::pair<int, int>>, CompareAvg> pq;
  std::cout << "Finding actor...\n";
  //find the input actor index
  int i;
  for(i = 0; i < AGraph.vertices.size(); i++){
    if(AGraph.vertices[i]->getName() == actor) break;
  }
  //if not found, exit program
  if(i == AGraph.vertices.size()){
    std::cerr << "The actor name you put in is invalid. Exiting..\n";
    exit (EXIT_FAILURE);
  }
  //if found, iterate through all other actors and find all shortest paths
  else{
    for(int j = 0; j < AGraph.vertices.size(); j++){
      //to hold the path
      std::pair<std::stack<int>,std::stack<std::string>> storage;
      //find the index, don't want to find shortest path for the same node 
      if(i != j){
	storage = AGraph.u_findShortestPath(AGraph.vertices, i, j);
	//if path has been found
	if(!storage.first.empty()){
	  //path length is how many edges in there, size is number
	  //of nodes and -1 gets you the number of edges
	  int path_length = storage.first.size() - 1;
	  //push it to min heap
	  pq.push(std::make_pair(j, path_length));
	}
      }
    }
    //if nothing in min-heap, gives a warning and exits
    if(pq.empty()){
      std::cerr << "No actor is connected to the actor you put in.\n";
      exit(EXIT_FAILURE);
    }
    //print out result 
    std::cout << "Closest actor to actor " << actor << " is: " << 
      AGraph.vertices[pq.top().first]->getName() << 
      ", with smallest path length of "  << pq.top().second << "\n";
    std::pair<std::stack<int>,std::stack<std::string>> path =
      AGraph.u_findShortestPath(AGraph.vertices, i, (pq.top().first));
    std::cout << "The path is: ";
    while(!path.first.empty()){
      path.second.pop();
      std::cout << "(" << 
	(AGraph.vertices.at(path.first.top()))->getName()<< ")";
      if(!path.second.empty()){ 
	std::cout << "--" << path.second.top()<< "-->";
      }       
      path.first.pop();
    }

    std::cout << "\n";
  }
  //vocareum complains otherwise
  return 0;

}




