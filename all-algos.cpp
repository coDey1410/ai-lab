#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <limits>
#include <fstream> // Added for file I/O
using namespace std;

vector<vector<int>> graph; // For DFS, BFS, etc.
vector<vector<pair<int,int>>> weightedGraph; // For UCS (stores {neighbor, weight})
vector<bool> vis;
int vertices, edges;
bool isWeighted = false; // global flag indicating if the graph is weighted

vector<int> order;
ifstream fin; // File input stream

// ------------------ DFS ------------------
void dfs(int node, map<int, vector<int>> &paths, vector<int> &path)
{
    if (!vis[node])
    {
        vis[node] = true;
        order.push_back(node);
        path.push_back(node);
        if (paths.find(node) != paths.end())
        {
            for (auto ele : path)
                paths[node].push_back(ele);
        }
        for (auto child : graph[node])
        {
            dfs(child, paths, path);
        }
        path.pop_back();
    }
}

// ------------------ BFS ------------------
void bfs(int start, map<int, vector<int>> &paths, vector<int> path)
{
    queue<int> q;
    unordered_map<int, int> par;
    par[start] = -1;
    q.push(start);
    vis[start] = true;

    while (!q.empty())
    {
        int node = q.front();
        order.push_back(node);
        q.pop();
        for (auto child : graph[node])
        {
            if (!vis[child])
            {
                vis[child] = true;
                par[child] = node;
                q.push(child);
            }
        }
    }
    for (auto itr : paths)
    {
        int parent = itr.first;
        while (parent != -1)
        {
            paths[itr.first].push_back(parent);
            parent = par[parent];
        }
        reverse(paths[itr.first].begin(), paths[itr.first].end());
    }
}

// ------------------ Iterative Broadening Search (IBS) ------------------
void ibs(int start, map<int, vector<int>> &paths, vector<int> &path)
{
    int limit;
    cout << "Enter the ibs limit --> ";
    cin >> limit;
    cout << limit << endl;

    queue<int> q;
    unordered_map<int, int> par;
    par[start] = -1;
    q.push(start);
    vis[start] = true;

    while (!q.empty())
    {
        int node = q.front();
        order.push_back(node);
        q.pop();
        // Process only up to 'limit' children for the current node
        for (int it = 0; it <= min(limit - 1, (int)graph[node].size() - 1); ++it)
        {
            int child = graph[node][it];
            if (!vis[child])
            {
                vis[child] = true;
                par[child] = node;
                q.push(child);
            }
        }
    }
    for (auto itr : paths)
    {
        if (vis[itr.first])
        {
            int parent = itr.first;
            while (parent != -1)
            {
                paths[itr.first].push_back(parent);
                parent = par[parent];
            }
            reverse(paths[itr.first].begin(), paths[itr.first].end());
        }
    }
}

// ------------------ Depth Limited Search (DLS) ------------------
void dls(int node, map<int, vector<int>> &paths, vector<int> &path, int curr_depth, int depth_limit)
{
    if (!vis[node] && curr_depth < depth_limit)
    {
        vis[node] = true;
        order.push_back(node);
        path.push_back(node);
        if (paths.find(node) != paths.end())
        {
            for (auto ele : path)
                paths[node].push_back(ele);
        }
        for (auto child : graph[node])
        {
            dls(child, paths, path, curr_depth + 1, depth_limit);
        }
        path.pop_back();
    }
}

// ------------------ Uniform Cost Search (UCS) ------------------
void ucs(int start, map<int, vector<int>> &paths, vector<int> &/*path*/)
{
    const int INF = numeric_limits<int>::max();
    vector<int> dist(vertices + 1, INF);
    vector<int> parent(vertices + 1, -1);

    // Priority queue: pair<cost, node> with smallest cost on top
    typedef pair<int, int> pii;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty())
    {
        auto curr = pq.top();
        pq.pop();
        int cost = curr.first;
        int node = curr.second;
        
        // If we have already found a better path, skip
        if(cost > dist[node])
            continue;
            
        vis[node] = true;
        order.push_back(node);

        // Traverse neighbors from weightedGraph
        for (auto &edge : weightedGraph[node])
        {
            int child = edge.first;
            int w = edge.second;
            if(dist[node] != INF && dist[node] + w < dist[child])
            {
                dist[child] = dist[node] + w;
                parent[child] = node;
                pq.push({dist[child], child});
            }
        }
    }

    // Reconstruct paths for each goal node in the 'paths' map
    for (auto &p : paths)
    {
        int goal = p.first;
        if (vis[goal])
        {
            int cur = goal;
            vector<int> revPath;
            while (cur != -1)
            {
                revPath.push_back(cur);
                cur = parent[cur];
            }
            reverse(revPath.begin(), revPath.end());
            paths[goal] = revPath;
            cout << "Cost to reach " << goal << " --> " << dist[goal] << "\n";
        }
    }
}

