/*
 * ActorGraph.cpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * PA4 
 * Date: 3/7/2016
 *
 * Works with pathfinder.cpp, actorconnections.cpp, extension.cpp. 
 * Has the two different find shortest path methods, depending on
 * weighted or unweighted options.
 * Has different loading methods for the BFS and UnionFind as well
 * Extension.cpp has no special methods for itself
 */

#include <fstream>
#include <iostream>
#include "ActorGraph.hpp"
#include <queue>
#include <algorithm>


/* This is the method that loads from the file.
 * Assume the input file is grouped by actor name. 
 * Returns: true or false, whether it successfully loaded file
 * Parameters: the filename that was passed in as an argument
 * */
bool ActorGraph::loadFromFile(const char* in_filename) {
  // Initialize the file stream - given code 
  std::ifstream infile(in_filename);

  bool have_header = false;

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
      //get the next string before hitting a tab character and put it 
      //in 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    std::string actor_name(record[0]);
    std::string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    //how you print things out 
    std::string buffer="["+movie_title+"#@"+std::to_string(movie_year)+"]";
    //weight is calculated regardless of 'w' or 'u' option 
    int weight = 1 + (2015 - movie_year);
    //nobody in there
    if(vertices.empty()) {
      Node* vertex = new Node();
      vertex->setName(actor_name);
      movie_cast_map.insert({buffer, 0});
      vertices.push_back(vertex);
    }
    //actor is already there 
    else if(vertices.back()->getName() == actor_name) {
      movie_cast_map.insert({buffer, vertices.size()-1});
      //create edge
      auto range = movie_cast_map.equal_range(buffer);
      for(auto it = range.first; it != range.second; ++it){
	Node::Edge edge;
	edge.movie_title_year = buffer;
	edge.WEIGHT = weight;
	if(it->second != (vertices.size()-1)){
	  vertices.back()->adjVertices.push_back
	    (std::pair<int, Node::Edge>(it->second, edge));
	  vertices[it->second]->adjVertices.push_back
	    (std::pair<int, Node::Edge>((vertices.size()-1), edge));
	}
      }
    }
    //actor is not there
    else if(vertices.back()->getName() != actor_name){

      Node* vertex1 = new Node();
      vertex1->setName(actor_name);
      movie_cast_map.insert({buffer, vertices.size()});
      //create edge
      auto range = movie_cast_map.equal_range(buffer);

      for(auto it = range.first; it != range.second; ++it){

	Node::Edge edge;
	edge.movie_title_year = buffer;
	edge.WEIGHT = weight;
	if(it->second != vertices.size()){
	  vertex1->adjVertices.push_back(std::pair<int, 
	      Node::Edge>(it->second, edge));
	  vertices[it->second]->adjVertices.push_back
	    (std::pair<int, Node::Edge>(vertices.size(), edge));
	}          
      }

      vertices.push_back(vertex1);

    }
  }
  //check for eof
  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  //close file 
  infile.close();
  return true;
}

/* This is the method that loads from the file 
 * Returns: true or false, whether it successfully loaded file
 * Parameters: the filename that was passed in as an argument
 * */
