
#include <bits/stdc++.h>

using namespace std;

string generateGraph = "generateGraph";
string program = "main";
vector<int> runModes = {1000, 2005, 2008, 2010};
string path = "tests/";
string testPref = "test";
vector<double> aVector = {1, 0.75, 0.5, 0.25, 0}; // for random graphs
// vector<double> aVector = {0.3, 0.15}; // for star like graphs
bool isStarLike = false;
int baseN = 2000;
int rangeN = 100;
int numTests = 10;
bool regenerate = true;

string getGenerateGraphCommand(int n, double a, long long seed, int testSuf) {
    return "./" + generateGraph + " " + to_string(n) + " " + to_string(a) + " " + to_string(seed) + " " + to_string(isStarLike) +
        " > " + path + testPref + to_string(testSuf) + ".in";
}

double averageFromVector(vector<double> vec) {
    double sum = 0;
    for (double value : vec) {
        sum += value;
    }
    if(vec.size()) sum /= vec.size();
    return sum;
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    mt19937 rng(802120171534790653LL);
    // cout<<fixed<<setprecision(15);

    cout << "GenerateGraphs" << endl;

    if(regenerate){
        for(int i = 0; i < (int)aVector.size(); ++i){
            double a = aVector[i];
            for(int j = 0; j < numTests; ++j){
                int n = baseN - rangeN + rng()%(2*rangeN + 1);
                long long seed = rng();
                string cmd = getGenerateGraphCommand(n, a, seed, i*numTests + j);
                int status = system(cmd.c_str());
            }
        }
    }


    cout << "runTests" << endl;

    for(int k = 0; k < (int)aVector.size(); ++k){

        for(int i = 0; i < numTests; ++i){
            int currentTestSeed = rng();
            for(int j = 0; j < (int)runModes.size(); ++j) {
                cout << "test " << k*numTests + i << " mode " << runModes[j]  << endl;
                string cmd = "./" + program + " " + to_string(runModes[j]) + " " + to_string(currentTestSeed) + " < " + path + testPref + to_string(k*numTests + i) + ".in" + 
                " > " + path + testPref + to_string(k*numTests + i) + "." + to_string(runModes[j]);
                int status = system(cmd.c_str());
            }
        }

        for(int j = 0; j < (int)runModes.size(); ++j) {
            vector<double> preprocessingTime; preprocessingTime.clear();
            vector<double> answeringTime; answeringTime.clear();
            vector<double> A1Size; A1Size.clear();
            vector<double> A1DistSize; A1DistSize.clear();
            vector<double> B_A1Size; B_A1Size.clear();
            vector<double> C_A1Size; C_A1Size.clear();
            vector<double> O_TableSize; O_TableSize.clear();
            vector<double> BallHashFuncSpaceTaken; BallHashFuncSpaceTaken.clear();
            vector<double> OHashFuncSpaceTaken; OHashFuncSpaceTaken.clear();
            vector<double> QueriesCount; QueriesCount.clear();
            vector<vector<double>> RelativeErrors; RelativeErrors.clear();
            vector<double> averageRelativeErrors; averageRelativeErrors.clear();
            vector<double> maxErrorCount; maxErrorCount.clear();
            vector<double> maxErrorPercentage; maxErrorPercentage.clear();
            for(int i = 0; i < numTests; ++i){
                ifstream exact(path + testPref + to_string(k*numTests + i) + ".1000");
                ifstream approx(path + testPref + to_string(k*numTests + i) + "." + to_string(runModes[j]));
                double dumpD; int dumpI;

                int numAnswers; approx >> numAnswers; exact >> dumpI;

                int numMaxErrors = 0;
                double avgError = 0;
                vector<double> relErrors; relErrors.clear();
                for (int q = 0; q < numAnswers; ++q) {
                    int distApprox; approx >> distApprox;
                    int distExact; exact >> distExact;
                    // if(distExact > distApprox) return EXIT_FAILURE;
                    // if(distApprox > 2*distExact + 1) return EXIT_FAILURE;
                    if (distExact == 0 || distExact == -1) { // -1 stands for not connected by any path
                        if(distApprox != distExact) cout << q << " issue" << endl;
                        if(distApprox != distExact) return EXIT_FAILURE;
                        relErrors.push_back(0);
                    } else {
                        relErrors.push_back(((double)distApprox - distExact)/distExact);
                        if(distApprox == 2*distExact + 1) ++numMaxErrors;
                    }
                    avgError += relErrors[q];
                }
                avgError/=numAnswers;

                double prepTime; approx >> prepTime; exact >> dumpD;
                double answerTime; approx >> answerTime; exact >> dumpD;
                int A1S; approx >> A1S; exact >> dumpI;
                int A1DistS; approx >> A1DistS; exact >> dumpI;
                int B_A1S; approx >> B_A1S; exact >> dumpI;
                int C_A1S; approx >> C_A1S; exact >> dumpI;
                int O_TableS; approx >> O_TableS; exact >> dumpI;
                int BHashSpace; approx >> BHashSpace; exact >> dumpI;
                int OHashSpace; approx >> OHashSpace; exact >> dumpI;

                preprocessingTime.push_back(prepTime);
                answeringTime.push_back(answerTime / numAnswers);
                A1Size.push_back(A1S);
                A1DistSize.push_back(A1DistS);
                B_A1Size.push_back(B_A1S);
                C_A1Size.push_back(C_A1S);
                O_TableSize.push_back(O_TableS);
                BallHashFuncSpaceTaken.push_back(BHashSpace);
                OHashFuncSpaceTaken.push_back(OHashSpace);
                QueriesCount.push_back(numAnswers);
                RelativeErrors.push_back(relErrors);
                averageRelativeErrors.push_back(avgError);
                maxErrorCount.push_back(numMaxErrors);
                maxErrorPercentage.push_back(((double)numMaxErrors) / numAnswers);
            }

            cout << "Tests for n = " << baseN << "+-" << rangeN << " a = " << aVector[k] << " mode = " << runModes[j] << " completed:\n";
            cout << "preprocessingTime " << averageFromVector(preprocessingTime) << "\n";
            cout << "TimePerAnswer " << averageFromVector(answeringTime) << "\n";
            cout << "A1Size " << averageFromVector(A1Size) << "\n";
            cout << "A1DistSize " << averageFromVector(A1DistSize) << "\n";
            cout << "B_A1Size " << averageFromVector(B_A1Size) << "\n";
            cout << "C_A1Size " << averageFromVector(C_A1Size) << "\n";
            cout << "O_TableSize " << averageFromVector(O_TableSize) << "\n";
            cout << "BallHashFuncSpaceTaken " << averageFromVector(BallHashFuncSpaceTaken) << "\n";
            cout << "OHashFuncSpaceTaken " << averageFromVector(OHashFuncSpaceTaken) << "\n";
            cout << "averageRelativeError " << averageFromVector(averageRelativeErrors) << "\n";
            cout << "maxErrorPercentage " << averageFromVector(maxErrorPercentage) << "\n";
            cout << endl;

        }
    }


    cout << "Successfully tested!" << endl;


    return 0;
}




/*
    __builtin_popcount count total set bits on a given integer
    (use __builtin_popcountll for long long).


*/


