#pragma once
#include "global.hpp"

vector<int> sample(vector<int> &X, double p) {
    vector<int> R, X1;
    X1.assign(X.begin(), X.end());

    while (!X1.empty()) {
        vector<int> S = uniformSample(X1, ceil(p * X.size()));
        R.insert(R.end(), S.begin(), S.end());
        vector<Ball> B_R = computeBalls(R);
        vector<Ball> C_R = computeClustersFromBalls(B_R);
        vector<int> newX1;
        newX1.clear();
        // Once vertex is out of X1 it will never be included again, hence only elements from X1 need to be checked
        for (int i = 0; i < X1.size(); ++i) { 
            int vertex = X1[i];
            if(p * C_R[vertex].vertices.size() > 4) newX1.push_back(C_R[vertex].x);
        }
        swap(X1, newX1);
    }

    return R;
}