bool ActorGraph::loadByYear(const char* in_filename, int year){
  // Initialize the file stream - given code 
  std::ifstream infile(in_filename);

  bool have_header = false;

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
      //get the next string before hitting a tab character and put it
      //in 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    std::string actor_name(record[0]);
    std::string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    //how you print things out 
    std::string buffer="["+movie_title+"#@"+std::to_string(movie_year)+"]";
    //weight is calculated regardless of 'w' or 'u' option 
    int weight = 1 + (2015 - movie_year);
    if (movie_year ==year) { 

      //if no nodes yet
      if(vertices.empty()) {
	Node* vertex = new Node();
	vertex->setName(actor_name);
	movie_cast_map.insert({buffer, 0});
	vertices.push_back(vertex);
      }
      int index;
      //actor is already there 			
      for (index = 0;index < vertices.size();index++) {
	if (vertices[index]->getName() == actor_name) {
	  break;
	}
      }
      if(index < vertices.size()){
	movie_cast_map.insert({buffer, index});
	//create edge
	auto range = movie_cast_map.equal_range(buffer);
	for(auto it = range.first; it != range.second; ++it){
	  Node::Edge edge;
	  edge.movie_title_year = buffer;
	  edge.WEIGHT = weight;
	  if(it->second != index){
	    vertices[index]->adjVertices.push_back(
		std::pair<int, Node::Edge>(it->second, edge));
	    vertices[it->second]->adjVertices.push_back(
		std::pair<int, Node::Edge>(index, edge));
	  }
	}
      }

      //actor is not there
      else if(index == vertices.size()){
	Node* vertex1 = new Node();
	vertex1->setName(actor_name);
	movie_cast_map.insert({buffer, vertices.size()});
	//create edge
	auto range = movie_cast_map.equal_range(buffer);
	for(auto it = range.first; it != range.second; ++it){

	  Node::Edge edge;
	  edge.movie_title_year = buffer;
	  edge.WEIGHT = weight;
	  if(it->second != vertices.size()){
	    vertex1->adjVertices.push_back(
		std::pair<int, Node::Edge>(it->second, edge));
	    vertices[it->second]->adjVertices.push_back(
		std::pair<int, Node::Edge>(vertices.size(), edge));
	  }          
	}
	vertices.push_back(vertex1);

      }

    }
  }
  //check for eof
  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  //close file 
  infile.close();
  return true;
}


/* unweighted find shortest path, using BFS
 * returns: a stack of strings and stack of ints, paired together
 * parameters: the whole graph, and the indexes of the actors
 * to find */
std::pair<std::stack<int>,std::stack<std::string>> 
ActorGraph::u_findShortestPath(
    std::vector<Node*>& theGraph, int from, int to){
  //to be returned
  std::pair<std::stack<int>,std::stack<std::string>> results;
  //to iterate over adjVertices
  std::vector<std::pair<int,Node::Edge>>::iterator itAdj;  
  //queue up nodes to explore 
  std::queue<int> toExplore;
  for(int i = 0; i < theGraph.size(); i++){
    theGraph[i]->prevMovie = "\0";
    theGraph[i]->prev = -1;
    theGraph[i]->visited = false;
  }
  theGraph[from]->dist = 0;
  theGraph[from]->visited = true;
  toExplore.push(from);
  //using bfs to search  
  while ( !toExplore.empty()) {
    int next = toExplore.front();
    toExplore.pop();
    itAdj= theGraph[next]->adjVertices.begin();
    for ( ; itAdj != theGraph[next]->adjVertices.end(); ++itAdj ){
      if(!theGraph[itAdj->first]->visited){
	(theGraph[itAdj->first])->visited = true;
	(theGraph[itAdj->first])->dist = 1+(theGraph[next])->dist;
	theGraph[itAdj->first]->prev = next;
	theGraph[itAdj->first]->prevMovie=(itAdj->second).movie_title_year;
	toExplore.push(itAdj->first);
      }
      //up to the actor we want to reach in the shortest path 
      if(itAdj->first == to){
	while(!toExplore.empty())
	  toExplore.pop();
	break;
      }
    }
  }//end while
  //push the node index and movie title/year together to the results
  for(int v = to; v >=0; v = theGraph[v]->prev){
    if(theGraph[to]->prev >= 0){
      results.first.push(v);
      results.second.push(theGraph[v]->prevMovie);
    }
  }
  return results;
}

//Container is a helper struct for weighted find shortest path
struct Container{
  int vertex;
  int prev;
  int cost;
  std::string prevMovie;
  //operator overload 
  bool operator<(const Container& v) const{
    return cost > v.cost;
  }
}; 

/* weighted find shortest path, using Djikstra's 
 * returns: a stack of strings and stack of ints, paired together
 * parameters: the whole graph, and the indexes of the actors
 * to find */
