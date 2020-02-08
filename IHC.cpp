#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <utility>
using namespace std;

static const int N = 60;
static const int hc = 500;
static const int ihc = 10;

//初期のグラフを生成し返す関数
vector<vector<int>> GenerateGraph(vector<int> vec)
{ //generate Random Number
    srand(101);
    bool suc = false;
    int n = vec.size();
    int n_div = n / 3;
    //Tightly coupled problem
    //int m = n * (n - 1) / 4;
    //Loose coupling problem
    int m = 3*n;
    vector<vector<int>> graph(n, vector<int>(n, 0));
    vector<int> ones(3 * n, 0);
    while (!suc)
    {
        suc = true;

        //Divide into lattices
        for (int i = 0; i < m; i++)
            ones[i] = 1;

        if (m != 3 * n)
            for (int i = 0; i < 10000; i++)
                swap(ones[(double)3 * n * rand() / (RAND_MAX + 1.0)], ones[(double)3 * n * rand() / (RAND_MAX + 1.0)]);

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

//全体の総罰則点を返す関数
int Vio(vector<int> vec,vector<vector<int>> graph){
    int size = vec.size();
    int violation = 0;
    for (int i = 0; i < size;i++){
        for (int j = i+1; j < size;j++){
            if(graph[i][j] == 1 && vec[i] == vec[j]){
                violation++;
            }
        }
    }
    return violation;
}

//与えられた一次元配列と二次元配列を用いて各領域（色塗れる場所）の罰則点を配列で返す関数
vector<int> EachVio(vector<int> vec, vector<vector<int>> graph){
    int size = vec.size();
    vector<int> eachVio(size, 0);
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (graph[i][j] == 1 && vec[i] == vec[j])
            {
                eachVio[i]++;
                eachVio[j]++;
            }
        }
    }
    return eachVio;
}

//与えられた配列中の最小値を返す関数
int minVec(vector<int> vec){
    int tmp = 100000;
    int size = vec.size();
    for (int i = 0; i < size;i++){
        if(vec[i] < tmp)
            tmp = vec[i];
    }
    return tmp;
}

int main(){
    ofstream IHC("IHC.csv");
    ofstream IhcGraph("IhcGraph");

    srand(103);

    int icnt = 0;
    vector<int> node(N, 0);
    vector<vector<int>> graph = GenerateGraph(node);
    //罰則点が０点になるか、リスタート数が指定された値に達するまで色を書き換える。
    while (Vio(node, graph) != 0 && icnt < ihc){
        vector<int> check(3,0);
        for (int i = 0; i < N;i++)
            node[i] = (double)3 * rand() / (RAND_MAX + 1.0);
        
        int cnt = 0;

        //罰則点が０点になるか、試行回数が指定された値に達するまで色を書き換える
        while(Vio(node,graph) != 0 && cnt < hc){
            IHC << Vio(node, graph) << endl;
            vector<int> vioCnt = EachVio(node, graph);
            int tmp = (double)N * rand() / (RAND_MAX + 1.0);

            //罰則点がある領域からランダムで１つの領域を選択する
            while(vioCnt[tmp] == 0)
                tmp = (double)N * rand() / (RAND_MAX + 1.0);

            //各色に書き換えた時の罰則点を確保する
            int hoge = node[tmp];
            // << hoge << endl;
            for (int i = 0; i < 3;i++){
                node[tmp] = i;
                if(i != hoge){
                check[i] = Vio(node, graph);
                }else{
                    check[i] = 214748364;
                }
            }

            //各色に書き換えた際の罰則点から最小になるような値を求める。
            int min = minVec(check);
            int ind = (double)3 * rand() / (RAND_MAX + 1.0);

            //罰則点が最小値になる色を（複数ある場合はランダムで）選択する。
            while(check[ind] > min || ind == hoge)
                ind = (double)3 * rand() / (RAND_MAX + 1.0);
            node[tmp] = ind;
            cnt++;
        }
    icnt++;
    }
    if(Vio(node,graph) == 0)
        IHC << 0 << endl;
    // << "count:" << icnt << " violation:" << Vio(node, graph) << endl;
    IhcGraph << ",";
    for (int i = 0; i < N;i++)
        IhcGraph << node[i] << ",";
    IhcGraph << endl;
    for (int i = 0; i < N;i++){
        IhcGraph << node[i] << ",";
        for (int j = 0; j < N;j++){
            if(j < i+1){
                IhcGraph << " ,";
            }else{
                IhcGraph << graph[i][j] << ",";
            }
        }
        IhcGraph << endl;
    }
        return 0;
}
