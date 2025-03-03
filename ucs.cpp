#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;
int main(){
    ifstream fin("input.txt");
    int n;
    fin >> n;
    vector<vector<int>> adj(n, vector<int>(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            fin >> adj[i][j];
        }
    }
    int start, goal;
    fin >> start >> goal;
    start--; goal--;
    vector<int> dist(n, INT_MAX), parent(n, -1);
    dist[start] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, start});
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        if(d > dist[u]) continue;
        if(u == goal) break;
        for(int v = 0; v < n; v++){
            if(adj[u][v] > 0){
                int nd = d + adj[u][v];
                if(nd < dist[v]){
                    dist[v] = nd;
                    parent[v] = u;
                    pq.push({nd, v});
                }
            }
        }
    }
    if(dist[goal] == INT_MAX){
        cout << "No path" << "\n";
        return 0;
    }
    vector<int> path;
    for(int cur = goal; cur != -1; cur = parent[cur])
        path.push_back(cur);
    reverse(path.begin(), path.end());
    cout << "Cost: " << dist[goal] << "\nPath: ";
    for(auto x : path)
        cout << x + 1 << " ";
    return 0;
}
