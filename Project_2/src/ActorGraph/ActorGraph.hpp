#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "ActorNode.hpp"

using namespace std;

class ActorGraph {
  protected:
    // TODO: add data structures used in actor graph
    unordered_map<string, ActorNode*> actor_graph;

    unordered_map<string, vector<string>> film_actor_list;

  public:
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraph(istream& is);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* TODO */
    ~ActorGraph();

    // HELPER METHOD
    void resetActorNode(ActorNode* node);
};

#endif  // ACTORGRAPH_HPP