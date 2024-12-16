#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <tuple>

using namespace std;

struct Edge {
    int to, line;
};

typedef tuple<int, int, int> State;

struct Compare {
    bool operator()(const State& a, const State& b){
        return get<0>(a) > get<0> (b);
    }
};

const int INF = INT_MAX;

int dijkstra_line_changes(int start, const vector<vector<Edge>>& graph, int n, int l) {

    priority_queue<State, vector<State>, Compare> pq; // Priority using State struct
    vector<vector<int>> dist(n + 1, vector<int>(l + 1, INF)); // Matrix with line changes count

    // Iterate through starts adjacencies and initiate pq
    for (const auto& edge : graph[start]) {
        int next_line = edge.line;
        dist[start][next_line] = 0;
        pq.emplace(0, start, next_line);
    }

    // Iterate through our priority queue
    while (!pq.empty()) {
        State top = pq.top(); // Biggest number of line changes
        int changes = get<0>(top);
        int u = get<1>(top);
        int current_line = get<2>(top);
        pq.pop(); // Remove from pq

        if (changes > dist[u][current_line]) continue; // In case of line not changing

        // Iterate thru adjacencies
        for (const auto& edge : graph[u]) {
            int v = edge.to; 
            int next_line = edge.line;
            int new_changes = changes + (current_line == next_line ? 0 : 1); // If line changes add 1
             // If this path offers fewer line changes, update the distance and push the new state into the queue
            if (new_changes < dist[v][next_line]) {
                dist[v][next_line] = new_changes;
                pq.emplace(new_changes, v, next_line); // Push the new state into the priority queu
            }
        }
    }

    // Find the maximum line changes across all reachable nodes
    int max_changes = 0;
    for (int v = 1; v <= n; ++v) {
        int min_changes = INF;
        for (int line = 1; line <= l; ++line) {
            min_changes = min(min_changes, dist[v][line]);
        }
        if (min_changes == INF) return -1; // Unreachable node
        max_changes = max(max_changes, min_changes);
    }

    return max_changes;
}

int main() {
    int n, m, l;
    cin >> n >> m >> l;

    vector<vector<Edge>> graph(n + 1);

    // Input the graph
    for (int i = 0; i < m; ++i) {
        int u, v, line;
        cin >> u >> v >> line;
        graph[u].push_back(Edge{v, line});
        graph[v].push_back(Edge{u, line}); // Undirected graph
    }

    int result = 0;

    // Run Dijkstra for each node to determine the global maximum line changes
    for (int i = 1; i <= n; i++) {
        int max_changes = dijkstra_line_changes(i, graph, n, l);
        if (max_changes == -1) {
            cout << -1 << endl;
            return 0;
        }
        result = max(result, max_changes);
    }

    cout << result << endl;
    return 0;
}
