#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class ActorNode {
  protected:
  public:
    string actor_name;
    ActorNode* prev;
    unsigned int dist;
    string movie_path;
    vector<pair<string, int>> movie_list;
    int year;

    ActorNode(string actor) {
        actor_name = actor;
        prev = nullptr;
        dist = -1;
        movie_path = "";
        year = -1;
    }
};

#endif