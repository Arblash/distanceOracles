#include<bits/stdc++.h>

using namespace std;

const int maxN = 100009;

vector<int> neighbours[maxN];

vector<int> A, B, C;

bool isInA[maxN];
bool isInB[maxN];
bool isInC[maxN];

int Cposition[maxN]; // positions of vertices in C vector, if vertex is not in C, then any value
vector<vector<int> > distFromC;
int distToC[maxN];
int NNC[maxN];

int isInCurrentBall[maxN];
bool isCurrentlyVisited[maxN];
unordered_map<pair<int, int>, int> ballCdistance;

vector<int> findBall(bool isInSet[], int centerVertex) {
    vector<int> ball;
    queue<pair<int, int> > border; //first index of vertex, second distance from vertex B[i]
    int distToSet = maxN;
    if (isInSet[centerVertex]) distToSet = 0;
    else {
        border.push({centerVertex, 0});
        isCurrentlyVisited[centerVertex] = true;
    }
    while (!border.empty()) {
        int v = border.front().first;
        int dist = border.front().second;
        if (distToSet <= dist) break;

        ball.push_back(v);
        border.pop();

        for (int j = 0; j < neighbours[v].size(); ++j) {
            int u = neighbours[v][j];
            if (isInSet[u]) distToSet = min(distToSet, dist + 1);
            else if (!isCurrentlyVisited[u]){
                border.push({u, dist + 1});
                isCurrentlyVisited[u] = true;
            }
        }
    }

    while (!border.empty()) {
        int v = border.front().first;
        border.pop();
        isCurrentlyVisited[v] = false;
    }
    for (int i = 0; i < ball.size(); ++i) {
        isCurrentlyVisited[ball[i]] = false;
    }

    return ball;
}

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

    // find sets A, B

    double AsampleProbability = pow(n, -1/3);
    double BsampleProbability = pow(n, -2/3);

    uniform_real_distribution<double> rng01(0, 1);
    
    for (int i = 0; i < n; ++i) {
        double randomNumber01 = rng01(rng);
        if (randomNumber01 < AsampleProbability) {
            A.push_back(i);
            isInA[i] = 1;
        }

        randomNumber01 = rng01(rng);
        if(randomNumber01 < BsampleProbability) {
            B.push_back(i);
            isInB[i] = 1;
        }
    }

    // find set C

    for (int i = 0; i < B.size(); ++i) {
        vector<int> ball = findBall(isInA, B[i]);

        for (int j = 0; j < ball.size(); ++j) {
            if (!isInC[ball[j]]) {
                C.push_back(ball[j]);
                isInC[ball[j]] = 1;
                Cposition[ball[j]] = C.size() - 1;
            }
        }
    }

    for (int i = 0; i < A.size(); ++i) {
        if (!isInC[A[i]]) {
            C.push_back(A[i]);
            isInC[A[i]] = 1;
            Cposition[A[i]] = C.size() - 1;
        }
    }

    for (int i = 0; i < n ; ++i) {
        distToC[i] = maxN;
        NNC[i] = -1;
    }

    // find distances from every vertex of C to every vertex of G and Nearest Neighour from C (NNC)

    for (int i = 0; i < C.size(); ++i) {
        int v = C[i];
        queue<pair <int, int> > nextVertex;
        nextVertex.push({v, 0});
        vector<int> distFromV(maxN);

        while (!nextVertex.empty()) {
            int v = nextVertex.front().first;
            int dist = nextVertex.front().second;
            nextVertex.pop();

            for (int j = 0; j < neighbours[v].size(); ++j) {
                int u = neighbours[v][j];
                if(distFromV[u] == 0) {
                    distFromV[u] = dist + 1;
                    nextVertex.push({u, dist + 1});
                }
            }
        }
        distFromV[v] = 0;
        distFromC.push_back(distFromV);
    }

    // find distances between vetices, that have intesecting Ball_C

    for (int i = 0; i < n; ++i) {
        if (isInC[i]) continue;
        vector<int> ballI = findBall(isInC, i);
        for (int j = 0; j < ballI.size(); ++j) {
            ++isInCurrentBall[ballI[j]];
        }
        for (int j = i + 1; j < n; ++j) {
            if(isInC[j]) continue;
            vector<int> ballJ = findBall(isInC, j);
            bool intersecting = 0;
            for (int k = 0; k < ballJ.size(); ++k) {
                if (isInCurrentBall[ballJ[k]] != 0) {
                    intersecting = 1;
                }
            }
            if (intersecting) {
                queue<pair<int, int> > nextVertices;
                vector<int> indicesToClear;
                nextVertices.push({i, 0});
                isCurrentlyVisited[i] = true;
                indicesToClear.push_back(i);
                while (!nextVertices.empty()) {
                    int v = nextVertices.front().first;
                    int dist = nextVertices.front().second;
                    nextVertices.pop();

                    for (int j = 0; j < neighbours[v].size(); ++j) {
                        int u = neighbours[v][j];
                        if (u == j) {
                            ballCdistance[{i, j}] = dist + 1;
                            break;
                        } else if (!isCurrentlyVisited[u]){
                            nextVertices.push({u, dist + 1});
                            isCurrentlyVisited[u] = true;
                            indicesToClear.push_back(u);
                        }
                    }
                }
                while (!nextVertices.empty()) nextVertices.pop();
                while (!indicesToClear.empty()) {
                    isCurrentlyVisited[indicesToClear[indicesToClear.size() - 1]] = false;
                    indicesToClear.pop_back();
                }
            }
        }
        for (int j = 0; j < ballI.size(); ++j) {
            --isInCurrentBall[ballI[j]];
        }
    }

    int q;
    cin >> q;

    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;
        int distance = maxN;
        if (ballCdistance.find({a, b}) != ballCdistance.end()) distance = ballCdistance[{a, b}];
        if (ballCdistance.find({b, a}) != ballCdistance.end()) distance = ballCdistance[{b, a}];
        if (distance != maxN) {
            cout << distance << "\n";
            continue;
        }
    }


    return 0;
}