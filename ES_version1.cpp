#include <iostream>
#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>
using namespace std;

static const int N = 60;
static const int m = 100;
static const int l = 500;
static const vector<int> seed = {101, 103, 107, 109, 113, 127, 131, 137, 139, 149};
static const int C = 200;
vector<vector<int>> GenerateGraph(int n,int s)
{ //generate Random Number
    srand(s);
    bool suc = false;
    int n_div = n / 3;
    //Tightly coupled problem
    int M = (n - 1) * n / 4;
    //Loose coupling problem
    M = 3 * n;
    vector<vector<int>> graph(n, vector<int>(n, 0));
    vector<int> ones(n * n_div, 0);
    while (!suc)
    {
        suc = true;

        //Divide into lattices
        for (int i = 0; i < M; i++)
            ones[i] = 1;

        for (int i = 0; i < 10000; i++)
            swap(ones[(double)n_div * n * rand() / (RAND_MAX + 1.0)], ones[(double)n_div * n * rand() / (RAND_MAX + 1.0)]);
        int ind = 0;
        for (int i = 0; i < 2 * n_div; i++)
        {
            if (i < n_div)
            {
                for (int j = 0; j < n_div; j++)
                {
                    graph[j][n_div + i] = ones[ind];
                    ind++;
                }
            }
            else
            {
                for (int j = 0; j < 2 * n_div; j++)
                {
                    graph[j][n_div + i] = ones[ind];
                    ind++;
                }
            }
        }
        //Check graph path is connected
        for (int i = 0; i < n; i++)
        {
            bool tmp_suc = false;
            for (int j = 0; j < n; j++)
            {
                if (j < i)
                {
                    if (graph[j][i] == 1)
                        tmp_suc = true;
                }
                else
                {
                    if (graph[i][j] == 1)
                        tmp_suc = true;
                }
            }
            if (tmp_suc == false)
                suc = false;
        }
    }
    return graph;
}

int Vio(vector<int> vec, vector<vector<int>> graph)
{
    int size = vec.size();
    int violation = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (graph[i][j] == 1 && vec[i] == vec[j])
            {
                violation++;
            }
        }
    }
    return violation;
}

int main(){
    long long com = 0;
    int success = 0;
    for (int t = 0; t < 10;t++){
        srand(seed[t]);
        int q = 0;
        vector<vector<int>> graph = GenerateGraph(N,seed[t]);
        vector<pair<int, vector<int>>> pp(m, make_pair(0, vector<int>(N)));
        vector<pair<int, vector<int>>> ppl(l, make_pair(0, vector<int>(N)));

        for (int i = 0; i < m;i++){
            for (int j = 0; j < N;j++)
                pp[i].second[j] = (double)3 * rand() / (RAND_MAX + 1.0);
            pp[i].first = Vio(pp[i].second, graph);
        }
        int cnt = 0;
        while(cnt < C && pp[0].first != 0){
            q++;
            cnt++;
            for (int i = 0; i < l; i++){
                q++;
                int tmp = (double)m * rand() / (RAND_MAX + 1.0);
                int random = (double)N * rand() / (RAND_MAX + 1.0);
                for (int j = 0; j < N;j++){
                    ppl[i].second[j] = pp[tmp].second[j];
                    if(j == random){
                        int x = (double)3 * rand() / (RAND_MAX + 1.0);
                        ppl[i].second[j] = x;
                    }
                }
                ppl[i].first = Vio(ppl[i].second, graph);
            }
            sort(ppl.begin(), ppl.end());
            for (int i = 0; i < m;i++)
                pp[i] = ppl[i];
        }
        if(pp[0].first == 0){
            com += q;
            success++;
        }
        cout << "q: " << q << endl;
    }
    cout << "success: " << success << endl;
    cout << "average: " << com / success << endl;
    return 0;
}
