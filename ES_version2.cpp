#include <iostream>
#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>
using namespace std;

static const int N = 60;
static const int m = 10;
static const int l = 30;
static const int seed = 149;
static const int C = 500;
vector<vector<int>> GenerateGraph(int n)
{ //generate Random Number
    srand(seed);
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
    ofstream total_data150("疎total_data103.csv");
    srand(seed);
    vector<vector<int>> graph = GenerateGraph(N);
    vector<pair<int, vector<int>>> pp(m+l, make_pair(0, vector<int>(N)));

    for (int i = 0; i < m;i++){
        for (int j = 0; j < N;j++)
            pp[i].second[j] = (double)3 * rand() / (RAND_MAX + 1.0);
        pp[i].first = Vio(pp[i].second, graph);
    }
    int cnt = 0;
    while(cnt < C && pp[0].first != 0){
        cnt++;
        for (int i = m; i < m + l; i++){
            int tmp = (double)m * rand() / (RAND_MAX + 1.0);
            int random = (double)N * rand() / (RAND_MAX + 1.0);
            for (int j = 0; j < N;j++){
                pp[i].second[j] = pp[tmp].second[j];
                if (j == random){
                    int x = (double)3 * rand() / (RAND_MAX + 1.0);
                    pp[i].second[j] = x;
                }
            }
            pp[i].first = Vio(pp[i].second, graph);
        }
        sort(pp.begin(), pp.end());
        int sum = 0;
        for (int i = 0; i < m;i++)
            sum += pp[i].first;

        total_data150 << pp[0].first << "," << pp[m - 1].first << "," << (double)sum / m << endl;
        if (cnt % 10 == 0)
            cout << "cnt: " << cnt << " first: " << pp[0].first << endl;
    }
    cout << "cnt: " << cnt << " first: " << pp[0].first << endl;

    return 0;
}
