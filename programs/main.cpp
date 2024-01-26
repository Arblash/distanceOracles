#include "global.hpp"
#include "2008.cpp"
#include "2010.cpp"

using namespace std;

// 2008 2010
int alg = 2008; 

int main(){
    initializeProgram();

    vector<int> A0;
    for (int i = 0; i < n; ++i) {
        A0.push_back(i);
    }
    double p = cbrt(n);
    vector<int> A1;
    if (alg == 2008)A1 = sample(A0, p);
    if (alg == 2008)A1 = samplePatRo(A0, p, p*p);

    vector<vector<int>> A1Dist;
    A1Dist.resize(n);
    vector<int> pA1;
    pA1.resize(n, -1);

    for(int i = 0; i < A1.size(); ++i) {
        int s = A1[i];
        A1Dist[s].resize(n, INT_MAX);
        vector<int> xs; xs.clear();
        xs.push_back(s); // TODO: simplify BFS call
        vector<BallElement> distances = BFS(INT_MAX, xs);
        for (int j = 0; j < distances.size(); ++j) {
            int u = distances[j].vertex;
            A1Dist[s][u] = distances[j].dist;
            if(pA1[u] == -1 || A1Dist[s][u] < A1Dist[pA1[u]][u]) pA1[i] = s;
        }
    }

    vector<unordered_map<int, int>> B_A1, C_A1;
    B_A1.resize(n); C_A1.resize(n);

    for (int i = 0; i < n; ++i) {
        vector<int> xs; xs.clear();
        xs.push_back(i);
        int r = INT_MAX;
        if (pA1[i] > -1) r = A1Dist[pA1[i]][i];
        vector<BallElement> distances = BFS(r, xs);
        for (int j = 0; j < distances.size(); ++j) {
            int u = distances[j].vertex;
            B_A1[i][u] = distances[j].dist;
            C_A1[u][i] = distances[j].dist;
        }
    }

    unordered_map<long long, int> OTable;

    for (int i = 0; i < n; ++i) {
        for (const auto& u : C_A1[i]) {
            for (const auto& w : C_A1[i]) {
                
                OTable[1LL * u.first * INT_MAX + w.first] = i;
            }
        }
    }

    // we can keep the exact distances for Case 2.
    for (int i = 0; i < n; ++i) {
        for (const auto& u : C_A1[i]) {
            for (const auto& w : C_A1[i]) {
                int v = OTable.find(1LL * u.first * INT_MAX + w.first)->second;
                int d1 = B_A1[u.first].find(v)->second;
                int d2 = B_A1[w.first].find(v)->second;

                int d3 = B_A1[u.first].find(i)->second;
                int d4 = B_A1[w.first].find(i)->second;

                if(d1 + d2 < d3 + d4) {
                    OTable[1LL * u.first * INT_MAX + w.first] = v;
                }
            }
        }
    }

    // Preprocessing finished

    int q; cin >> q;
    for (int i = 0; i < q; ++i) {
        int a, b; cin >> a >> b;
            // We can skip Case 1, if OTable[1LL * a * INT_MAX + b] remembers vertex
            // from the shortest path between a and b - we calculate exact distance
            // Case 2.
        if(OTable.find(1LL * a * INT_MAX + b) != OTable.end()) {
            int v = OTable.find(1LL * a * INT_MAX + b)->second;
            int d1 = B_A1[a].find(v)->second;
            int d2 = B_A1[b].find(v)->second;
            cout << d1 + d2 << "\n";
            // Case 3.
        } else {
            int d1 = A1Dist[pA1[a]][a] + A1Dist[pA1[a]][b];
            int d2 = A1Dist[pA1[b]][a] + A1Dist[pA1[b]][b];
            cout << min(d1, d2) << "\n";
        }
    }


    return 0;
}
