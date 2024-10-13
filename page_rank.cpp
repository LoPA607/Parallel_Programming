#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>
using namespace std;
void parallelPageRank(const std::vector<std::vector<int>>& graph, std::vector<double>& ranks, double damping = 0.85, int iterations = 100) {
    int n = graph.size();
    std::vector<double> newRanks(n, 1.0 / n);

    for (int iter = 0; iter < iterations; iter++) {
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (std::find(graph[j].begin(), graph[j].end(), i) != graph[j].end()) {
                    sum += ranks[j] / graph[j].size();
                }
            }
            newRanks[i] = (1 - damping) / n + damping * sum;
        }

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            ranks[i] = newRanks[i];
        }
    }
}

int main() {
    std::vector<std::vector<int>> graph = {
        {1, 2},     // Node 0
        {2},        // Node 1
        {0},        // Node 2
        {0, 1, 2}   // Node 3
    };

    std::vector<double> ranks(graph.size(), 1.0 / graph.size());

    parallelPageRank(graph, ranks);

    cout << "PageRank values: ";
    for (double rank : ranks) {
        cout << rank << " ";
    }
    cout << std::endl;

    return 0;
}
