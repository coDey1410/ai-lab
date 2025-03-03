#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;
void printPath(const vector<int>& path) {
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1)
            cout << " -> ";
    }
    cout << "\n";
}
bool dfsUtil(int node, int goal, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& path, vector<int>& order) {
    order.push_back(node);
    if (node == goal)
        return true;
    for (int i = 1; i < adj.size(); i++) {
        if (adj[node][i] && !visited[i]) {
            visited[i] = true;
            path.push_back(i);
            if (dfsUtil(i, goal, adj, visited, path, order))
                return true;
            path.pop_back();
        }
    }
    return false;
}
int main(){
    ifstream inFile("input.txt");
    int n, start, max_depth, goal;
    inFile >> n >> start >> max_depth >> goal;
    vector<vector<int>> adj(n+1, vector<int>(n+1, 0));
    int edges;
    inFile >> edges;
    for (int i = 0; i < edges; i++){
        int u, v;
        inFile >> u >> v;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }
    vector<int> bfsOrder;
    vector<int> parent(n+1, -1), depth(n+1, 0);
    vector<bool> visitedBFS(n+1, false);
    queue<int> q;
    q.push(start);
    visitedBFS[start] = true;
    depth[start] = 0;
    bool bfsGoalFound = false;
    while(!q.empty()){
        int cur = q.front();
        q.pop();
        bfsOrder.push_back(cur);
        if(cur == goal){
            bfsGoalFound = true;
            break;
        }
        if(depth[cur] == max_depth)
            continue;
        for (int i = 1; i <= n; i++){
            if(adj[cur][i] && !visitedBFS[i]){
                visitedBFS[i] = true;
                parent[i] = cur;
                depth[i] = depth[cur] + 1;
                q.push(i);
            }
        }
    }
    cout << "BFS Order:\n";
    for (int x : bfsOrder)
        cout << x << " ";
    cout << "\n";
    if(bfsGoalFound){
        vector<int> path;
        int cur = goal;
        while(cur != -1){
            path.push_back(cur);
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        cout << "BFS Path:\n";
        printPath(path);
        return 0;
    }
    vector<int> frontier;
    for (int i = 1; i <= n; i++){
        if(visitedBFS[i] && depth[i] == max_depth)
            frontier.push_back(i);
    }
    bool found = false;
    vector<int> finalPath, finalDFSOrder;
    for (int node : frontier){
        vector<int> bfsPath;
        int cur = node;
        while(cur != -1){
            bfsPath.push_back(cur);
            cur = parent[cur];
        }
        reverse(bfsPath.begin(), bfsPath.end());
        vector<bool> visitedDFS(n+1, false);
        for (int x : bfsPath)
            visitedDFS[x] = true;
        vector<int> path = bfsPath, dfsOrder;
        if(dfsUtil(node, goal, adj, visitedDFS, path, dfsOrder)){
            finalPath = path;
            finalDFSOrder = dfsOrder;
            found = true;
            break;
        }
    }
    if(found){
        cout << "DFS Order:\n";
        for (int x : finalDFSOrder)
            cout << x << " ";
        cout << "\nPath found using BFS+DFS:\n";
        printPath(finalPath);
    }
    else{
        cout << "Goal node " << goal << " not found!\n";
    }
    return 0;
}
