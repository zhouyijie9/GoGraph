#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <map>
#include <queue>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
using namespace std;

bool cmp(int a, int b, const map<int, int>& m) {
    return m.at(a) < m.at(b);
}

struct ordered_vertex{
    int id;
    double score;
    int type;  //入邻居：1  出邻居：2
};

struct Vertex{
    int id;
    double score;
};

bool cmp_Vertex(Vertex a, Vertex b){
    return a.score<b.score;
}


bool cmp_neighbor(ordered_vertex a , ordered_vertex b){
    return a.score<b.score;
}


//获取保证最大值的插入位置
void getMaxScore(std::vector<ordered_vertex> neighbor_vertex, double& Maxscore){

    if(neighbor_vertex.size()==0){
        Maxscore=-100; //感觉可以随便取的
        return;
    }

    double max_score = 0;
    int max_index = -1;

    double score = 0;

    for(int i=0 ; i< neighbor_vertex.size() ; i++){
        //入邻居：1  出邻居：2
        //入+ 出-
        if(neighbor_vertex[i].type==1){
            score++;
            if(score > max_score){
                max_score = score;
                max_index = i;
            }
        }
        else if(neighbor_vertex[i].type == 2){
            score--;
            if(score > max_score){
                max_score = score;
                max_index = i;
            }
        }


    }



    if(max_index==-1){
        //说明在最前方
        Maxscore = neighbor_vertex[0].score - 5;//步长设置为5
    }

    else if(max_index == neighbor_vertex.size()-1){
        //说明在最后方
        Maxscore = neighbor_vertex[neighbor_vertex.size()-1].score + 5;//步长设置为5

    }
    else{
        Maxscore = (neighbor_vertex[max_index].score + neighbor_vertex[max_index+1].score)/2;
    }

}




int main(int argc, char** argv) {
    // read file
    int MAX_SCORE = -2147483648;
    double load_time = clock();
    string input_filename = argv[1];
    ifstream infile(input_filename);
    vector<pair<int, int> > edges;
    int max_vid = -1;
    int u, v;
    while (infile >> u >> v) {
        edges.emplace_back(make_pair(u, v));
        max_vid = max_vid < u ? u : max_vid;
        max_vid = max_vid < v ? v : max_vid;
    }
    infile.close();

    string input_filename_1 = argv[2];
    ifstream infile_1(input_filename_1);
    vector<int> isolated_points; //存放孤立点

    while (infile_1 >> v) {
        isolated_points.emplace_back(v);
    }
    infile_1.close();

    string input_filename_2 = argv[3];
    ifstream infile_2(input_filename_2);

    int v_id;
    double v_score;

    vector<Vertex> all_vertex;

    while (infile_2 >> v_id >> v_score){
        all_vertex.push_back({v_id , v_score});
        //目前保存所有中间点 后面会保存所有点
    }

    infile_2.close();



    // build graph
    int n = max_vid + 1; // 顶点数量设置为: 最大id+1
    cout << "node_num = " << n << endl;
    vector<vector<int>> out_neib_vec(n);
    vector<vector<int>> in_neib_vec(n);
    for (auto edge : edges) {
        u = edge.first;
        v = edge.second;
        out_neib_vec[u].emplace_back(v);
        in_neib_vec[v].emplace_back(u);
    }



    map<int,int> index_of_all_Vertex;

    //记录all_Vertex内部的索引
    //前面是id 后面是index

    //int bigdegreeStart = all_vertex.size();//从该处开始写bigdegree

    for(int i=0;i<isolated_points.size();i++){
        //初始化bigdegreeVertex_and_score
        all_vertex.push_back({isolated_points[i] , -9999999});
        //index_of_bigdegreeVertex[bigdegree_points[i]] = bigdegreeStart+i;
    }

    for(int i=0;i<all_vertex.size();i++){
        index_of_all_Vertex[all_vertex[i].id] = i;
    }



    for(int i=0;i<isolated_points.size();i++){

        vector<ordered_vertex>neighbor;

        //获取出邻居
        for(auto out_vertex: out_neib_vec[isolated_points[i]]){
            if(all_vertex[index_of_all_Vertex[out_vertex]].score!=-9999999){
                //如果被更新了值 就加入
                neighbor.push_back({out_vertex,all_vertex[index_of_all_Vertex[out_vertex]].score,2});
                //neighbor.push_back({bigdegree_points[i],all_vertex[index_of_all_Vertex[out_vertex]],2});

            }
        }
        //获取入邻居
        for(auto in_vertex : in_neib_vec[isolated_points[i]]){
            if(all_vertex[index_of_all_Vertex[in_vertex]].score!=-9999999){
                //如果被更新了值 就加入
                neighbor.push_back({in_vertex,all_vertex[index_of_all_Vertex[in_vertex]].score, 1});
                //neighbor.push_back({bigdegree_points[i],bigdegreeVertex_and_score[index_of_bigdegreeVertex_and_score[in_vertex]].score,1});

            }

        }

        sort(neighbor.begin() , neighbor.end() , cmp_neighbor);
        //对邻居顶点的score进行排序


        double Maxscore = 0;
        getMaxScore(neighbor,Maxscore);

        all_vertex[index_of_all_Vertex[isolated_points[i]]].score = Maxscore;

        //bigdegreeVertex_and_score[index_of_bigdegreeVertex_and_score[bigdegree_points[i]]].score = Maxscore;

    }


    std::sort(all_vertex.begin(), all_vertex.end(), cmp_Vertex);





//输出文件
    //std::ofstream outfile(input_filename+".gographcluster_v");
    std::ofstream outfile(input_filename + ".PartGoGraph_v");

    for(int i=0;i<all_vertex.size();i++){
        outfile << all_vertex[i].id << '\n';

    }


    outfile.close();

    // cmd="${root_path}/ProcessIsolatedPoint ${efile} 原始边文件 .iso 孤立点文件 _v 剩余点文件"
    return 0;
}
