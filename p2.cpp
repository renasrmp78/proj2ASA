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

void print_lines_graph(const vector<vector<int>>& lines_graph, int total_lines) {
    cout << "Lines Graph:" << endl;
    for (int i = 1; i <= total_lines; ++i) { // Start from line 1 (1-based indexing)
        cout << "Line " << i << " connects to: ";
        for (int neighbor : lines_graph[i]) {
            cout << neighbor << " "; // Print each neighboring line
        }
        cout << endl; // Newline after printing all neighbors for this line
    }
}


int bfs_lines_graph(int start_line, const vector<vector<int>>& lines_graph, int total_lines) {
    
    vector<int> visited(total_lines + 1, -1); // Distance from start_line (-1 means not visited)
    queue<int> q;

    // Initialize BFS
    q.push(start_line);
    visited[start_line] = 0;

    int max_changes = 0;

    while (!q.empty()) {
        int current_line = q.front();
        q.pop();

        // Traverse all neighboring lines
        for (int neighbor_line : lines_graph[current_line]) {
            if (visited[neighbor_line] == -1) { // If not visited
                visited[neighbor_line] = visited[current_line] + 1; // Increment line change
                max_changes = max(max_changes, visited[neighbor_line]); // Update maximum distance
                q.push(neighbor_line); // Push the neighbor to the queue
            }
        }
    }

    // Check if all lines are reachable
    for (int i = 1; i <= total_lines; i++) {
        if (visited[i] == -1) return -1; // If a line is not reachable, return -1
    }

    return max_changes; // Return the maximum line changes
}

int main() {
    int n, m, l;
    cin >> n >> m >> l;

    vector<vector<Edge>> graph(n + 1);
    vector<vector<int>> lines_graph(l + 1);

    // Input the graph
    for (int i = 0; i < m; ++i) {
        int u, v, line;
        cin >> u >> v >> line;
        graph[u].push_back(Edge{v, line});
        graph[v].push_back(Edge{u, line}); // Undirected graph
    }

    int result = 0;

    // Creation of lines graph
    // Idea: Start in a certain point and check its adjacencies
    for (int i = 1 ; i <= n; i++){ // In a point
        for (size_t j = 0; j < graph[i].size(); j++){ // Check all adjacencies and link lines 
            for (size_t k = j + 1; k < graph[i].size(); k++) {
                int line1 = graph[i][j].line;
                int line2 = graph[i][k].line;
                if (line1 != line2){
                    lines_graph[line1].push_back(line2);
                    lines_graph[line2].push_back(line1);
                }
            }
        }
    }
    
    print_lines_graph(lines_graph, n);

    // Run bfs for each node to determine the global maximum line changes
    for (int i = 1; i <= n; i++) {
        int max_changes = bfs_lines_graph(i, lines_graph, l);
        if (max_changes == -1) {
            cout << -1 << endl;
            return 0;
        }
        result = max(result, max_changes);
    }

    cout << result << endl;
    return 0;
}
