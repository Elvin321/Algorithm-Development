#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#define MAX_NODES 8
#define INF 1000000 // Represents infinity for edges that don't exist

// Struct to hold path and its weight
struct PathWeight {
    std::vector<int> path;
    int weight;
};

// Function to perform DFS and find a path
PathWeight dfs(int adjMatrix[MAX_NODES][MAX_NODES], int start, int target, std::vector<bool>& visited, std::vector<int>& path, int currWeight) {
    visited[start] = true;
    path.push_back(start);

    if (start == target) { //checks to see if user entered the same vertex for the start and end meaning goal is already reached
        PathWeight result;
        result.path = path;
        result.weight = currWeight;
        return result;
    }

    PathWeight finalResult;
    finalResult.weight = 0; //weight is 0 as no distance is travelled

    for (int i = 0; i < MAX_NODES; ++i) {
        if (adjMatrix[start][i] != INF && !visited[i]) {
            PathWeight result = dfs(adjMatrix, i, target, visited, path, currWeight + adjMatrix[start][i]); //calls function checking if target is reached
            if (!result.path.empty()) {
                return result;
            }
        }
    }

    path.pop_back();//remove element
    return finalResult;
}

int main() {
    std::ifstream file("Paths.txt"); //open file with data
    std::string line;
    std::map<char, int> nodeIndex = { {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7} };
    char indexToNode[MAX_NODES] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    int adjMatrix[MAX_NODES][MAX_NODES];

    // Initialize matrix with INF and 0 for self loops
    for (int i = 0; i < MAX_NODES; ++i) {
        for (int j = 0; j < MAX_NODES; ++j) {
            if (i == j) {
                adjMatrix[i][j] = 0;
            }
            else {
                adjMatrix[i][j] = INF;
            }
        }
    }

    // Read edges from the file and populate the matrix
    while (std::getline(file, line)) {
        char from = line[0];
        char to = line[5];
        int weight;
        std::stringstream ss(line.substr(8));
        ss >> weight;

        int i = nodeIndex[from];
        int j = nodeIndex[to];
        adjMatrix[i][j] = weight;
    }

    // Take user input for start and target nodes
    char startNode, targetNode;
    std::cout << "Nodes range from A - H, please enter in captial letter format." << std::endl;
    std::cout << "Enter the starting node: ";
    std::cin >> startNode;
    std::cout << "Enter the target node: ";
    std::cin >> targetNode;

    int start = nodeIndex[startNode];
    int target = nodeIndex[targetNode];

    // Perform DFS to find a path
    std::vector<bool> visited(MAX_NODES, false);
    std::vector<int> path;

    PathWeight result = dfs(adjMatrix, start, target, visited, path, 0);

    if (!result.path.empty()) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << indexToNode[node] << " ";
        }
        std::cout << "\nPath weight: " << result.weight << std::endl;
    }
    else {
        std::cout << "No path found." << std::endl;
    }

    return 0;
}
