/* UnionFind.hpp
 * Author: Duwei Wang, A10169673; 
 * Qihan Guan, A92413483;
 * PA4
 * Date: 3/7/2016
 *
 * contains the UnionFind class, which is an object just for implementing
 * UnhionFind. Contains an upTreeNode struct, as well as the two needed
 * unionFind methods of find and union. 
 */

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP
#include <string>
#include <vector>
#include <limits> 
#include <utility>
#include <cstdlib>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <utility>
#include <fstream>
#include <iterator>
#include <cstdlib>
//This class defines all of the UnionFind functionalities
class UnionFind{
  protected:
    //each node's neighbors, we store edges in adjacency vertices vector 
  public:
    struct upTreeNode{
      std::string actorName;
      upTreeNode * parent = this; //point to itself initially 
      //SIZE OF TREE - only root has updated value
      int size = 1;
    };

    //edges contain the movie title and year, as well as the weight 
    UnionFind(){}
    ~UnionFind(){
      for(auto it = actorSet.begin(); it != actorSet.end(); ++it){
        delete it->second;
      }
    }
    //holds all the actors together to keep track of reinsertion
    std::unordered_map <std::string, upTreeNode *> actorSet; 

    /* This method finds the root of whatever node pointer is passed in;
     * also attempts to recursively do path compression. 
     * parameter: upTreeNode pointer; want to find root of it
     * return: upTreeNode pointer; points to the root of parameter
     * */
    upTreeNode * findRoot(upTreeNode * temp) {
      if (temp != temp->parent) {
	temp->parent = findRoot(temp->parent);
      }
      return temp->parent;
    }

    /*This method joins two sets together,according to which one is smaller 
     * Paramters: two upTreeNode pointers, the nodes
     * (and the set they're in) to be joined together
     * return: void
     * */
    void unionJoin(upTreeNode * currA, upTreeNode * currB) {
      //first find the root of both upTreeNodes
      upTreeNode * rootOfA = findRoot(currA);
      upTreeNode * rootOfB = findRoot(currB);
      //std::cerr << temp->actorName << "--" << temp1->actorName << "\n";
      //Don't join if hte root is same, meaning they're in same set
      if(rootOfA->actorName == rootOfB->actorName) return;
      //set the root of A to the root of B since A is smaller
      else if(rootOfA->size > rootOfB->size) {
	//update the values
        rootOfB->parent = rootOfA;
        rootOfA->size = rootOfA->size + rootOfB->size;
      }
      else{
        rootOfA->parent = rootOfB;
        rootOfB->size = rootOfB->size + rootOfA->size;
      }
    }
};

#endif // UNIONFIND_HPP
