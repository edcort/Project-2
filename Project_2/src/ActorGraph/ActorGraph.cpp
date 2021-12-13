#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>

using namespace std;

ActorGraph::ActorGraph() {}

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraph(istream& is) {
    unsigned int actor_count = 0;  // delete me, just for testing

    bool readHeader = false;

    while (is) {
        string s;
        if (!getline(is, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);

        // TODO: we have an actor/movie relationship to build the graph

        // iterator to check if curr actor exisit in actor graph
        unordered_map<string, ActorNode*>::const_iterator find_actor =
            actor_graph.find(actor);

        pair<string, int> movie_year = make_pair(title, year);

        // if actor does not exist in graph create a new node
        if (find_actor == actor_graph.end()) {
            ActorNode* addNode = new ActorNode(actor);

            addNode->movie_list.push_back(movie_year);
            actor_graph[actor] = addNode;

            actor_count++;
        }
        // else if actor node already exists then just update the
        // movie list in their node
        else {
            find_actor->second->movie_list.push_back(movie_year);
        }

        // update film_actor_list
        // check is movie is in list if not add along with actor as first cast
        // member if movie does exist then just add actor to movie cast

        // iterator to check if curr actor exisit in actor graph
        string title_year_string = title + " " + record[2];
        unordered_map<string, vector<string>>::const_iterator find_title =
            film_actor_list.find(title_year_string);

        if (find_title == film_actor_list.end()) {
            film_actor_list[title_year_string] = vector<string>();
            film_actor_list[title_year_string].push_back(actor);
        } else {
            film_actor_list[title_year_string].push_back(actor);
        }
    }

    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        // TODO: delete the graph
        return false;
    }

    return true;
}

/* TODO */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // stack to be utilized to push actor nodes in BFS alg.
    queue<ActorNode*> q;

    queue<ActorNode*> changed_nodes;
    // iterator to check if fromActor exists in actor graph
    unordered_map<string, ActorNode*>::const_iterator find_fromActor =
        actor_graph.find(fromActor);

    // iterator to check if toActor exists in actor graph
    unordered_map<string, ActorNode*>::const_iterator find_toActor =
        actor_graph.find(toActor);

    // if fromActor OR toActor does not exist then return
    if (find_fromActor == actor_graph.end() ||
        find_toActor == actor_graph.end()) {
        return;
    }

    // if fromActor is EQUAL to toActor then return
    if (fromActor == toActor) {
        return;
    }

    // else if fromActor and toActor does exist
    // then proceed to find shortest path between them
    else {
        bool found_path =
            false;  // flag for when we have found the desired end actor
        ActorNode* curr_actor;  // curr actor node

        q.push(find_fromActor->second);  // push fromActor to queue

        find_fromActor->second->dist = 0;  // set fromActor dist to 0

        changed_nodes.push(find_fromActor->second);

        // continue while the queue is NOT empty
        while (q.size() > 0) {
            curr_actor = q.front();
            q.pop();

            // FOUND desired toActor !!!
            if (curr_actor->actor_name == toActor) {
                found_path = true;

                while (q.size() > 0) {
                    q.pop();
                }

                ActorNode* temp = curr_actor;
                stack<ActorNode*> construct_string;

                while (temp->dist > 0) {
                    construct_string.push(temp);

                    temp = temp->prev;
                }
                construct_string.push(temp);

                string final_string =
                    "(" + construct_string.top()->actor_name + ")--";
                construct_string.pop();

                while (construct_string.size() > 1) {
                    final_string = final_string + "[" +
                                   construct_string.top()->movie_path + "#@" +
                                   to_string(construct_string.top()->year) +
                                   "]-->(" +
                                   construct_string.top()->actor_name + ")--";

                    construct_string.pop();
                }
                final_string =
                    final_string + "[" + construct_string.top()->movie_path +
                    "#@" + to_string(construct_string.top()->year) + "]-->(" +
                    construct_string.top()->actor_name + ")";

                construct_string.pop();

                while (changed_nodes.size() > 0) {
                    ActorNode* reset = changed_nodes.front();
                    resetActorNode(reset);
                    changed_nodes.pop();
                }
                shortestPath = final_string;
                return;
            }
            if (found_path == false) {
                // count how many movies curr_actor has been in
                unsigned int curr_movie_list_size =
                    curr_actor->movie_list.size();

                // loop through each of the movies curr_actor has been in
                for (unsigned int x = 0; x < curr_movie_list_size; x++) {
                    // build string of #x title + year
                    string curr_movie_costars =
                        curr_actor->movie_list[x].first + " " +
                        to_string(curr_actor->movie_list[x].second);

                    // find x's movie in film_actor_list
                    unordered_map<string, vector<string>>::const_iterator
                        curr_film_list =
                            film_actor_list.find(curr_movie_costars);

                    // count how many co-stars are in x's movie
                    unsigned int curr_movie_costars_size =
                        curr_film_list->second.size();

                    // loop through each of the co-stars in x's movie
                    for (unsigned int y = 0; y < curr_movie_costars_size; y++) {
                        // get us access to the current co-stars actor node
                        unordered_map<string, ActorNode*>::const_iterator
                            curr_child =
                                actor_graph.find(curr_film_list->second[y]);

                        // enter if the co-star actor node has NOT been visted
                        // yet NOT visited is denoted with dist = -1
                        if (curr_child->second->dist == -1 &&
                            curr_child != actor_graph.end()) {
                            // update ALL appropriate infor in co-star actor
                            // node;

                            curr_child->second->dist = curr_actor->dist + 1;

                            curr_child->second->prev = curr_actor;

                            curr_child->second->movie_path =
                                curr_actor->movie_list[x].first;

                            curr_child->second->year =
                                curr_actor->movie_list[x].second;

                            q.push(curr_child->second);

                            changed_nodes.push(curr_child->second);
                        }
                    }
                }
            }
        }
    }
    return;
}

// HELPER METHOD
void ActorGraph::resetActorNode(ActorNode* node) {
    node->dist = -1;
    node->movie_path = "";
    node->year = -1;
    node->prev = nullptr;
}

/* TODO */
ActorGraph::~ActorGraph() {
    unordered_map<string, ActorNode*>::const_iterator it = actor_graph.begin();
    // Iterate over the map using iterator
    while (it != actor_graph.end()) {
        delete it->second;
        it++;
    }
}