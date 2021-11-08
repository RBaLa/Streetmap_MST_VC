#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <climits>
#include <queue>

using namespace std;

bool bFSearch(vector<unsigned> neighbors[], unsigned src, unsigned dest, unsigned nV, unsigned dist[], unsigned prev[]){

    bool visited[nV];
    //*prev = new unsigned[nV];
    queue<unsigned> q;
    for (unsigned i=0;i<nV;i++){
        dist[i] = INT_MAX;
        visited[i] = false;
        prev[i] = -1;
    }
    dist[src-1] = 0;
    visited[src-1] = true;
    q.push(src);

    while(!q.empty()){
        unsigned x = q.front();
        q.pop();
        for (int i = 0; i < neighbors[x-1].size(); i++) {
            if (visited[neighbors[x-1][i]-1] == false) {
                visited[neighbors[x-1][i]-1] = true;
                dist[neighbors[x-1][i]-1] = dist[x-1] + 1;
                prev[neighbors[x-1][i]-1] = x;
                q.push(neighbors[x-1][i]);

                if (neighbors[x-1][i] == dest)
                   return true;
            }
        }
    }
    return false;
}

int main() {

    regex ex_1("V\\s(\\d+)");
    regex ex_2("\\<(\\d+),(\\d+)\\>");
    sregex_iterator End;
    unsigned nVertices;
    unsigned nEdges;
    unsigned source;
    unsigned destination;
    vector<unsigned> edgeValues;

    while (!cin.eof()) {

        string line;
        char command;
        getline(cin, line);
        command = line[0];
        unsigned count=0;
        if (command=='V'){

            if (edgeValues.size()>0){
                edgeValues.clear();
            }

            smatch match;
            regex_match(line.cbegin(),line.cend(),match,ex_1);
            int temp = stoi(match[1]);
            nVertices = temp;
        }
        if (command=='E'){

            int temp1, temp2;
            sregex_iterator iter(line.begin(),line.end(),ex_2);
            while (iter!=End){
                temp1 = stoi((*iter)[1]);
                temp2 = stoi((*iter)[2]);
                edgeValues.push_back(temp1);
                edgeValues.push_back(temp2);
                ++iter;
            }
            nEdges = edgeValues.size()/2;
        }
        if (command=='s'){

            istringstream input(line);
            while(!input.eof()){
                char com;
                input>>com;
                input>>source;
                input>>destination;
            }
            if ((source>nVertices)||(destination>nVertices)){
                cout<<"Error: specified node(s) not in graph"<<endl;
            }
            else if ((source<=0)||(destination<=0)){
                cout<<"Error: specified node value(s) out of range."<<endl;
            }
            else{
                unsigned edges1[nEdges], edges2[nEdges];
                for (unsigned i=0; i<nEdges; i++){
                    edges1[i] = edgeValues[2*i];
                    edges2[i] = edgeValues[2*i+1];
                }

                vector<unsigned> neighborArray[nVertices];
                for (unsigned i=0; i<nVertices; i++){
                    for (unsigned j=0; j<nEdges; j++){
                        if ((i+1)==edges1[j]){
                            neighborArray[i].push_back(edges2[j]);
                        }
                        if ((i+1)==edges2[j]){
                            neighborArray[i].push_back(edges1[j]);
                        }
                    }
                }
                unsigned distance[nVertices], prev[nVertices];
                if (bFSearch(neighborArray,source,destination,nVertices,distance,prev)==false){
                    cout<<"Error: No path exists between specified nodes."<<endl;
                }
                else{
                    vector<unsigned> path;
                    unsigned hop = destination;
                    path.push_back(hop);
                    while(prev[hop-1]!=-1){
                        path.push_back(prev[hop-1]);
                        hop = prev[hop-1];
                    }
                    for (unsigned i = path.size() - 1; i >= 1; i--)
                        cout << path[i] << "-";
                    cout<<path[0]<<endl;
                }
            }
        }
    }
    return(0);
}