//
//  main.cpp
//  Dijkstra
//
//  Created by forever on 11/10/2017.
//  Copyright © 2017 MAC. All rights reserved.
//

#include <iostream>
#include"Dijkstra.hpp"
#include<fstream>
#include<sstream>

string ** DijPath;
int ** MAP;
int *NodePath = NULL;
int vexnum;
int edge;
int fflag = 0;
string nlink = "-->";

//检验输入边数和顶点数的值是否有效，可以自己推算为啥：
//顶点数和边数的关系是：((Vexnum*(Vexnum - 1)) / 2) < edge
bool check(int Vexnum, int edge) {
    if (Vexnum <= 0 || edge <= 0 || ((Vexnum*(Vexnum - 1)) / 2) < edge)
        return false;
    return true;
}


void ReadMap(){
    char buf[1024];                //临时保存读取出来的文件内容
    string message;
    ifstream infile;
    infile.open("AGVMap.txt");
    if(infile.is_open())          //文件打开成功,说明曾经写入过东西
    {
        unsigned long d1,d2;
        char * end;
        int line = 0;
        
        //读取第一行节点数和边数
        if (infile.good() && !infile.eof()) {
            memset(buf, 0, 1024);
            infile.getline(buf, 1024);
            message = buf;
            
            d1 = message.find_first_of(",");
            vexnum =  static_cast<int>(strtol(message.substr(0, d1).c_str(),&end,10));
            edge = static_cast<int>(strtol(message.substr(d1+1).c_str(), &end, 10));
            if (!check(vexnum, edge)) {
                cout << "输入的数值不合法，请检查配置文件AGVMap.txt确认规格、数值以及输入法规范 " << endl;
                return;
            }
            cout <<"顶点数："<< vexnum <<";边数："<< edge <<endl;
            MAP = new int*[edge];
        }
        
        while(infile.good() && !infile.eof())
        {
            memset(buf,0,1024);
            infile.getline(buf,1204);
            message = buf;
            
            d1 = message.find_first_of(",");
            d2 = message.find_last_of(",");
            
            MAP[line] = new int[3];                                             //init col
            MAP[line][0] =  static_cast<int>(strtol(message.substr(0, d1).c_str(),&end,10));
            MAP[line][1] = static_cast<int>(strtol(message.substr(d1 + 1, d2 - d1 - 1 ).c_str(),&end,10));
            MAP[line][2] = static_cast<int>(strtol(message.substr(d2 + 1 ).c_str(),&end,10));
//            cout<< MAP[line][0]<< MAP[line][1]<< MAP[line][2]<<endl;
            ++line;
        }
        infile.close();
    }
}

int GetPathNum(string p){
    int count = 0;
    size_t n = p.find(nlink,0);
    while (n != p.npos) {
        count++;
        n = p.find(nlink,n+3);
    }
    return count + 1;
}

void ParsePath(int begin, int last){
    delete[] NodePath;
    string p = DijPath[begin - 1][last - 1];
    cout << "选择的路径为：" << p << endl;
    int num = GetPathNum(p);
    if (num == 0) {
        cout << "路径为空或路径格式错误！" << endl;
        return;
    }
    NodePath = new int[num];
    
    char * end;
    size_t n1 = p.find(nlink,0);
    NodePath[0] = static_cast<int>(strtol(p.substr(0, n1).c_str(),&end,10));
    size_t n2 = 0;
    for (int i = 1; i != num - 1; i++) {
        n2 = p.find(nlink,n1+3);
        NodePath[i] = static_cast<int>(strtol(p.substr(n1+3, n2).c_str(),&end,10));
        n1 = n2;
    }
    NodePath[num - 1] = static_cast<int>(strtol(p.substr(n2+3).c_str(),&end,10));
}

void Init(){
    DijPath = new string*[vexnum];
    for (int i = 0; i < vexnum; i++) {
        DijPath[i] = new string[vexnum];
        for (int k = 0; k < vexnum; k++) {
            //邻接矩阵初始化为无穷大
            DijPath[i][k] = "";
        }
    }
    NodePath = new int[vexnum];
}

int * aaa;

void test(int * x){
    x[0] = 2;
    x[1] = 2;
}


int main() {

    //get the vexnum and edge
    if (fflag) {
        cout << "输入图的顶点个数和边的条数：" << endl;
        cin >> vexnum >> edge;
        while (!check(vexnum, edge)) {
            cout << "输入的数值不合法，请重新输入" << endl;
            cin >> vexnum >> edge;
        }
    }
    else
        ReadMap();
    
    Init();                             //初始化DijPath路径存储tab
    Graph_DG graph(vexnum, edge);       //创建dijkstra
    
    //get the detail map path
    if (fflag)
        graph.createGraph();
    else{
        for (int i = 0; i != edge; i++) {
            graph.createGraph(MAP[i][0], MAP[i][1], MAP[i][2]);
        }
        //delete temp Map for node and edge
        for (int i = 0; i<edge; i++) {
            delete MAP[i];
        }
        delete MAP;
    }

    
    //print adjacent matrix
    graph.print();
    //get and save the path to DijPath
    for (int i = 1; i<=vexnum; i++) {
        graph.Dijkstra(i);
        graph.Save_path(DijPath, i);
    }
    ParsePath(1,10);
    return 0;
}
