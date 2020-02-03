#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

static const int N = 30;
static const int hc = 100;

//初期のグラフを生成し返す関数
vector<vector<int>> GenerateGraph(vector<int> vec){
    srand(101);
    int n = vec.size();
    int n_div = n / 3;
    //Tightly coupled problem
    int m = n * (n - 1) / 4;
    //Loose coupling problem
    //int m = 3 * n;
    bool suc = false;
    vector<vector<int>> graph(n, vector<int>(n, 0));
    while (!suc){
        suc = true;
        //Divide into lattices
        vector<vector<int>> mini_graph(3, vector<int>(2, 0));
        mini_graph[0][0] = 0;
        mini_graph[0][1] = n_div;
        mini_graph[1][0] = 0;
        mini_graph[1][1] = 2 * n_div;
        mini_graph[2][0] = n_div;
        mini_graph[2][1] = 2 * n_div;

        for (int i = m; i > 0; i--)
        {
            int tmp = (double)3 * rand() / (RAND_MAX + 1.0);
            int ind_x = mini_graph[tmp][0], ind_y = mini_graph[tmp][1];

            int rand_xval = (double)(n / 3) * rand() / (RAND_MAX + 1.0);
            int rand_yval = (double)(n / 3) * rand() / (RAND_MAX + 1.0);
            if (graph[ind_x + rand_xval][ind_y + rand_yval] == 1)
                i++;
            graph[ind_x + rand_xval][ind_y + rand_yval] = 1;
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
    vector<int> eachVio(size, 0);
    int violation = 0;
    for (int i = 0; i < size;i++){
        for (int j = i+1; j < size;j++){
            if(graph[i][j] == 1 && vec[i] == vec[j]){
                violation++;
                eachVio[i]++;
                eachVio[j]++;
            }
        }
    }
    return violation;
}

//与えられた一次元配列と二次元配列を用いて各領域（色塗れる場所）の罰則点を配列で返す関数
vector<int> EachVio(vector<int> vec, vector<vector<int>> graph){
    int size = vec.size();
    vector<int> eachVio(size, 0);
    int violation = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (graph[i][j] == 1 && vec[i] == vec[j])
            {
                violation++;
                eachVio[i]++;
                eachVio[j]++;
            }
        }
    }
    return eachVio;
}

//与えられた配列中の最小値を返す関数
int minVec(vector<int> vec){
    int tmp = 2147483647;
    int size = vec.size();
    for (int i = 0; i < size;i++){
        if(vec[i] < tmp)
            tmp = vec[i];
    }
    return tmp;
}

int main(){
    srand(101);

    ofstream ofs("IC.csv");
    ofstream file1("IcGraph.csv");

    vector<int> node(N,0);
    vector<vector<int>> graph = GenerateGraph(node);
    vector<int> check(3, 0);
    for (int i = 0; i < N;i++)
        node[i] = (double)3 * rand() / (RAND_MAX + 1.0);
    
    int cnt = 0;

    //罰則点が０点になるか、試行回数が指定された値に達するまで色を書き換える
    while(Vio(node,graph) != 0 && cnt < hc){
        ofs << Vio(node, graph) << endl;
        vector<int> vioCnt = EachVio(node, graph);
        int tmp = (double)N * rand() / (RAND_MAX + 1.0);

        //罰則点がある領域からランダムで１つの領域を選択する
        while(vioCnt[tmp] == 0)
            tmp = (double)N * rand() / (RAND_MAX + 1.0);
        
        //各色に書き換えた時の罰則点を確保する
        for (int i = 0; i < 3;i++){
            node[tmp] = i;
            check[i] = Vio(node, graph);
        }

        //各色に書き換えた際の罰則点から最小になるような値を求める。
        int min = minVec(check);
        int ind = (double)3 * rand() / (RAND_MAX + 1.0);
        
        //罰則点が最小値になる色を（複数ある場合はランダムで）選択する。
        while(check[ind] > min)
            ind = (double)3 * rand() / (RAND_MAX + 1.0);
        node[tmp] = ind;
        cnt++;
        cout << Vio(node, graph) << endl;
    }
    if (Vio(node, graph) == 0)
        ofs << 0 << endl;
    cout << "count:" << cnt <<" violation:" << Vio(node, graph) << endl;
    file1 << ",";
    for (int i = 0; i < N;i++)
        file1 << node[i] << ",";
    file1 << endl;
    for (int i = 0; i < N;i++){
        file1 << node[i] << ",";
        for (int j = 0; j < N;j++){
            if(j < i+1){
                file1 << ",";
            }else{
                file1 << graph[i][j] << ",";
            }
        }
        file1 << endl;
    }
        return 0;
}
