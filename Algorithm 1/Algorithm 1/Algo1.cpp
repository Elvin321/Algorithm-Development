#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <climits>
#include <algorithm>

// Define the structure to represent an edge
struct Edge {
    char from;
    char to;
    int distance; // Represents the distance (or weight) between vertices
};

// Define the comparator for the priority queue
struct CompareEdge {
    bool operator()(std::pair<char, int> const& p1, std::pair<char, int> const& p2) {
        return p1.second > p2.second; // Min-heap based on distance
    }
};

// Function to perform Dijkstra's algorithm
std::unordered_map<char, int> dijkstra(const std::unordered_map<char, std::vector<Edge>>& graph, char start, std::unordered_map<char, char>& previous) {
    std::priority_queue<std::pair<char, int>, std::vector<std::pair<char, int>>, CompareEdge> pq;
    std::unordered_map<char, int> distances;

    for (const auto& pair : graph) {
        distances[pair.first] = INT_MAX;
    }
    distances[start] = 0;
    pq.push({ start, 0 });

    while (!pq.empty()) {
        char current = pq.top().first;
        int current_distance = pq.top().second;
        pq.pop();

        for (const Edge& edge : graph.at(current)) {
            int new_distance = current_distance + edge.distance;
            if (new_distance < distances[edge.to]) {
                distances[edge.to] = new_distance;
                pq.push({ edge.to, new_distance });
                previous[edge.to] = current;
            }
        }
    }
    return distances;
}

// Function to read the graph from a file
std::unordered_map<char, std::vector<Edge>> readGraphFromFile() {
    std::unordered_map<char, std::vector<Edge>> graph;
    std::ifstream file("Paths.txt");
    if (!file) {
        std::cerr << "Unable to open file: edges.txt" << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char from, to;
        int distance;
        char arrow1, arrow2, colon;

        if (iss >> from >> arrow1 >> arrow2 >> to >> colon >> distance) {
            graph[from].push_back({ from, to, distance });
        }
        else {
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }

    file.close();
    return graph;
}

// Function to reconstruct the shortest path
std::vector<char> reconstructPath(const std::unordered_map<char, char>& previous, char start, char target) {
    std::vector<char> path;
    for (char at = target; at != start; at = previous.at(at)) {
        path.push_back(at);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    // Read the graph from the file
    std::unordered_map<char, std::vector<Edge>> graph = readGraphFromFile();

    // Get the source and target vertices from the user
    char start, target;
    std::cout << "The vertices you can enter range from A - H (please enter in capital letters)" << std::endl;
    std::cout << "Enter the starting vertex: ";
    std::cin >> start;
    std::cout << "Enter the target vertex: ";
    std::cin >> target;

    // Perform Dijkstra's algorithm
    std::unordered_map<char, char> previous;
    std::unordered_map<char, int> distances = dijkstra(graph, start, previous);

    // Print the shortest path and distance
    if (distances[target] == INT_MAX) {
        std::cout << "No path exists from " << start << " to " << target << std::endl;
    }
    else {
        std::cout << "Shortest distance from " << start << " to " << target << " is " << distances[target] << std::endl;

        // Reconstruct the path
        std::vector<char> path = reconstructPath(previous, start, target);
        std::cout << "Path: ";
        for (char vertex : path) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}