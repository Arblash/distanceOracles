#include "global.hpp"
#include "2008.hpp"
#include "2010.hpp"
#include "hashTable.hpp"

using namespace std;


int main(int argc, char* argv[]){

    // 2005 2008 2010
    int alg = stoi(argv[1]);
    long long seed = stoll(argv[2]);

    initializeProgram(seed);

    auto startPreprocessingTime = clock_type::now();

    vector<int> A0;
    for (int i = 0; i < n; ++i) {
        A0.push_back(i);
    }
    double p = 1/cbrt(n);
    vector<int> A1;

    if (alg == 1000) A1 = A0;
    else if (alg == 2005)A1 = uniformSample(A0, ceil(p * A0.size()));
    else if (alg == 2008)A1 = sampleBasGoySen(A0, p);
    else if (alg == 2010)A1 = samplePatRod(A0, p, p*p);
    else {
        cerr << "unrecognized algorithm type" << endl;
    }

    vector<vector<int>> A1Dist;
    A1Dist.resize(n);
    vector<int> pA1;
    pA1.resize(n, -1);

    // cout << "Compute A1Dist and pA1" << endl;
    // Compute A1Dist and pA1
    for(int i = 0; i < (int)A1.size(); ++i) {
        int s = A1[i];
        // cout << "s = " << s <<endl;
        A1Dist[s].resize(n, maxDist);
        vector<int> xs; xs.clear();
        xs.push_back(s); // TODO: simplify BFS call
        vector<BallElement> distances = BFS(maxDist, xs);
        // cout << "BFS ended" << endl;
        for (int j = 0; j < (int)distances.size(); ++j) {
            int u = distances[j].vertex;
            A1Dist[s][u] = distances[j].dist;
            if(pA1[u] == -1 || A1Dist[s][u] < A1Dist[pA1[u]][u]) pA1[u] = s;
        }
    }

    // cout << "Compute B_A1" << endl;
    vector<Ball> B_A1 = computeBalls(A1);
    // cout << "Compute C_A1" << endl;
    vector<Ball> C_A1 = computeClustersFromBalls(B_A1);
    vector<unordered_map<u_int64_t, int, pthash_type>> B_A1Dist;

    // cout << "Compute B_A1Dist" << endl;
    // Compute B_A1Dist
    int BHashSpace = 0; // TODO
    for (int i = 0; i < n; ++i) {
        Ball currentBall = B_A1[i];
        vector<BallElement> distances = currentBall.vertices;
        vector<pair<long long, int>> pthashInput;
        for (int j = 0; j < (int)distances.size(); ++j) {
            pthashInput.push_back(make_pair(distances[j].vertex, distances[j].dist));
        }
        auto createPthashResult = createPthash(pthashInput);
        B_A1Dist.push_back(createPthashResult.hashTable);
        BHashSpace += createPthashResult.f.num_bits();
    }

    // cout << "Compute the OTable" << endl;
    // Compute the OTable
    vector<pair<long long, int>> OTablePthashInput;
    for (int i = 0; i < n; ++i) {
        for (const auto& u : C_A1[i].vertices) {
            for (const auto& w : C_A1[i].vertices) {
                
                OTablePthashInput.push_back(make_pair(1LL * u.vertex * maxDist + w.vertex, i));
            }
        }
    }

    auto createPthashResult = createPthash(OTablePthashInput);
    int OHashSpace = createPthashResult.f.num_bits();
    auto OTable = createPthashResult.hashTable;

    // we can keep the exact distances for Case 2. as the graph is unweighted
    for (int i = 0; i < n; ++i) {
        for (const auto& u : C_A1[i].vertices) {
            for (const auto& w : C_A1[i].vertices) {
                int v = OTable.find(1LL * u.vertex * maxDist + w.vertex)->second;

                int d1 = B_A1Dist[u.vertex].find(v)->second;
                int d2 = B_A1Dist[w.vertex].find(v)->second;

                int d3 = B_A1Dist[u.vertex].find(i)->second;
                int d4 = B_A1Dist[w.vertex].find(i)->second;

                if(d3 + d4 < d1 + d2) {
                    OTable[1LL * u.vertex * maxDist + w.vertex] = i;
                }
            }
        }
    }

    // cout << "Preprocessing finished" << endl;

    // Preprocessing finished

    
    auto startAnsweringTime = clock_type::now();
    
    int numAnswers = n*n;
    cout << numAnswers << "\n";

    // int q; cin >> q;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // int a, b; cin >> a >> b;
            int a = i, b = j;
                // We can skip Case 1, if OTable[1LL * a * maxDist + b] remembers vertex
                // from the shortest path between a and b - we calculate exact distance
            if(OTable.find(1LL * a * maxDist + b) != OTable.end()) {
                // Case 2.
                int v = OTable.find(1LL * a * maxDist + b)->second;
                int d1 = B_A1Dist[a].find(v)->second;
                int d2 = B_A1Dist[b].find(v)->second;
                cout << d1 + d2 << "\n";
            } else {
                // Case 3.
                if (pA1[a] == -1 || pA1[b] == -1) {
                    cout << "-1\n"; // a and b are not connected by any path
                } else {
                    int d1 = A1Dist[pA1[a]][a] + A1Dist[pA1[a]][b];
                    int d2 = A1Dist[pA1[b]][a] + A1Dist[pA1[b]][b];
                    if (min(d1, d2) >= maxDist) cout << "-1\n"; // a and b are not connected by any path
                    else cout << min(d1, d2) << "\n";
                }
            }
        }
    }

    double prepTime = seconds(startAnsweringTime - startPreprocessingTime); // TODO
    cout << prepTime << " ";
    double answerTime = seconds(clock_type::now() - startAnsweringTime); // TODO
    cout << answerTime << " ";
    int A1S = A1.size();
    cout << A1S << " ";
    int A1DistS = A1.size() * n;
    cout << A1DistS << " ";
    int B_A1S = 0;
    for (auto& ball : B_A1) B_A1S += ball.vertices.size();
    cout << B_A1S << " ";
    int C_A1S;
    for (auto& cluster : C_A1) C_A1S += cluster.vertices.size();
    cout << C_A1S << " ";
    int O_TableS = OTablePthashInput.size();
    cout << O_TableS << " ";
    cout << BHashSpace << " ";
    cout << OHashSpace << " ";

    cout << flush;
    return 0;
}