// ------------------ Graph Initialization ------------------
void initGraph()
{
    cout << "Enter the number of vertices --> ";
    fin >> vertices;
    cout << vertices << endl;
    
    cout << "Enter the number of edges --> ";
    fin >> edges;
    cout << edges << endl;
    
    cout << "Is the graph weighted? (1 for yes, 0 for no): ";
    int weightedChoice;
    fin >> weightedChoice;
    cout << weightedChoice << endl;
    
    isWeighted = (weightedChoice == 1);

    graph.assign(vertices + 1, vector<int>());
    weightedGraph.assign(vertices + 1, vector<pair<int,int>>());

    int remainingEdges = edges;
    while (remainingEdges--)
    {
        int u, v;
        fin >> u >> v;
        int w = 1;
        if(isWeighted)
        {
            cout << "Enter weight for edge (" << u << ", " << v << "): ";
            fin >> w;
            cout << w << endl;
        }
        // For unweighted searches, store only the neighbor information
        graph[u].push_back(v);
        graph[v].push_back(u);
        // For UCS, store both neighbor and weight
        weightedGraph[u].push_back({v, w});
        weightedGraph[v].push_back({u, w});
    }
}

// ------------------ Utility Function ------------------
void initialiseUtilityTools(int &start, map<int, vector<int>> &paths, vector<int> &path)
{
    int goal, no_of_goal;
    cout << "Enter the start --> ";
    cin >> start;
    cout << start << endl;
    
    cout << "Enter the number of goal nodes --> ";
    cin >> no_of_goal;
    cout << no_of_goal << endl;
    
    cout << "Enter the Goal Nodes --> ";
    while (no_of_goal--)
    {
        cin >> goal;
        cout << goal << " ";
        paths[goal] = path;
    }
    cout << endl;
}

// ------------------ Print Goal Paths ------------------
void printPaths(map<int, vector<int>> paths)
{
    for (auto itr : paths)
    {
        if (vis[itr.first])
        {
            cout << "Goal Encountered --> " << itr.first << "\n";
            for (auto ele : paths[itr.first])
            {
                cout << ele << " ";
            }
            cout << "\n";
        }
        else
        {
            cout << itr.first << " -> Goal could not be reached\n";
        }
    }
}

// ------------------ Iterative Deepening Search (IDS) ------------------
void ids()
{
    map<int, vector<int>> paths;
    vector<int> path;
    int start, max_depth;

    initialiseUtilityTools(start, paths, path);
    cout << "Enter max depth --> ";
    cin >> max_depth;
    cout << max_depth << endl;

    auto checkIfAllGoalsReached = [&](vector<bool> &temp) -> bool
    {
        for (auto itr : paths)
        {
            if (!temp[itr.first])
                return false;
        }
        return true;
    };

    // Perform IDS by increasing depth limits
    vector<bool> temp(vertices + 1, false);
    for (int depth = 0; depth <= max_depth; depth++)
    {
        if (!checkIfAllGoalsReached(temp))
        {
            vis.assign(vertices + 1, false);
            dls(start, paths, path, 0, depth);
            temp = vis;
        }
    }
    printPaths(paths);
}

// ------------------ Generic Utility for DFS/BFS/IBS/UCS ------------------
void utilityDFS_BFS_IBS(function<void(int, map<int, vector<int>> &, vector<int>&)> searchFunction)
{
    map<int, vector<int>> paths;
    vector<int> path;
    int start;

    initialiseUtilityTools(start, paths, path);

    vis.assign(vertices + 1, false);
    searchFunction(start, paths, path);
    printPaths(paths);
}

// ------------------ Print Order of Node Visits ------------------
void printOrder()
{
    cout << "Order --> ";
    for (auto node : order)
        cout << node << " ";
    cout << endl;
}

// ------------------ Main Function ------------------
int main()
{
    // Open input file
    fin.open("input.txt");
    if (!fin.is_open()) {
        cout << "Error: Unable to open input.txt" << endl;
        return 1;
    }
   
    while (true)
    {
        int ch;
        cout << "\nMenu:\n"
             << "1. Enter Graph\n"
             << "2. DFS\n"
             << "3. BFS\n"
             << "4. Depth Limited Search (DLS)\n"
             << "5. Iterative Deepening Search (IDS)\n"
             << "6. Iterative Broadening Search (IBS)\n"
             << "7. Uniform Cost Search (UCS)\n"
             << "Enter Choice --> ";
        cin>> ch;
        cout << ch << endl;
        switch (ch)
        {
            case 1:
                initGraph();
                break;
            case 2:
            {
                order.clear();
                utilityDFS_BFS_IBS(&dfs);
                printOrder();
                break;
            }
            case 3:
            {
                order.clear();
                utilityDFS_BFS_IBS(&bfs);
                printOrder();
                break;
            }
            case 4:
            {
                int start;
                map<int, vector<int>> paths;
                vector<int> path;

                initialiseUtilityTools(start, paths, path);

                int depth_limit;
                cout << "Enter the depth limit --> ";
                cin >> depth_limit;
                cout << depth_limit << endl;
                
                vis.assign(vertices + 1, false);
                order.clear();
                dls(start, paths, path, 0, depth_limit);
                printPaths(paths);
                printOrder();
                break;
            }
            case 5:
            {
                ids();
                break;
            }
            case 6:
            {
                order.clear();
                utilityDFS_BFS_IBS(&ibs);
                printOrder();
                break;
            }
            case 7:
            {
                order.clear();
                vis.assign(vertices + 1, false);
                utilityDFS_BFS_IBS(&ucs);
                printOrder();
                break;
            }
            default:
                fin.close();
                exit(0);
                break;
        }
        
        // Check if we've reached the end of the file
        if (fin.eof() || !fin.good()) {
            cout << "End of input file reached." << endl;
            break;
        }
    }
    
    fin.close();
    return 0;
}