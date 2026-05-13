#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

class Graph{
    public:
    int vertices;
    vector<vector<int>>adj;
    Graph(int vertices){
        this->vertices=vertices;
        adj.resize(vertices);
    }

    void addedge(int src,int dest){
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }

    void printgraph(){
        cout<<"Graph:"<<endl;
        for(int i=0;i<vertices;i++){
            cout<<"Vertex:"<<i<<"->";
            for(int j=0;j<adj[i].size();j++){
                cout<<adj[i][j]<<" ";
            }
            cout<<endl;

        }
    }

    void seq_bfs(int start){
        vector<bool>visited(vertices,false);
        queue<int>q;
        q.push(start);
        visited[start]=true;
        while(!q.empty()){
            int node=q.front();
            q.pop();
            cout<<node<<" ";
            for(int i=0;i<adj[node].size();i++){
                int neighbour=adj[node][i];
                if(!visited[neighbour]){
                    visited[neighbour]=true;
                    q.push(neighbour);
                }
            }
        }
    }

    void dfs(int start){
        stack<int>st;
        vector<bool>visited(vertices,false);
        st.push(start);
        visited[start]=true;
        while(!st.empty()){
            int node=st.top();
            st.pop();
            cout<<node<<" ";
            for(int i=0;i<adj[node].size();i++){
                int neighbour=adj[node][i];

                if(!visited[neighbour]){
                    visited[neighbour]=true;
                    st.push(neighbour);
                }
            }
        }
    }

    void parallel_bfs(int start){
        queue<int>q;
        vector<bool>visited(vertices,false);
        q.push(start);
        visited[start]=true;
        while(!q.empty()){
            int node=q.front();
            q.pop();
            cout<<node<<" ";
            #pragma omp parallel for
            for(int i=0;i<adj[node].size();i++){
                int neighbour=adj[node][i];
                bool ispush=false;
                #pragma omp critical
                {
                if(!visited[neighbour]){
                    visited[neighbour]=true;
                    ispush=true;
                }
                }

                #pragma omp critical
                {
                    if(ispush){
                        q.push(neighbour);
                    }
                }
            }

        }
    }

    void parallel_dfs(int start){
        stack<int>st;
        st.push(start);
        vector<bool>visited(vertices,false);
        visited[start]=true;
        while(!st.empty()){
            int node=st.top();
            st.pop();
            cout<<node<<" ";
            #pragma omp parallel for
            for(int i=0;i<adj[node].size();i++){
                int neighbour=adj[node][i];
                bool ispush=false;

                #pragma omp critical
                {
                    if(!visited[neighbour]){
                        visited[neighbour]=true;
                        ispush=true;
                    }
                }

                #pragma omp critical
                {
                    if(ispush){
                        st.push(neighbour);
                    }

                }
            }
        }
    }
};
int main(int argc, char const *argv[])
{
    
    int vertices,edges;
    cout<<"Enter the number of vertices"<<endl;
    cin>>vertices;
    Graph obj=Graph(vertices);
    cout<<"Enter the number of edges"<<endl;
    cin>>edges;
    cout<<"Enter the source and destination of edges"<<endl;
    for(int i=0;i<edges;i++){
        int src,dest;
        cin>>src>>dest;
        obj.addedge(src,dest);
    }
    obj.printgraph();
    cout<<"Sequential BFS: "<<endl;
    double starttime=omp_get_wtime();
    obj.seq_bfs(0);
    double endtime=omp_get_wtime();
    cout<<"\nTime taken by sequential BFS: "<<endtime-starttime<<" seconds"<<endl;

    double starttime1=omp_get_wtime();
    cout<<"Parallel BFS: "<<endl;
    obj.parallel_bfs(0);
    double endtime1=omp_get_wtime();
    cout<<"\nTime taken by parallel BFS: "<<endtime1-starttime1<<" seconds"<<endl;

    double starttime2=omp_get_wtime();
    cout<<"Sequential DFS: "<<endl;
    obj.dfs(0);
    double endtime2=omp_get_wtime();
    cout<<"\nTime taken by sequential DFS: "<<endtime2-starttime2<<" seconds"<<endl;

    double starttime3=omp_get_wtime();
    cout<<"Parallel DFS: "<<endl;
    obj.parallel_dfs(0);
    double endtime3=omp_get_wtime();
    cout<<"\nTime taken by parallel DFS: "<<endtime3-starttime3<<" seconds"<<endl;

    

    /* code */
    return 0;
}
