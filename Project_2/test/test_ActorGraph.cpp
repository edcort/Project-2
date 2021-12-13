#include <gtest/gtest.h>

#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ActorGraphTests, TwoActorsOneMovie) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

// TODO: add more tests for actor graph
TEST(ActorGraphTests, test_resetActorNode) {
    ActorNode* testNode = new ActorNode("Eduardo Cortez");
    ActorNode* testNode2 = new ActorNode("UCSD");

    ASSERT_EQ(testNode->actor_name, "Eduardo Cortez");

    testNode->dist = 100;
    testNode->year = 2020;
    testNode->movie_path = "Jurassic";
    testNode->prev = testNode2;
    ASSERT_EQ(testNode->dist, 100);
    ASSERT_EQ(testNode->year, 2020);
    ASSERT_EQ(testNode->movie_path, "Jurassic");
    ASSERT_EQ(testNode->prev, testNode2);

    ActorGraph graph;
    graph.resetActorNode(testNode);

    ASSERT_EQ(testNode->dist, -1);
    ASSERT_EQ(testNode->year, -1);
    ASSERT_EQ(testNode->movie_path, "");
    ASSERT_EQ(testNode->prev, nullptr);

    delete testNode;
    delete testNode2;
}

TEST(ActorGraphTests, test_BFS) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);

    string answer;
    string shouldBe = "(Actor 1)--[Awesome Movie#@2020]-->(Actor 2)";
    graph.BFS(from, to, answer);

    ASSERT_EQ(answer, shouldBe);
}

TEST(ActorGraphTests, test_BFS_harder) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";
    string c = "Actor 3";
    string mov = "CSE 100\t1956";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ss << to << "\t" << mov << endl;
    ss << c << "\t" << mov << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);

    string answer;
    string shouldBe =
        "(Actor 1)--[Awesome Movie#@2020]-->(Actor 2)--[CSE "
        "100#@1956]-->(Actor 3)";
    graph.BFS(from, c, answer);

    ASSERT_EQ(answer, shouldBe);
}