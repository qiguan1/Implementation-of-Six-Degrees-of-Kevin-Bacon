/*
 * actorconnections.cpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * Date: 3/10/2016
 * PA4
 * This program implements part 2 of the pa4, which finds the earliest year 
 * that two actors are connected. If no year is found, 9999 is written.
 * Arguments: file to read to construct map from, file testing pairs, 
 * output file, bfs option or ufind.
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

/* This is the method that loads from the file to read in pairs. 
 * Returns: a queue of actor pairs, actor 1 and actor 2. 
 * Parameters: the filename that was passed in as an argument, has pairs. 
 * */
std::vector<std::pair<std::string, std::string>> loadFromFilePair
(const char* in_filename) { 
  std::vector<std::pair<std::string, std::string>> actorPair;
  // Initialize the file stream
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

      //get the next string before hitting a tab character and put 
      //it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }
    //only have two records this time
    std::string actor_name1(record[0]);
    std::string actor_name2(record[1]);
    //make a pair, to push into actorPair
    actorPair.push_back(std::make_pair(actor_name1, actor_name2));
  }
  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return actorPair;
  }
  infile.close();
  return actorPair;
}

/* This is the method that loads from the file to read in pairs,
 * but store them in string format. This is used for outputing the
 * pairs with year the same order as the testing pairs. Later on, 
 * we will use this vector to reorder the unordered map.
 * Returns: a queue of actor pair combined strings.
 * Parameters: the filename that was passed in as an argument, has pairs. 
 * */
std::vector<std::string> loadFromFilePairCopy
(const char* in_filename) { 
  std::vector<std::string> actorPair;
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

      //get the next string before hitting a tab character and put 
      //it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }

    std::string actor_name1(record[0]);
    std::string actor_name2(record[1]);
    std::string temp = actor_name1 + "\t" + actor_name2;
    //NOT in the pair format, just a string with both names
    actorPair.push_back(temp);
  }
  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return actorPair;
  }
  infile.close();
  return actorPair;
}

/*This is the function that get the earliest year of the input file.
 *It is used to build the graph by year.
 *Input parameter:the filestream of the input file
 *Returns: an integer that has the earlies year.
 */
int getMinimumYear(const char* in_filename){
  // Initialize the file stream - given code 
  std::ifstream infile(in_filename);

  bool have_header = false;
  int min_year = std::numeric_limits<int>::max();

  // keep reading lines until the end of file is reached
  while (infile) 
  {
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
      // get the next string before hitting a tab character and put it in 
      // 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    int movie_year = stoi(record[2]);
    if(movie_year < min_year) min_year = movie_year;
  }
  //check for eof
  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  //close file 
  infile.close();
  return min_year;
}




/* This is the main method that runs the actorconnections. 
 * Returns: 0
 * Parameters: the 4 input parameters used to call on the program, like
 * ./actorconnections movie_casts.tsv test_pairs.tsv out.tsv bfs
 */
