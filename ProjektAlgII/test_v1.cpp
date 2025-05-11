#include <gtest/gtest.h> //using Google Test framework
#include <memory>
#include "sourceFiles/Country.h"

class CountryTest : public ::testing::Test {
protected:
    void SetUp() override {
        country = std::make_unique<Country>();
    }

    std::unique_ptr<Country> country;
};

TEST_F(CountryTest, TestAddRelationship) {
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    Lane lane(node1, node2, 10);
    
    country->addRelationship(lane);
    
    auto adjList = country->getAdjList();
    ASSERT_EQ(adjList[node1].size(), 1);
    ASSERT_EQ(adjList[node1][0].getToPtr(), node2);
}

TEST_F(CountryTest, TestBFS) {
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    auto node3 = std::make_shared<Node>(3);
    country->addRelationship(Lane(node1, node2, 5));
    country->addRelationship(Lane(node2, node3, 7));
    
    testing::internal::CaptureStdout();
    country->bfs(node1);
    std::string output = testing::internal::GetCapturedStdout();
    //std::cout << "Captured output: " << output << std::endl;  //debugging line



    EXPECT_NE(output.find("Field_1"), std::string::npos);
    EXPECT_NE(output.find("Brewery_0"), std::string::npos);
    EXPECT_NE(output.find("Pub_0"), std::string::npos);
}

TEST_F(CountryTest, TestEdmondsKarpAlgorithm) {
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    auto node3 = std::make_shared<Node>(3);
    
    country->addRelationship(Lane(node1, node2, 10));
    country->addRelationship(Lane(node2, node3, 5));
    
    int maxFlow = country->edmondsKarp(node1, node3);
    
    ASSERT_EQ(maxFlow, 5);
}

TEST_F(CountryTest, TestPerformanceEdmondsKarp) {
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    auto node3 = std::make_shared<Node>(3);
    
    country->addRelationship(Lane(node1, node2, 1000));
    country->addRelationship(Lane(node2, node3, 1000));
    
    auto start = std::chrono::high_resolution_clock::now();
    country->edmondsKarp(node1, node3);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    ASSERT_LE(duration, 100);  //timeout of 100ms
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