std::pair<std::stack<int>,std::stack<std::string>> 
ActorGraph::w_findShortestPath(std::vector<Node*>& theGraph, 
    int from, int to){
  std::pair<std::stack<int>,std::stack<std::string>> results;
  //to iterate over adjVertices
  std::vector<std::pair<int,Node::Edge>>::iterator itAdj;  

  //initialize all Vertexs dist field to INFINITY
  for(int i = 0; i < theGraph.size(); i++){
    theGraph[i]->prevMovie = "\0";
    theGraph[i]->prev = -1;
    theGraph[i]->visited = false;
    theGraph[i]->dist = std::numeric_limits<int>::max();
  }
  //holds the nodes to explore
  std::priority_queue<Container> toExplore;
  //initialize a container, push first element in
  Container start = {from, -1, 0, "\0"};
  toExplore.push(start);
  while(!toExplore.empty()){
    Container obj = toExplore.top();
    //pop out the node 
    toExplore.pop();
    if(theGraph[obj.vertex]->visited) continue;
    theGraph[obj.vertex]->visited = true;
    //calculate the distance
    theGraph[obj.vertex]->dist = obj.cost;
    theGraph[obj.vertex]->prev = obj.prev;
    theGraph[obj.vertex]->prevMovie = obj.prevMovie; 
    if(obj.vertex == to) break;
    //to deal with duplicate and update priority
    std::unordered_map<int, std::pair<int, std::string>> temp;
    //iterate through the graph vertices vector
    for(itAdj = theGraph[obj.vertex]->adjVertices.begin(); 
	itAdj != theGraph[obj.vertex]->adjVertices.end(); ++itAdj){
      //if not visited before, then make iterator
      if(!theGraph[itAdj->first]->visited){
	std::unordered_map<int, std::pair<int,std::string>>::const_iterator 
	  got = temp.find(itAdj->first);
	//if duplicate not found, insert it into unordered map  
	if(got == temp.end()){
	  temp.insert({itAdj->first, std::pair<int, std::string>(
		(itAdj->second).WEIGHT +
		obj.cost,(itAdj->second).movie_title_year) });
	}
	//otherwise, update priority of the old one   
	else{
	  if(((itAdj->second).WEIGHT + obj.cost) <temp[itAdj->first].first)
	    temp[itAdj->first] = std::make_pair(
		(itAdj->second).WEIGHT+obj.cost, 
		(itAdj->second).movie_title_year );
	}
      }
    }
    //at the end, put container object into toExplore based on temp
    for(auto it = temp.begin(); it != temp.end(); ++it){
      Container new_obj = {it->first, obj.vertex, 
	(it->second).first, (it->second).second};
      toExplore.push(new_obj);
    }
  }


  //push together the results 
  for(int v = to; v >=0; v = theGraph[v]->prev){
    if(theGraph[to]->prev >= 0){
      results.first.push(v);
      results.second.push(theGraph[v]->prevMovie);
    }
  }
  return results;  
}


/*A function used in part2, it checks if there is a path between
 *the two actor.
 *Input parameters: graph vertices vector, source actor index,
 *destination actor index.
 *Return true, if there is a path between the two actors
 *Return false, if there is no path between the two actors
 */        
bool ActorGraph::BFSByYear(
    std::vector<Node*>& theGraph, int from, int to){
  //to be returned
  std::stack<int> results;
  //to iterate over adjVertices
  std::vector<std::pair<int,Node::Edge>>::iterator itAdj;  
  //queue up nodes to explore 
  std::queue<int> toExplore;
  for(int i = 0; i < theGraph.size(); i++){
    theGraph[i]->prev = -1;
    theGraph[i]->visited = false;
  }
  //still need distance to traverse bfs
  theGraph[from]->dist = 0;
  theGraph[from]->visited = true;
  toExplore.push(from);
  //using bfs to search  
  while ( !toExplore.empty()) {
    int next = toExplore.front();
    toExplore.pop();
    itAdj= theGraph[next]->adjVertices.begin();
    //iterate through the neighbors
    for ( ; itAdj != theGraph[next]->adjVertices.end(); ++itAdj ){
      if(!theGraph[itAdj->first]->visited){
	//mark as visited
	(theGraph[itAdj->first])->visited = true;
	(theGraph[itAdj->first])->dist = 1+(theGraph[next])->dist;
	theGraph[itAdj->first]->prev = next;
	toExplore.push(itAdj->first);
      }
      //up to the actor we want to reach in the shortest path 
      if(itAdj->first == to){
	while(!toExplore.empty())
	  toExplore.pop();
	break;
      }
    }
  }//end while

  //push the node index and movie title/year together to the results
  for(int v = to; v >=0; v = theGraph[v]->prev){
    if(theGraph[to]->prev >=0){
      results.push(v);
    }
  }
  //if there is nothing in results, it means path not found
  if(!results.empty()) return true;
  //only need to return a boolean because we already know the year
  else return false;
}