int main(int argc, char* argv[]){
  ActorGraph AGraph;
  ActorGraph AGraphUF;
  std::vector<std::pair<std::string, std::string>> inputActor;
  std::vector<std::string> inputActorCopy;
  bool use_bfs = false;
  //used for comparison 
  std::string bfsCheck ("bfs");
  std::ifstream in;
  std::ofstream out;
  //stores the results from the findshortestpath. 
  //std::vector<std::pair<std::string, int>> storage; 
  std::unordered_map<std::string, int> storage;
  //check for valid amount of arguments
  if (argc <4 || argc>5) {
    std::cout<<"invalid number of arguments.\n";
    return -1;
  }
  //check for bfs option 
  if (argc==5) {
    std::string option =argv[4];
    if (option.compare(bfsCheck) == 0) {
      use_bfs = true;
    }
  }
  //check for valid input - no neg numbers
  //check for valid file
  in.open(argv[1], std::ios::in);
  if (!in.is_open()) {
    std::cout<<"invalid input file. \n";
    return -1;
  }
  in.seekg(0, std::ios_base::end);
  //check for empty file
  unsigned int len = in.tellg();
  if (len==0) {
    std::cout<<"file is empty. \n";
    return -1;
  }
  in.close();
  in.open(argv[2], std::ios::in);
  if (!in.is_open()) {
    std::cout<<"invalid input file. \n";
    return -1;
  }
  in.seekg(0, std::ios_base::end);
  //check for empty file
  unsigned int len2 = in.tellg();
  if (len2==0) {
    std::cout<<"file is empty. \n";
    return -1;
  }
  //get the min year
  int year = getMinimumYear(argv[1]);
  //gets all of the pairs to find 
  inputActor = loadFromFilePair(argv[2]);
  inputActorCopy = loadFromFilePairCopy(argv[2]);

  out.open(argv[3], std::ios::binary);
  //write the header 
  out << "Actor1" << "\t" << "Actor2" << "\t" << "Year\n";
  //BFS option
  if (use_bfs==true) {
    //make the graph of actors
    AGraph.loadByYear(argv[1], year);

    //while the pairs to find isn't empty 
    while (!inputActor.empty() && year<2016) {
      //pop out first element in inputActors
      //find index of the two actors
      for (auto it = inputActor.begin();it != inputActor.end();) {
	bool found=false;
	//position of actor 1
	int iFrom=0;
	for(iFrom; iFrom < AGraph.getVertices().size(); iFrom++){
	  if( AGraph.getVertices().at(iFrom)->getName() == it->first)
	    break;
	}
	//position of actor 2
	int iTo=0;
	for(iTo; iTo < AGraph.getVertices().size(); iTo++){
	  if( AGraph.getVertices().at(iTo)->getName() == it->second)
	    break;
	}

	//run different methods based on the use_weight boolean
	if (iFrom < AGraph.getVertices().size() && 
	    iTo < AGraph.getVertices().size()) {
	  //load by year, check if a path has been found for that year
	  found = AGraph.BFSByYear(AGraph.vertices, iFrom, iTo);
	}
	//if the pair has a connection
	if (found==true){
	  std::string fir =  it->first;
	  std::string sec = it->second;
	  std::string temp = fir + "\t" + sec;
	  //since we load in earliest year, and increment, this
	  //guarantees the earliest year of connection. 
	  storage.insert({temp, year});
	  //this is how you iterate after erasing an element - 
	  //erase after finding it
	  it = inputActor.erase(it);
	}
	else {
	  //go to next pair and try to find it
	  ++it;
	}
      }
      //increase the year after all the pairs have been iterated through
      year = year +1;
      //load in another year and try to find the pairs again 
      AGraph.loadByYear(argv[1], year);
    }
    //if any remaining actors after all the movies have been loaded in
    if(!inputActor.empty()) {
      for(auto it = inputActor.begin();it != inputActor.end();){
	std::string fir1 = it->first;
	std::string sec1 = it->second;
	std::string temp1 = fir1 + "\t" + sec1;
	//year is 9999, per instructions
	storage.insert({temp1, 9999});
	if (!inputActor.empty()){
	  it = inputActor.erase(it);
	}
	else {
	  ++it;
	}
      }
    } 
    //since we find things in a different order than the file order, 
    //we have to
    //re-order the answers
    for(auto it = inputActorCopy.begin();it != inputActorCopy.end(); ++it){
      auto search = storage.find(*it);
      out << *it << "\t" << search->second << "\n";
    }
    out.close();
  }
  //if use ufind option or default 
  else {
    AGraphUF.MakeActorsAndMovies(argv[1]);
    AGraphUF.loadByYearUnionFind(year);
    //while the pairs to find isn't empty 
    while (!inputActor.empty() && year<2016) {
      for (auto it = inputActor.begin();it != inputActor.end();) {
	bool found=false;
	//we have the graph object that has a unionfind object that has a 
	//unordered set that we are trying to find 
	//find the location of the first actor in the pair, 
	//in the big set of actors
	auto got4 = AGraphUF.unionfind->actorSet.find(it->first);
	//find the location of the second actor
	auto got5 = AGraphUF.unionfind->actorSet.find(it->second);
	//if both actors exist in the set...
	if (got4!=AGraphUF.unionfind->actorSet.end() 
	    && got5!=AGraphUF.unionfind->actorSet.end()) {
	  //have a pointer point to the root of whatever tree the first 
	  //actor upTrieNode is
	  UnionFind::upTreeNode* temp1= 
	    AGraphUF.unionfind->findRoot(got4->second);
	  UnionFind::upTreeNode* temp2= 
	    AGraphUF.unionfind->findRoot(got5->second);
	  //if the pointers are equal, it means they've been found 
	  if (temp1->actorName == temp2->actorName) {
	    found=true;
	  }
	  if (found==true){
	    //get it ready for printing 
	    std::string fir =  it->first;
	    std::string sec = it->second;
	    std::string temp = fir + "\t" + sec;
	    storage.insert({temp, year});
	    //erase it from the list of actors
	    it = inputActor.erase(it);
	  }
	  else {
	    ++it;
	  }
	}
	else {
	  ++it;
	}
      }
      //increase year and then make more connections
      year = year +1;
      AGraphUF.loadByYearUnionFind(year);
    }
    //for unfound pairs
    if(!inputActor.empty()) {
      for(auto it = inputActor.begin();it != inputActor.end();){
	std::string fir1 = it->first;
	std::string sec1 = it->second;
	std::string temp1 = fir1 + "\t" + sec1;
	storage.insert({temp1, 9999});
	if (!inputActor.empty()){
	  it = inputActor.erase(it);
	}
	else {
	  ++it;
	}
      }
    }
    //sort the output - make sure it matches the order of original 
    //input file
    for(auto it = inputActorCopy.begin();it != inputActorCopy.end(); ++it){
      auto search = storage.find(*it);
      out << *it << "\t" << search->second << "\n";
    }
    out.close();
  }
  //vocareum complains otherwise
  return 0;
}

