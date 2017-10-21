//
//  Dijkstra.hpp
//  Dijkstra
//
//  Created by forever on 11/10/2017.
//  Copyright © 2017 MAC. All rights reserved.
//

#ifndef Dijkstra_hpp
#define Dijkstra_hpp

#include <stdio.h>

#pragma once
//#pragma once是一个比较常用的C/C++杂注，
//只要在头文件的最开始加入这条杂注，
//就能够保证头文件只被编译一次。

#include<iostream>
#include<string>
using namespace std;

/*
 本程序是使用Dijkstra算法实现求解最短路径的问题
 采用的邻接矩阵来存储图
 */
//记录起点到每个顶点的最短路径的信息
struct Dis {
    string path;
    int value;
    bool visit;
    Dis() {
        visit = false;
        value = 0;
        path = "";
    }
};

class Graph_DG {
private:
    int vexnum;     //图的顶点个数
    int edge;       //图的边数
    int **arc;      //邻接矩阵
    Dis * dis;      //记录各个顶点最短路径的信息
    string nodelink = "-->";
public:
    
    //构造函数
    Graph_DG(int vexnum, int edge);
    //析构函数
    ~Graph_DG();
    // 判断我们每次输入的的边的信息是否合法
    //顶点从1开始编号
    bool check_edge_value(int start, int end, int weight);
    //创建图
    void createGraph();
    void createGraph(int start, int end, int len);
    //打印邻接矩阵
    void print();
    //初始化结构体
    void InitDisStruct();
    //求最短路径
    void Dijkstra(int begin);
    //打印最短路径
    void print_path(int);
    //保存路径
    void Save_path(string **tab,int begin);
};

#endif /* Dijkstra_hpp */