/*Union find methods*/
/*This method makes a bunch of actor nodes and movie node objects.
 * The actor nodes are not connected to each other at this point 
 * parameters: const char* in_filename, the filename to be read in 
 * return: bool, indicating success or failure of reading file 
 */
bool ActorGraph::MakeActorsAndMovies(const char* in_filename) {
  // Initialize the file stream - given code 
  std::ifstream infile(in_filename);
  bool have_header = false;
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
      // get the next string before hitting a tab character and 
      // put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    std::string actor_name(record[0]);
    std::string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    //how you print things out 
    std::string buffer="["+movie_title+"#@"+std::to_string(movie_year)+"]";
    //no actor in there
    if(unionfind->actorSet.empty()) {
      UnionFind::upTreeNode* vertex = new UnionFind::upTreeNode();
      vertex->actorName = actor_name;
      vertex->size = 1;
      vertex->parent = vertex;
      unionfind->actorSet.insert(std::make_pair(actor_name, vertex));
      Movie::MovieNode* m = new Movie::MovieNode();
      m->movieNY = movie_title;
      m->year = movie_year;
      m->cast.insert({actor_name, vertex});
      movieObj->allMovies.insert(make_pair(buffer, m));
      movieObj->moviesByYear.insert(std::make_pair(movie_year, m));

    }
    //check for movie
    auto gotM = movieObj->allMovies.find(buffer);
    //check for position of actor
    auto gotActor = unionfind->actorSet.find(actor_name);
    //movie is there
    if (gotM!=movieObj->allMovies.end()){
      //actor is there
      if ( gotActor != unionfind->actorSet.end() ) {  
	((gotM->second)->cast).insert({actor_name, gotActor->second});
      } 
      //actor is not there 
      else {	
	UnionFind::upTreeNode * vertex1 = new UnionFind::upTreeNode();
	vertex1->actorName = actor_name;
	vertex1->size = 1;
	vertex1->parent = vertex1;
	unionfind->actorSet.insert(std::make_pair(actor_name, vertex1));   
	((gotM->second)->cast).insert({actor_name, vertex1});
      }
    }

    //if neither movie nor actor is there
    if (gotM==movieObj->allMovies.end() && gotActor == 
	unionfind->actorSet.end()) {
      UnionFind::upTreeNode* vertex2 = new UnionFind::upTreeNode();
      vertex2->actorName = actor_name;
      vertex2->size = 1;
      vertex2->parent = vertex2;
      unionfind->actorSet.insert(std::make_pair(actor_name, vertex2));
      Movie::MovieNode* m1 = new Movie::MovieNode();
      m1->movieNY = movie_title;
      m1->year = movie_year;
      m1->cast.insert({actor_name, vertex2});
      movieObj->allMovies.insert(std::make_pair(buffer, m1));
      movieObj->moviesByYear.insert(std::make_pair(movie_year, m1));
    }
    if (gotM==movieObj->allMovies.end() && gotActor != 
	unionfind->actorSet.end()) {
      Movie::MovieNode* m1 = new Movie::MovieNode();
      m1->movieNY = movie_title;
      m1->year = movie_year;
      m1->cast.insert({actor_name, gotActor->second});
      movieObj->allMovies.insert(std::make_pair(buffer, m1));
      movieObj->moviesByYear.insert(std::make_pair(movie_year, m1));
    }
  }  //check for eof
  if (!infile.eof()) {
    std::cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  //close file 
  infile.close();
  return true;
}



/* This is the method that joins movies year by year; joins all the member
 * of a cast of a movie together
 * Returns: void
 * Parameters: year to load things from
 * */
void ActorGraph::loadByYearUnionFind(int year){
  auto rangeYears = (movieObj->moviesByYear).equal_range(year);
  for (auto iter = rangeYears.first; iter!=rangeYears.second;++iter){
    //movie found? join up all its actors
    for (auto it = iter->second->cast.begin();
	it !=iter->second->cast.end();++it) {
      unionfind->unionJoin(((iter->second->cast.begin())->second), 
	  (it->second));
    }
  }
}


