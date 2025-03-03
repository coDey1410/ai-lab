#include<bits/stdc++.h>

#include<iostream>
#include<stack>
#include<queue>
#include<vector>

using namespace std;


bool dfs(vector<int> adj[],int node,int parent,int goal,int b,int d,vector<int> & visited,vector<int>& order,unordered_map<int,int>& mpp,int currB,int currD){


    if(currD>d || currB >b) return false;
    if(visited[node]) return false;

    // cout<<node<< " ....";

    visited[node]=1;

    order.push_back(node);
    mpp[node]=parent;

    if(goal== node) return true;
    int bb=1;


    for(int i=0;i<adj[node].size();i++){
        if(visited[adj[node][i]]) continue;
        

        if(dfs(adj,adj[node][i],node,goal,b,d,visited,order, mpp,bb,currD+1)==true) return true;
        bb++;
    }
}



void dfsHelper(vector<int> adj[],int start,int goal,int b,int d,int n){


    vector<int> order,path;
    unordered_map <int,int> mpp;
    mpp[start]=-1;

    // int n=adj.size();

    vector<int> visited(n+1,0);

    // cout<<"visited: ";
    // for(int i=0;i<visited.size();i++){
    //     cout<<visited[i]<< " ";
    // }



    dfs(adj,start,-1,goal,b,d,visited,order,mpp,0,0);


    if(mpp.find(goal)==mpp.end()){
        cout<<"Goal not found with given depth and branch  "<<endl;
        return;
    }

    cout<<"Order: ";

    for(int i=0;i<order.size();i++){
        cout<<order[i]<< " <  ";
    }
    cout<<endl;

    int curr=goal;

    while(curr!=-1){
        path.push_back(curr);
        curr=mpp[curr];
    }

    reverse(path.begin(),path.end());

    cout<<"Path: ";
    for(int i=0;i<path.size();i++){
        cout<<path[i]<<" < ";
    }
    cout<<endl;



}


int main(){

    freopen("input1.txt","r",stdin);

    int n,m;
    cin>>n>>m;
    // n=vertices,m=edges

    vector<int> adj[n+1];

    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
    }

    int b,d,goal,start;
     

    cin>>start>>goal;

    cin>>b>>d;


    cout<<"Start:"<<start<<"Goal: "<<goal<<"b: "<<b<<" d: "<<d<<endl;
    
    vector<int> order,path;
    dfsHelper(adj,start,goal,b,d,n);





     return 0;       
}
