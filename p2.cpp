#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Edge {
    int to, line;
};

const int INF = INT_MAX;

int bfs_line_changes(int start, const vector<vector<Edge>>& graph, int n, int l) {
    vector<vector<int>> dist(n + 1, vector<int>(l + 1, INF)); // dist[node][line]
    queue<pair<int, int>> q; // {node, current_line}

    // Initialize the BFS queue for the starting node
    for (size_t i = 0; i < graph[start].size(); ++i) {
        int next_line = graph[start][i].line;
        dist[start][next_line] = 0;
        q.push(make_pair(start, next_line));
    }

    while (!q.empty()) {
        int u = q.front().first;
        int current_line = q.front().second;
        q.pop();

        for (size_t i = 0; i < graph[u].size(); ++i) {
            int v = graph[u][i].to;
            int next_line = graph[u][i].line;

            // Calculate the new cost for this neighbor
            int new_changes = dist[u][current_line] + (current_line == next_line ? 0 : 1);

            if (new_changes < dist[v][next_line]) {
                dist[v][next_line] = new_changes;
                q.push(make_pair(v, next_line));
            }
        }
    }

    // Get the minimum line changes for this BFS
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
        graph[u].push_back((Edge){v, line});
        graph[v].push_back((Edge){u, line}); // Undirected graph
    }

    int result = 0;

    // Run BFS for each node to determine the global maximum line changes
    for (int i = 1; i <= n; ++i) {
        int max_changes = bfs_line_changes(i, graph, n, l);
        if (max_changes == -1) {
            cout << -1 << endl;
            return 0;
        }
        result = max(result, max_changes);
    }

    cout << result << endl;
    return 0;
}

