#pragma once
#include "global.hpp"

vector<int> samplePatRod(vector<int> &X, double p1, double p2) {
    vector<int> R = uniformSample(X, ceil(p1 * X.size()));
    vector<int> S = uniformSample(X, ceil(p2 * X.size()));
    vector<bool> isInS; isInS.resize(n, false);
    for (int i = 0; i < (int)S.size(); ++i) {
        isInS[S[i]] = true;
    }
    vector<Ball> B_R = computeBalls(R);
    vector<int> Sample;
    vector<bool> isInSample; isInSample.resize(n, false);
    for (int i = 0; i < (int)B_R.size(); ++i) {
        if(isInS[B_R[i].x]) {
            for (int j = 0; j < (int)B_R[i].vertices.size(); ++j) {
                isInSample[B_R[i].vertices[j].vertex] = true;
            }
        }
    }
    for (int i = 0; i < (int)R.size(); ++i) {
        isInSample[R[i]] = true;
    }


    for (int i = 0; i < n; ++i) {
        if(isInSample[i]) Sample.push_back(i);
    }
    return Sample;
}