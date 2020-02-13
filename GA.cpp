#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>

using namespace std;

static const int N = 150;
static const int M = 200;
static const int C = 500; 
static const vector<int> seed = {101,103,107,109,113,127,131,137,139,149
};
//,151,157,163,167,173,179,181,191,193,197};
static const double rink = 3*N;
//static const double rink = (N - 1) * N / 4;
static const double kakuritu = 0.005;
vector<vector<int>> GenerateGraph(int n,int q)
{ //generate Random Number
    srand(seed[q]);
    bool suc = false;
    int n_div = n / 3;
    //Tightly coupled problem
    int M = rink;
    //Loose coupling problem

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

double Vio(vector<int> vec, vector<vector<int>> graph)
{
    int size = vec.size();
    double violation = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (graph[i][j] == 1 && vec[i] == vec[j])
            {
                violation+= 1.0;
            }
        }
    }
    return violation;
}

double powScale(double d){
    return d * d * d * d * d * d * d * d * d * d * d * d * d * d * d;
}

double senkeiScale(double d,double max,double min){
    return (d - min) / (max - min);
}

vector<int> Mask(int N){
    vector<int> tmp(N, 0);
    for (int i = 0; i < N / 2;i++)
        tmp[i] = 1;
    for (int i = 0; i < 10000;i++){
        swap(tmp[(double)N * rand() / (RAND_MAX + 1.0)], tmp[(double)N * rand() / (RAND_MAX + 1.0)]);
    }
    return tmp;
}

int main(){
    long long com = 0;
    int success = 0;
    for (int q = 0; q < 10;q++){
            srand(seed[q]);
        vector<vector<int>> graph = GenerateGraph(N,q);
        double asum = 0;
        
        vector<int> mask;
        mask = Mask(N);

        vector<pair<double, vector<int>>> bp(M, make_pair(0, vector<int>(N)));
        vector<pair<double, vector<int>>> ap(M, make_pair(0, vector<int>(N)));
        double sum = 0;
        for (int i = 0; i < M;i++){
            for (int j = 0; j < N;j++)
                bp[i].second[j] = (double)3 * rand() / (RAND_MAX + 1.0);
            bp[i].first = 1.0 - Vio(bp[i].second, graph)/rink;
            sum += bp[i].first;
        }
        sort(bp.begin(),bp.end());
        vector<pair<double, vector<int>>> tmpp = bp;
        for (int i = 0; i < M;i++)
            bp[i] = tmpp[M - 1 - i];
        int cnt = 0;
        
        while(cnt < C && bp[0].first < 1.0){
            mask = Mask(N);
            cnt++;
            //線形スケーリング用
            //double tMax = bp[0].first, tMin = bp[M - 1].first;
            double tSum = 0;
            //スケーリング
            for (int i = 0; i < M;i++){
                //cout << i << "番目は" << bp[i].first << endl;
                //bp[i].first = senkeiScale(bp[i].first, tMax, tMin);
                bp[i].first = powScale(bp[i].first);
                //cout << i << "番目は" << bp[i].first << endl;
                tSum += bp[i].first;
            }
            vector<double> roulette(M,0);

            for (int i = 0; i < M;i++){
                roulette[i] += bp[i].first / tSum;
                //cout << i << "番目は" << roulette[i] << endl;
            }
            //cout << rous << endl;
            for (int i = 0; i < M; i += 2){
                asum += 2;
                double ra1 = (double)rand() / (RAND_MAX + 1.0);
                double ra2 = (double)rand() / (RAND_MAX + 1.0);
                int ind1 = 0, ind2 = 0;
                double tmp1 = 0,tmp2 = 0;
                //cout << ra1 << endl;
                while(tmp1 <= ra1 && ind1 < M-1){
                    //cout << tmp1 << endl;
                    tmp1 += roulette[ind1];
                    ind1++;
                    }
                while(tmp2 <= ra2 && ind2 < M-1){ 
                    tmp2 += roulette[ind2];
                    ind2++;
                    }
                    //cout <<"ra1 = "<<ra1 << " ind1 = "<< ind1 <<" tmp1 = " <<tmp1 <<"  ra2 = " <<ra2 << " ind2 = "<< ind2 <<" tmp2 = "<< tmp2<< endl;
                    for (int j = 0; j < N; j++)
                    {
                        double m1 = (double)rand() / (RAND_MAX + 1.0);
                        double m2 = (double)rand() / (RAND_MAX + 1.0);

                        if (mask[j] == 1)
                        {
                            ap[i].second[j] = bp[ind2].second[j];
                            ap[i + 1].second[j] = bp[ind1].second[j];
                        }
                        else
                        {
                            ap[i].second[j] = bp[ind1].second[j];
                            ap[i + 1].second[j] = bp[ind2].second[j];
                        }
                        if (m1 <= kakuritu){
                            int tmpo = ap[i].second[j];
                            while(ap[i].second[j] == tmpo)
                                ap[i].second[j] = (double)3 * rand() / (RAND_MAX + 1.0);
                        }
                        if (m2 <= kakuritu){
                            int tmpo = ap[i+1].second[j];
                            while (ap[i+1].second[j] == tmpo)
                                ap[i + 1].second[j] = (double)3 * rand() / (RAND_MAX + 1.0);
                        }
                    }
                ap[i].first = 1.0 - Vio(ap[i].second, graph) / rink;
                ap[i + 1].first = 1.0 - Vio(ap[i+1].second, graph) / rink;
            }
            sort(ap.begin(),ap.end());
            for (int i = 0; i < M;i++){
                bp[i] = ap[M-1-i];
            }
        }
        if (bp[0].first == 1){
            com += asum;
            success++;
        }
            cout << bp[0].first <<" sum: "<< asum << endl;
    }
    cout << "success: " << success << endl;
    cout << "average: " << com / success << endl;
    return 0;
}
