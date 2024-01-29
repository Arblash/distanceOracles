#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <string>

using namespace std;

bool isStarLike = true;


int main(int argc, char* argv[]) {
    int n = stoi(argv[1]);
    double a = stod(argv[2]);
    long long seed = stoll(argv[3]);

    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);
    mt19937 rng(seed);

    int maxM = ceil(pow(n,a)*(n-1)/2);
    int minM = ceil(maxM/5);

    int m = rng()%(maxM - minM) + minM;

    vector<pair<int, int>> possibleEdges;

    if (isStarLike) {
        int stars = ceil(cbrt(n));
        // stars = stars * stars;
        for(int i = 0; i < stars; ++i) {
            for(int j = stars; j < n; ++j){
                possibleEdges.push_back(make_pair(i, j));
            }
        }
    } else {
        for(int i = 0; i < n; ++i) {
            for(int j = i + 1; j < n; ++j){
                possibleEdges.push_back(make_pair(i, j));
            }
        }
    }

    vector<pair<int, int>> edges;

    for (int i = 0; i < m; ++i){
        int viableRange = ((int)possibleEdges.size() - i);
        if(viableRange <= 0) break;
        int r = rng()%viableRange;
        edges.push_back(possibleEdges[r]);
        swap(possibleEdges[r], possibleEdges[viableRange-1]);
    }

    cout << n << " " << (int)edges.size() << "\n";
    for (int i = 0; i < (int)edges.size(); ++i){
        if(rng()%2 == 0) cout << edges[i].first << " " << edges[i].second << "\n";
        else cout << edges[i].second << " " << edges[i].first << "\n";
    }

    cout << flush;
    return 0;
}