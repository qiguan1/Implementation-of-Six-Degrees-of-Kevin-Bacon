/* Node.hpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * PA4
 * Date: 3/7/2016
 *
 * contains the Node class, which represents an actor. Also contains the 
 * struct Edge; edges belong to nodes. 
 */

#ifndef NODE_HPP
#define NODE_HPP
#include <string>
#include <vector>
#include <limits> 
#include <utility>
#include <cstdlib>

class Node {
  protected:
    std::string actorName;//To hold the actor's name

  public:
    //edges contain the movie title and year, as well as the weight 
    struct Edge{
      std::string movie_title_year;
      int WEIGHT;
    };

    //each node's neighbors, we store edges in adjacency vertices vector 
    std::vector<std::pair<int, Edge>> adjVertices;
    //the previous node INDEX. 
    int prev;
    //the moviename-year combined string of the edge
    std::string prevMovie;
    //boolean value indicates whether the node has been visited
    bool visited;
    //distance of node 
    int dist;
    //need to set distance to infinity and actor name to "\0"
    Node(){actorName = "\0"; dist = std::numeric_limits<int>::max();}
    ~Node(){}

    /*setter for actorName variable
     *input parameter is a string
     *returns nothing*/
    void setName(std::string name){actorName = name;}
    /*getter for actorName variable
     *no input parameter, returns a string of name*/
    std::string getName() const{return actorName;}

};

#endif // NODE_HPP
