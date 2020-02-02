#include <iostream>
#include <vector>
#include <stdio.h>
#include <utility>
using namespace std;

int main(){
    //generate Random Number
    srand(101);
    bool suc = false;
    int n = 120;
    int n_div = n / 3;
    //Tightly coupled problem
    int m = n * (n - 1) / 4;
    //Loose coupling problem
    //int m = 3*n;
    vector<vector<int>> graph(n, vector<int>(n, 0));
    vector<int> ones(n*n_div, 0);
    while(!suc){
        suc = true;

        //Divide into lattices
        for (int i = 0; i < m;i++)
            ones[i] = 1;

        
            for (int i = 0; i < 10000;i++)
            swap(ones[(double)n * n_div * rand() / (RAND_MAX + 1.0)], ones[(double)n * n_div * rand() / (RAND_MAX + 1.0)]);

        int ind = 0;
        for (int i = 0;i < 2*n_div;i++){
            if(i < n_div){
                for (int j = 0; j < n_div;j++){
                    graph[j][n_div+i] = ones[ind];
                    ind++;
                }
            }else{
                for (int j = 0; j < 2 * n_div;j++){
                    graph[j][n_div+i] = ones[ind];
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
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
