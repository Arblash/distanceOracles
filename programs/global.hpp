#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <queue>
#include <string>

// #define size(x) ((int)(x).size())

using namespace std;

vector<vector<int>> neighbors;
int n, m;

// for convenience we set up maximum distance a bit lower than INT_MAX
int maxDist = 1 << 30;

struct BallElement {
    int vertex;
    int dist;
};

struct Ball {
    int x;
    int r;
    vector<BallElement> vertices;
};


mt19937 rng;

void initializeProgram(long long seed) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    mt19937 newRng(seed);
    rng = newRng; // 
    cin >> n >> m;
    neighbors.resize(n);

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        neighbors[a].push_back(b);
        neighbors[b].push_back(a);
    }

    return;
}

vector<int> uniformSample(vector<int>& X, int sampleSize) {
    vector<int> sample;
    if (sampleSize >= (int)X.size()) return X;

    for(int i = 0; i < sampleSize; ++i){
        int samplingMax = (int)X.size() - i;
        int elementIndex = rng() % samplingMax;
        sample.push_back(X[elementIndex]);
        swap(X[elementIndex], X[samplingMax - 1]);
    }

    return sample;
}

vector<BallElement> BFS(int r, vector<int>& xs) {
    vector<BallElement> result;
    queue<BallElement> queue;
    if(r > 0) {
        for (int i = 0; i < (int)xs.size(); ++i) {
            BallElement e;
            e.vertex = xs[i]; e.dist = 0;
            queue.push(e);
        }
    }

    vector<bool> isVisited; isVisited.resize(n, false);
    while(!queue.empty()) {
        BallElement current = queue.front(); queue.pop();
        if(isVisited[current.vertex]) continue;
        else isVisited[current.vertex] = true;
        result.push_back(current);
        if(current.dist + 1 < r) {
            for (int i = 0; i < (int)neighbors[current.vertex].size(); ++i) {
                BallElement nextVertex;
                nextVertex.vertex = neighbors[current.vertex][i];
                nextVertex.dist = current.dist + 1;
                queue.push(nextVertex);
            }
        }
    }
    return result;
}

Ball computeBall(int r, int x) {
    Ball ball; 
    ball.r = r; ball.x = x;
    vector<int> xs; xs.push_back(x);
    ball.vertices = BFS(r, xs);

    return ball;
}

vector<Ball> computeBalls(vector<int>& R) {
    vector<BallElement> distRData = BFS(maxDist, R);
    vector<int> distR; distR.resize(n, maxDist);
    for (int i = 0; i < (int)distRData.size(); ++i) {
        distR[distRData[i].vertex] = distRData[i].dist; 
    }
    vector<Ball> balls;
    for(int i = 0; i < n; ++i) {
        balls.push_back(computeBall(distR[i], i));
    }
    return balls;
}

vector<Ball> computeClustersFromBalls(vector<Ball>& balls) {
    vector<Ball> clusters;
    for (int i = 0; i < n; ++i) {
        Ball c;
        c.x = i;
        c.r = -1; // clusters do not have radius - suppress warnings
        clusters.push_back(c);
    }
    for (int i = 0; i < (int)balls.size(); ++i) {
        Ball ball = balls[i];
        for (int j = 0; j < (int)ball.vertices.size(); ++j) {
            BallElement ballVertex = ball.vertices[j];
            BallElement clusterVertex;
            clusterVertex.dist = ballVertex.dist;
            clusterVertex.vertex = ball.x;
            clusters[ballVertex.vertex].vertices.push_back(clusterVertex);
        }
    }
    return clusters;
}