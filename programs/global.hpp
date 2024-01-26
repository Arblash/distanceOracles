#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <queue>
#include <unordered_map>

using namespace std;

const int maxN = 10000;

vector<int> neighbors[maxN];
int n, m;

struct BallElement {
    int vertex;
    int dist;
};

struct Ball {
    int x;
    int r;
    vector<BallElement> vertices;
};

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void initializeProgram() {
    ios_base::sync_with_stdio(0);

    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        neighbors[a].push_back(b);
        neighbors[b].push_back(a);
    }

    return;
}

vector<int> uniformSample(vector<int> &X, int sampleSize) {
    vector<int> sample;
    if (sampleSize >= X.size()) return X;

    for(int i = 0; i < sampleSize; ++i){
        int samplingMax = 1*X.size() - i;
        int elementIndex = rng() % samplingMax;
        sample.push_back(X[elementIndex]);
        swap(X[elementIndex], X[samplingMax - 1]);
    }

    return sample;
}

vector<BallElement> BFS(int r, vector<int> &xs) {
    vector<BallElement> result;
    queue<BallElement> queue;
    if(r > 0) {
        for (int i = 0; i < xs.size(); ++i) {
            BallElement e;
            e.vertex = xs[i]; e.dist = 0;
            queue.push(e);
        }
    }

    vector<bool> isVisited; isVisited.insert(isVisited.end(), n, false);
    while(!queue.empty()) {
        BallElement current = queue.front(); queue.pop();
        result.push_back(current);
        if(current.dist + 1 < r) {
            for (int i = 0; i < neighbors[current.vertex].size(); ++i) {
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

vector<Ball> computeBalls(vector<int> &R) {
    vector<Ball> balls;
    vector<BallElement> distR = BFS(INT_MAX, R);
    for (int i = 0; i < distR.size(); ++i) {
        balls.push_back(computeBall(distR[i].dist, distR[i].vertex));
    }
    return balls;
}

vector<Ball> computeClustersFromBalls(vector<Ball> &balls) {
    vector<Ball> clusters;
    for (int i = 0; i < n; ++i) {
        Ball c;
        c.x = i;
        clusters.push_back(c);
    }
    for (int i = 0; i < balls.size(); ++i) {
        Ball ball = balls[i];
        for (int j = 0; j < ball.vertices.size(); ++j) {
            BallElement ballVertex = ball.vertices[i];
            BallElement clusterVertex;
            clusterVertex.dist = ballVertex.dist;
            clusterVertex.vertex = ball.x;
            clusters[ballVertex.vertex].vertices.push_back(clusterVertex);
        }
    }
    return clusters;
}