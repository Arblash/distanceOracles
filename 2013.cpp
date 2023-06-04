#include<bits/stdc++.h>

using namespace std;

const int maxN = 1000009;

vector<int> neighbours[maxN];

vector<int> L;
vector<int> distToL[maxN];
int l[maxN];
unordered_map<int, int> distToS[maxN];

vector<int> W;
int sizeOfInverseBall[maxN];
bool isInL[maxN];
bool isBfsVisited[maxN];

int main(){
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    ios_base::sync_with_stdio(0);

    // read graph

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        neighbours[a].push_back(b);
        neighbours[b].push_back(a);
    }

    // find set L

    int alpha = 2*pow(n, 2/3) * pow(m, -1/3) * pow(log2(n), 1/3);
    int sampleSize = 4*n/alpha;

    for(int i = 0; i < n; ++i) {
        W.push_back(i);
    }

    while (W.size() > 0) {
        shuffle(W.begin(), W.end(), rng);
        for (int i = 0; i < sampleSize; ++i) {
            if(W.size() == 0) break;
            L.push_back(W[W.size() - 1]);
            isInL[W[W.size() - 1]] = true;
            W.pop_back();
        }
        for (int i = 0; i < n; ++i) sizeOfInverseBall[i] = 0;

        for (int i = 0; i < n; ++i) {
            vector<int> currentLayer, nextLayer, visited;
            currentLayer.push_back(i);
            while(currentLayer.size() > 0) {
                bool endLoop = false;
                for (int j = 0; j < currentLayer.size(); ++j) {
                    int v = currentLayer[j];
                    endLoop = true;
                }
                if (endLoop) break;

                for (int j = 0; j < currentLayer.size(); ++j) {
                    int v = currentLayer[j];
                    if (isBfsVisited[v]) continue;
                    isBfsVisited[v] = true;
                    visited.push_back(v);
                    ++sizeOfInverseBall[v];
                    for (int k = 0; k < neighbours[v].size(); ++k) {
                        int u = neighbours[v][k];
                        nextLayer.push_back(u);
                    }
                }
                swap(currentLayer, nextLayer);
                nextLayer.clear();
            }
            currentLayer.clear();
            nextLayer.clear();
            for (int j = 0; j < visited.size(); ++j) {
                isBfsVisited[visited[j]] = false;
            }
            visited.clear();
        }

        vector<int> newW;
        for (int i = 0; i < n; ++i) {
            if (sizeOfInverseBall[i] > alpha) newW.push_back(i)
        }
        swap(W, newW);
        newW.clear();
    }

    // preprocess oracle

    for (int i = 0; i < L.size(); ++i) {
        vector<int> currentLayer, nextLayer;
        distToL[L[i]].push_back(0);
        currentLayer.push_back(L[i]);
        int dist = 0;
        while (currentLayer.size() > 0) {
            int v = currentLayer[currentLayer.size() - 1];
            currentLayer.pop_back();
            distToL[v].push_back(dist);
            for (int j = 0; j < neighbours[v].size(); ++j) {
                int u = neighbours[v][j];
                if(distToL[u].size() == i) {
                    distToL[u].push_back(dist + 1);
                    nextLayer.push_back(u);
                }
            }
            if(currentLayer.size() == 0) {
                swap(currentLayer, nextLayer);
                ++dist;
            }
        }
    }

    for (int i = 0; i < n; ++ i) {
        l[i] = maxN;
        for (int j = 0; j < L.size(); ++j) {
            l[i] = min(l[i], distToL[i][j])
        }
    }

    for (int i = 0; i < n; ++i) {
        vector<int> currentLayer, nextLayer;
        currentLayer.push_back(i);
        int dist = 0;
        while(1) {
            bool endLoop = false;
            if (currentLayer.size() == 0) {
                swap(currentLayer, nextLayer);
                ++dist;
                if (currentLayer.size() == 0) break;
            }
            int v = currentLayer[currentLayer.size() - 1];
            currentLayer.pop_back();
            if (distToS[i].find(v) != distToS[i].end()) 
            distToS[i][v] = dist;
            for (int j = 0; j < neighbours[v].size(); ++j) {
                int u = neighbours[v][j];
                if (isInL[u]) endLoop = true;
                nextLayer.push_back(u);
            }
            if(endLoop) break;
        }
        currentLayer.clear();
        nextLayer.clear();
    }

    // TODO: finish distToS

    // TODO: return distance for queries

    


    return 0;
}