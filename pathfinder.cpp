/*
 * Pathfinder.cpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * Date: 3/7/2016
 *
 * This program implements part 1 of the pa4, which finds the shortest path 
 * between two actors. If no path is found, empty line is outputted.
 * Arguments: file to read to construct map from,
 *  weighted or unweighted, file that has pairs to find, and output file. 
 *  It works with actorgraph.hpp.  
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

/* This is the method that loads from the file to read in pairs. 
 * Returns: a queue of actor pairs, actor 1 and actor 2. 
 * Parameters: the filename that was passed in as an argument, has pairs. 
 * */
std::queue<std::pair<std::string, std::string>>loadFromFilePair
(const char* in_filename) { 
  //same story, just load in the pairs
  std::queue<std::pair<std::string, std::string>> actorPair;
  // Initialize the file stream; basically the same as loadFromFile. 
  std::ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
    std::string s;
    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }
    std::istringstream ss( s );
    std::vector <std::string> record;

    while (ss) {
      std::string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }

    std::string actor_name1(record[0]);
    std::string actor_name2(record[1]);
    actorPair.push(std::make_pair(actor_name1, actor_name2));
  }

  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return actorPair;
  }
  infile.close();
  return actorPair;
}



/* This is the main method that runs the pathfinder. 
 * Returns:0 
 * Parameters: the 4 input parameters used to call on the program, like
 ./pathfinder movie_casts.tsv u test_pairs.tsv out2.tsv
 * */
int main(int argc, char* argv[]){
  ActorGraph AGraph;
  std::queue<std::pair<std::string, std::string>> inputActor;
  bool use_weight;
  //used for comparison 
  std::string Su ("u");
  std::string Sw ("w");
  std::ifstream in;
  std::ofstream out;

  //check for valid amount of arguments
  if (argc!=5) {
    std::cout<<"invalid number of arguments.\n";
    return -1;
  }
  //check for weighted or unweighted 
  std::string option =argv[2];
  if (option.compare(Su) == 0) {
    use_weight = false;
  }
  else if (option.compare(Sw)==0) {
    use_weight = true;
  }
  else {
    std::cout<<"invalid use of weighted or unweighted parameter.\n";
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
  in.open(argv[3], ios::in);
  if (!in.is_open()) {
    std::cout<<"invalid input file. \n";
    return -1;
  }
  in.seekg(0, ios_base::end);
  //check for empty file
  unsigned int len2 = in.tellg();
  if (len2==0) {
    std::cout<<"file is empty. \n";
    return -1;
  }

  std::cout << "Loading file...\n";
  //make the graph of actors
  AGraph.loadFromFile(argv[1]);
  std::cout << "File loaded.\n";

  //gets all of the pairs to find
  inputActor = loadFromFilePair(argv[3]);

  out.open(argv[4], ios::binary);

  std::cout << "Writing to the file...\n";
  //write the header 
  out << "(actor)--[movie#@year]-->(actor)--..." << endl;
  //while the pairs to find isn't empty 
  while (!inputActor.empty()) {
    //find index of the source actor
    int iFrom=0;
    for(iFrom; iFrom < AGraph.getVertices().size(); iFrom++){
      if( AGraph.getVertices().at(iFrom)->getName() ==inputActor.front().first)
	break;
    }

    //if actor is not in graph, exit program 
    if(iFrom == AGraph.getVertices().size()) {exit (EXIT_FAILURE);}

    //find index of the destination actor
    int iTo=0;
    for(iTo; iTo < AGraph.getVertices().size(); iTo++){
      if( AGraph.getVertices().at(iTo)->getName() == inputActor.front().second)
	break;
    }

    //if actor is not in graph, exit program
    if(iTo == AGraph.getVertices().size()) {exit (EXIT_FAILURE);}
    //stores the results from the findshortestpath. 
    std::pair<std::stack<int>,std::stack<std::string>> storage;

    //run different methods based on the use_weight boolean
    if(use_weight == false) storage = AGraph.u_findShortestPath
      (AGraph.vertices, iFrom, iTo);
    else storage = AGraph.w_findShortestPath(AGraph.vertices, iFrom, iTo);

    //output the results into the file
    //if no path found, print out a newline 
    if(storage.first.empty()) {out<<endl;} 
    while(!storage.first.empty()){
      storage.second.pop();
      out << "(" << (AGraph.vertices.at(storage.first.top()))->getName()<< ")";
      if(!storage.second.empty()){ 
	out << "--" << storage.second.top()<< "-->";
      }       
      storage.first.pop();
    }
    out << endl;      
    inputActor.pop();
  }
  //close the output file
  out.close();
  return 0;
}


