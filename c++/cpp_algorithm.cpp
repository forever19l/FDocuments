#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>
using namespace std;

typedef struct Node
{
    int data;
    struct Node* next;
}Node;
typedef struct Node *LinkList;

//注意：凡是更改链表数据的地方都要用指针。
void CreateListHead(LinkList *L,int n)//创建链表，头插法
{
    LinkList p;
    int i;
    srand(time(NULL));
    *L=(LinkList)malloc(sizeof(Node));
    (*L)->data=n;
    (*L)->next=NULL;
    for (i=0;i<n;i++)
    {
        p=(LinkList)malloc(sizeof(Node));
        p->data=rand()%100+1;
        p->next=(*L)->next;
        (*L)->next=p;
    }
}


void CreateListTail(LinkList *L,int n)//创建链表，尾插法，利用一个动态指针。
{
    LinkList p,q;
    int i;
    srand(time(NULL));
    *L=(LinkList)malloc(sizeof(Node));
    (*L)->data=n;
    p=*L;
    for(i=0;i<n;i++)
    {
        q=(LinkList)malloc(sizeof(Node));
        q->data=rand()%100+1;
        p->next=q;
        p=q;
    }

    p->next=NULL;//记住喽要置空
}

void ClearList(LinkList *L)//删除链表
{
    LinkList p,q;
    p=(*L)->next;
    while (p)
    {
        q=p->next;
        free(p);
        p=q;
    }
    (*L)->next=NULL;
}

bool GetElem(LinkList L,int i, int *e)  //从链表中查找一个元素，利用e返回
{
    int j;
    LinkList p;
    p=L->next;
    j=1;
    while (p&&j<i)
    {
        p=p->next;
        ++j;
    }

    if (!p||j>i)
    {
        return false;
    }
    *e=p->data;
    return true;
}

bool ListInsert(LinkList *L,int i,int e)//在位置i处插入元素
{
    int j;
    LinkList p,q;
    p=*L;
    j=1;
    while(p&&j<i)
    {
        p=p->next;
        ++j;
    }

    if (!p||j>i)
    {
        return false;
    }

    q=(LinkList)malloc(sizeof(Node));
    q->data=e;
    q->next=p->next;
    p->next=q;
    return true;
}

bool ListDelete(LinkList *L,int i)//删除一个元素
{
    int j=1;
    LinkList p,q;
    p=(*L);
    while(p&&j<i)
    {
        p=p->next;
        ++j;
    }

    if (!(p->next))
    {
        return false;
    }

    q=p->next;
    p->next=q->next;
    free(q);
    return true;
}


typedef struct StackNode
{
    int data;
    StackNode* next;
}StackNode,*LinkStackPtr;

typedef struct LinkStack
{
    LinkStackPtr top;
    int count;
}LinkStack;

void InitStack(LinkStack* S)
{
 //     LinkStackPtr s=(LinkStackPtr)malloc(sizeof(StackNode));
 //     s->data=0;
 //     s->next=NULL;
 //     S->top=s;
 //     S->count=0;
    S->top=NULL;
    S->count=0;
}

void ClearStack(LinkStack *S)
{
    LinkStackPtr p,q;
    p = S->top;
    while(p){
        q = p;
        p = p->next;
        free(q);
    }
    S->count = 0;
}

bool GetStackTop(LinkStack S, int *e)  //获取栈顶元素e
{
    if (S.top != NULL)
    {
        *e = S.top->data;
        return true;
    }
    return false;
}

bool Push(LinkStack* S,int e)  //插入e为新栈顶元素
{
    LinkStackPtr ls=(LinkStackPtr)malloc(sizeof(StackNode));
    ls->data=e;
    ls->next=S->top;
    S->top=ls;
    S->count++;
    return true;
}

bool Pop(LinkStack* S,int *e)  //栈非空，则删除S栈顶元素
{
    LinkStackPtr p;
    if (S->count==0)
    {
        return false;
    }
    *e=S->top->data;
    p=S->top;
    S->top=S->top->next;
    free(p);
    S->count--;
    return true;
}


//迭代  斐波那契
int Fbi(int i)
{
    if (i<2)
    {
        return i==0 ? 0:1;
    }
    return Fbi(i-1)+Fbi(i-2);
}

//循环队列
typedef int QElemType;  //前端删除，后端插入
typedef struct{
    QElemType data[100];
    int front;
    int rear;
}SqQueue;

BOOL InitQueue(SqQueue* Q)
{
    Q->rear=0;
    Q->front=0;
    return true;
}

int QueueLength(SqQueue Q)
{
    return (Q.rear-Q.front+100)%100;
}

BOOL EnQueue(SqQueue* Q,QElemType e)
{
    if ((Q->rear+1)%100==Q->front)//队列满判断
    {
        return false;
    }
    Q->data[Q->rear]=e;
    Q->rear=(Q->rear+1)%100;//移到下一位置
    return true;
}

BOOL DeQueue(SqQueue* Q,QElemType *e)
{
    if (Q->front==Q->rear)//队列空判断
    {
        return false;
    }
    *e=Q->data[Q->front];
    Q->front=(Q->front+1)%100;
    return true;
}


// KMP模式匹配算法
void get_next(string T,int * next)//返回字串T的next数组
{
    int i,j;
    i=1;j=0;
    next[1]=0;
    while (i<T[0])
    {
        if (j==0||T[i]==T[j])  //T[i]表示后缀的单个字符，T[j]表示前缀的单个字符
        {
            ++i;
            ++j;
            next[i]=j;
        }

        else
        {
            j=next[j];//若字符不同则j值回溯
        }
    }
}

int Index_KMP(string S,string T,int pos)//返回子串在pos个字符之后的位置，不存在返回0
{
    int i=pos;//i用于主串S当前位置下标值，若pos不为1，则从pos位置开始匹配
    int j=1;//j用于子串T中当前位置下标值
    int next[255];//定义以next数组
    get_next(T,next);//对串T做分析，得到next数组
    while (i<=S[0]&&j<=T[0])//若i小于S的长度且j小于T的长度时，循环继续
    {
        if (j==0||S[i]==T[j])//两字母相等则继续，与朴素法增加了j=0的判断
        {
            ++i;
            ++j;
        }
        else
            j=next[j];//j退回合适的位置，i值不变
    }

    if (j>T[0])
    {
        return i-T[0];
    }
    else
        return 0;
}

//二叉树二叉链表节点结构定义
typedef struct BiTNode
{
    int data;
    int bf;
    struct BiTNode *lchild,*rchild;//左右孩子指针
}BiTNode,*BitTree;

void PreOrderTraverse(BitTree T)//前序遍历递归算法
{
    if (T==NULL)
    {
        return;
    }

    printf("%d",T->data);//可以更改为其他对节点操作
    PreOrderTraverse(T->lchild);
    PreOrderTraverse(T->rchild);
}

void InOrderTraverse(BitTree T)//中序遍历
{
    if (T==NULL)
    {
        return;
    }

    InOrderTraverse(T->lchild);
    printf("%d",T->data);
    InOrderTraverse(T->rchild);
}

void PostOrderTraverse(BitTree T)//后序遍历
{
    if (T=NULL)
    {
        return;
    }

    PostOrderTraverse(T->lchild);
    PostOrderTraverse(T->rchild);
    printf("%d",T->data);
}

void CreateBitTree(BitTree* T)//生成一个二叉树
{
    int num;
    scanf("%d",&num);
    if (99==num)  //自己定义
    {
        *T=NULL;
    }
    else
    {
        *T=(BitTree)malloc(sizeof(BiTNode));
        if (!*T)
        {
            (*T)->data=num;
            CreateBitTree(&(*T)->lchild);
            CreateBitTree(&(*T)->rchild);
        }
    }
}

 //二叉排序树查找
BOOL SearchBST(BitTree T,int key,BitTree f,BitTree*p)//f为T的双亲，初始调用值为NULL//指针p指向该数据结点
{
    if (!T)
    {
        *p=f;
        return false;
    }

    else if (key==T->data)
    {
        *p=T;
        return true;
    }
    else if (key<T->data)
    {
        return SearchBST(T->lchild,key,T,p);
    }
    else
        return SearchBST(T->rchild,key,T,p);
}

BOOL InsertBST(BitTree *T,int key)
{
    BitTree p,s;
    if (!SearchBST(*T,key,NULL,&p))
    {
        s=(BitTree)malloc(sizeof(BiTNode));
        s->data=key;
        s->lchild=s->rchild=NULL;
        if (!p)
        {
            *T=s;
        }
        else if (key<p->data)
        {
            p->lchild;
        }
        else
            p->rchild=s;
        return true;
    }
    return false;
}

BOOL DeleteBST(BitTree* T,int key)//二叉排序树删除元素
{
    if (!*T)
    {
        return false;
    }

    else
    {
        if (key==(*T)->data)
        {
            return Delete(T);//删除元素
        }
        else if (key<(*T)->data)
        {
            return DeleteBST(&(*T)->lchild,key);
        }
        else
            return DeleteBST(&(*T)->rchild,key);
    }
}

BOOL Delete(BitTree*p)
{
    BitTree q,s;
    if ((*p)->rchild==NULL)//右子树为空则只重连接左子树
    {
        q=*p;*p=(*p)->lchild;free(q);
    }
    else if ((*p)->lchild==NULL)//重连右子树
    {
        q=*p;*p=(*p)->rchild;free(q);
    }
    else//左右子树都不空
    {
        q=*p;s=(*p)->lchild;//在左子树查找替代元素
        while (s->rchild)//右子树不空，就对q赋值
        {
            q=s;s=s->rchild;
        }
        (*p)->data=s->data;
        if (q!=*p)
        {
            q->rchild=s->lchild;
        }
        else
            q->lchild=s->lchild;
        free(s);
    }
    return true;
}

void R_Rotate(BitTree* P)//二叉排序树右旋处理
{
    BitTree L;
    L=(*P)->lchild;//L指向P的左子树根节点
    (*P)->lchild=L->rchild;//L的右子树挂接为P的左子树
    L->rchild=(*P);//P成为L的右子树
    *P=L;//重新设置新结点
}

void L_Rotate(BitTree*P)//二叉排序树左旋处理
{
    BitTree R;
    R=(*P)->rchild;
    (*P)->rchild=R->lchild;
    R->lchild=(*P);
    *P=R;
}


//平衡二叉树 AVL
#define LH +1
#define EH 0
#define RH -1
void LeftBalance(BitTree*T)//对以T所指结点为根的二叉排序树做左平衡旋转处理
{
    BitTree L,Lr;
    L=(*T)->lchild;
    switch (L->bf)
    {
    case LH:
        (*T)->bf=L->bf=EH;//新节点在T的左孩子的左子树上，简单右旋处理
        R_Rotate(T);
        break;
    case RH:        //新结点在T的左孩子的右子树上，要做双旋处理
        Lr=L->rchild;
        switch(Lr->bf)
        {
        case LH:(*T)->bf=RH;
                L->bf=EH;
                break;
        case EH:(*T)->bf=L->bf=EH;
            break;
        case RH:(*T)->bf=EH;
            L->bf=LH;
            break;
        }
        Lr->bf=EH;
        L_Rotate(&(*T)->lchild);//对T的左子树做左旋平衡处理
        R_Rotate(T);//对T做右旋平衡处理
        break;
    }
}


void RightBalance(BitTree*T)
{
    BitTree R,Rr;
    R=(*T)->rchild;
    switch (R->bf)
    {
    case LH:
        Rr=R->lchild;
        switch(Rr->bf)
        {
        case LH:(*T)->bf=RH;
            R->bf=EH;
            break;
        case EH:(*T)->bf=R->bf=EH;
            break;
        case RH:(*T)->bf=EH;
            R->bf=LH;
            break;
        }
        Rr->bf=EH;
        R_Rotate(&(*T)->rchild);
        L_Rotate(T);

        break;
    case RH:
        (*T)->bf=R->bf=EH;
        L_Rotate(T);
        break;
    }
}


BOOL InsertAVL(BitTree*T,int e,BOOL* taller)
{
    if (!(*T))
    {
        *T=(BitTree)malloc(sizeof(BiTNode));
        (*T)->data=e;
        (*T)->lchild=(*T)->rchild=NULL;
        (*T)->bf=EH;
        *taller=true;
    }
    else
    {
        if (e==(*T)->data)
        {
            *taller=false;
            return false;
        }
        if(e<(*T)->data)//应继续在T的左子树中进行搜索
        {
            if (!InsertAVL(&(*T)->lchild,e,taller))//未插入
            {
                return false;
            }
            if (*taller)//已经插入到T的左子树中且左子树长高
            {
                switch((*T)->bf)
                {
                case LH:
                    LeftBalance(T);
                    *taller=false;
                    break;
                case EH:
                    (*T)->bf=LH;
                    *taller=true;
                    break;
                case RH:
                    (*T)->bf=EH;
                    *taller=false;
                    break;
                }
            }
        }
        else
        {
            if (!InsertAVL(&(*T)->rchild,e,taller))//应该在T的右子树中查找
            {
                return false;
            }
            if (*taller)//已经插入到T的右子树且右子树长高
            {
                switch((*T)->bf)
                {
                case LH:
                    (*T)->bf=EH;
                    *taller=false;
                    break;
                case EH:
                    (*T)->bf=RH;
                    *taller=true;
                    break;
                case RH:
                    RightBalance(T);
                    *taller=false;
                    break;

                }
            }
        }
    }
    return true;
}

//平衡二叉树 AVL//有序查找
int Binary_Search(int *a,int n,int key)//折半（二分）查找法  用于有序表中
{
    int low,high,mid;
    low=1;
    high=n;

    while (low<=high)
    {
        mid=(low+high)/2;//折半
        if (key<a[mid])
        {
            high=mid-1;
        }
        else if (key>a[mid])
        {
            low=mid+1;
        }
        else
            return mid;

    }
    return 0;
}

int Inerpolation_Search(int *a,int n,int key)//插值查找，二分法的一种改进
{
    int low,high,mid;                       //试用于数据分布比较均匀的数列，不适合数据分布极端不均的数据
    low=1;
    high=n;
    while(low<=high)
    {
        mid=low+(key-a[low])/(a[high]-a[low])*(high-low);
        if (key<a[mid])
        {
            high=mid-1;
        }

        else if (key>a[mid])
        {
            low=mid+1;
        }
        else
            return mid;
    }
    return 0;
}


int Fibonacci_Search(int * a ,int n,int key)//斐波那契查找，不是很明白 //大数据时用会提高效率 F为斐波那契数组
{
    int low,high,mid,i,k;
    low=1;
    high=n;
    k=0;
    while(n>F[k]-1)
        k++;
    for (i=n;i<F[k]-1;i++)
    {
        a[i]=a[n];
    }
    while (low<=high)
    {
        mid=low+F[k-1]-1;
        if (key<a[mid])
        {
            high=mid-1;
            k=k-1;
        }
        else  if (key>a[mid])
        {
            low=mid+1;
            k=k-2;
        }
        else
        {
            if (mid<=n)
            {
                return mid;
            }
            else
                return n;
        }
    }
    return 0;
}


// 有序查找//以下为排序算法
#define MAXSIZE 10
typedef struct
{
    int r[MAXSIZE+1];//存储排序数组，r[0]用作哨兵
    int length;     //记录表长
}SqList;

void swap(SqList*L,int i,int j)//交换函数，频繁用到
{
    int temp=L->r[i];
    L->r[i]=L->r[j];
    L->r[j]=temp;
}

//冒泡排序O(n^2)，将相邻数据进行比较，然后将较大的移到后面(由小到大)。
void BubbleSort(SqList *L)
{
    int i=1;
    int j;
    bool flag=true;//标记
    while(flag)             //如果flag为true，说明有数据交换，为false说明无数据交换，可停止循环了
    {
        flag=false;
        for (j=L->length-1; j>=i; j--)
        {
            if (L->r[j] > L->r[j+1])
            {
                swap(L,j,j+1);
                flag=true;  //有数据交换，则值为真。
            }
        }
        i++;                //最小值已到第一位，不用再比较
    }
}

//选择排序O(n^2)，以第一个为基准值，向右扫描，找到最小的与之交换。然后基准值右移一位循环扫描交换，直至最后一个元素。
void SelectSort(SqList*L)
{
    int i,j,min;
    for (i=1;i<L->length;i++)
    {
        min=i;
        for (j=i+1;j<=L->length;j++)
        {
            if (L->r[min]>L->r[j])
            {
                min=j;
            }
        }
        // if (i!=min)
        // {
            swap(L,i,min);
        // }
    }
}

//直插排序O(n^2),将一个记录插入到已经排好序的有序表x中，得到一个新的、记录数增1的有序表
void InsertSort(SqList *L, int x)
{
    int i,j;
    for (i = x; i <= L->length; i++)    //前x是有序的（对于本数据结构，0位置无效）
    {
        if (L->r[i]<L->r[i-1])
        {
            L->r[0]=L->r[i];            //temp
            for (j=i-1; L->r[j]>L->r[0] && j >= 0; j--)
                L->r[j+1]=L->r[j];      //元素比temp大，后移一位
            L->r[j+1]=L->r[0];          //找到比temp小的值，跳出循环在其位置后插入元素
        }
    }
}

//希尔排序,先以步长分组，再进行直插排序
void ShellSort(SqList*L)
{
    int i,j;
    int gap = L->length;
    do
    {
        gap = gap/2;                        //步长，wiki更多步长选择
        for (i = gap+1; i<=L->length; i++)  //+1适应本数据结构
        {
            if (L->r[i] < L->r[i-gap])
            {
                L->r[0] = L->r[i];          //temp
                for (j = i-gap; j>0 && L->r[0] < L->r[j]; j -= gap)
                {
                    L->r[j+gap]=L->r[j];    //记录后移，查找插入位置
                }
                L->r[j+gap]=L->r[0];        //插入
            }
        }
    } while (gap>1);
}

//堆排序O(n*log2n) ，适用于完全二叉树（除了最深的一层，其他层次都是满节点）
//i结点的父结点下标就为(i – 1) / 2。它的左右子结点下标分别为2i + 1和2i + 2。如第0个结点左右子结点下标分别为1和2。
void HeapSort(SqList *L)
{
    int i;
    for (i=L->length/2;i>0;i--)//将原始序列转为堆序列，为什么是length/2？因为根据完全二叉树的性质，每个双亲访问其子孩子可完全遍历整棵树
    {
        MaxHeapAdjust(L,i,L->length);
    }

    for (i=L->length;i>1;i--)//将堆顶元素与最末元素交换，继续调整序列为堆序列
    {
        swap(L,1,i);
        MaxHeapAdjust(L,1,i-1);
    }
}//最终L为有序序列

void MaxHeapAdjust(SqList *L,int st,int n)   //大顶堆调整,st开始节点，n总结点数
{
    int j = 2*st;
    int temp = L->r[st];
    // while(j<=n)
    for (j; j <= n; j*=2)
    {
        if (j<n && L->r[j] < L->r[j+1])
        {
            ++j;
        }
        if (temp >= L->r[j])
        {
            break;
        }
        L->r[st] = L->r[j];
        st = j;
    }
    L->r[st] = temp;//插入
}

//归并排序O(n*log2n) ，占用内存较多
void MergeSort(SqList*L)
{
    MSort(L->r,L->r,1,L->length);
}

void MSort(int SR[],int TR1[],int s,int t)  //s-start, t-lenth
{
    int m;
    int TR2[MAXSIZE+1];
    if (s==t)
    {
        TR1[s]=SR[s];
    }
    else
    {
        m=(s+t)/2;              //将SR[s……t]平分为SR[s……m]和ST[m+1……t]
        MSort(SR,TR2,s,m);      //递归将SR[s……m]归并为有序的TR2[s……m]
        MSort(SR,TR2,m+1,t);    //递归将SR[m+1……t]归并为有序TR2[m+1……t]
        Merge(TR2,TR1,s,m,t);   //将TR2[s……m]和TR2[m+1……t]归并到TR1[s……t]
    }
}

void Merge(int SR[],int TR[],int i,int m,int n)//将有序的SR[i……m]和SR[m+1……t]归并为有序的TR[i……n]
{
    int j=m+1 ,k ;
    while(i<=m && j<=n)//将SR中记录由小到大归并入TR
    {
        if (SR[i] < SR[j])
            TR[k++] = SR[i++];
        else
            TR[k++] = SR[j++];
    }

    while (i <= m)
        TR[k++] = SR[i++];

    while (j <= n)
        TR[k++] = SR[j++];
}

void MemeryArray(int a[], int n, int b[], int m, int c[])  //将a,b合并到c; n=a.lenth, m=b.lenth
{  
    int i, j, k;  
    i = j = k = 0;  
    while (i < n && j < m)  
    {  
        if (a[i] < b[j])  
            c[k++] = a[i++];  
        else  
            c[k++] = b[j++];   
    }  
  
    while (i < n)  
        c[k++] = a[i++];  
  
    while (j < m)  
        c[k++] = b[j++];  
} 

void MergeSort2(SqList* L)//不用递归的归并排序
{
    int* TR = (int*)malloc(L->length*sizeof(int));//申请额外空间
    int k=1;
    while (k<L->length)
    {
        MergePass(L->r,TR,k,L->length);
        k=2*k;
        MergePass(TR,L->r,k,L->length);
        k=2*k;
    }
}

void MergePass(int SR[],int TR[],int s,int n)//将SR中相邻长度为s的子序列两两归并到TR[]
{
    int i=1;
    int j;
    while (i<=n-2*s+1)
    {
        Merge(SR,TR,i,i+s-1,i+2*s-1);
        i=i+2*s;
    }
    if (i<n-s+1)
    {
        Merge(SR,TR,i,i+s-1,n);

    }
    else
    {
        for (j=i;j<=n;j++)
            TR[j]=SR[j];
    }
}

//快速排序O(n*log2n) ,分治法。
void QuickSort(SqList*L)
{
    QSort(L,1,L->length);
}
void QSort(SqList*L,int low,int high)//对顺序表L中的子序列L->r[low...high]做快速排序
{
    if (low<high)
    {
        //        pivot=Partition(L,low,high);//将L->r[low...high]一分为二,算出枢轴值pivot
        int i = low, j = high;
        L->r[0] = L->r[i];                      //用子表的第一个记录做枢轴记录，存于0，其位置i为坑位
        while (i<j)
        {
            while (i<j && L->r[j] >= L->r[0])   //倒序查找较小值，并填入r[i]
                j--;                            //找到较小值，并记录下一个大坑位置
            if (i<j)
                L->r[i++] = L->r[j];            //将较小值填入小坑位r[i]中，i++右移一位减少一次判断
            while (i<j && L->r[i] <= L->r[0])   //正序查找较大值，并填入r[j]
                i++;                            //找到较大值，并记录下一个小坑位置
            if (i<j)
                L->r[j--] = L->r[i];            //将较大值填入大坑位r[j]中，j--左移一位减少一次判断
        }
        L->r[i] = L->r[0];                      //相对中枢值查找完毕，i=j将中枢值填入坑位
        QSort(L,low,i-1);                       //对低子表递归排序
        QSort(L,i+1,high);                      //对高子表递归排序
    }
}

//partition作用就是选取序列当中的一个关键字，正序逆序挖坑填入，使得它左边的值都比它小，右边都比它大，这样的关键字称为枢轴
int Partition(SqList*L,int low,int high)//优化不必要的交换
{
    L->r[0] = L->r[low];//将枢轴值备份到L->r[0]
    while (low<high)
    {
        while(low<high && L->r[high] >= L->r[low])
            high--;
        L->r[low]=L->r[high];//采用替换而不是交换的方式进行操作
        while(low<high && L->r[low] <= L->r[low])
            low++;
        L->r[high]=L->r[low];//采用替换而不是交换的方式进行操作
    }
    L->r[low]=L->r[0];//将枢轴值替换为L->r[low]
    return low;
}

//优化小数组时的排序方案，小数组排序直接插入式简单排序中性能最好的
//优化递归操作，用到了以上的优化内容，可以说是总的优化
#define MAX_LENGTH_INSERT_SORT 7
void FSort(SqList*L,int low,int high)
{
    if ((high-low)>MAX_LENGTH_INSERT_SORT)
    {
        while(low<high)
        {
            int pivot=Partition(L,low,high);
            FSort(L,low,pivot-1);
            low=pivot+1;
        }
    }
    else
        InsertSort(L);
}
//排序总结http://download.csdn.net/detail/morewindows/4560056

//图的存储结构
//邻接矩阵   用两个数组来表示图，一维数组存储顶点信息，二维数组存储边或弧的信息
typedef char VertexType;
typedef int EdgeType;
#define MAXVEX 100             //最大定点数
#define INFINITY 65535         //用65535表示无穷
typedef struct
{
    VertexType vexs[MAXVEX];//顶点表
    EdgeType arc[MAXVEX][MAXVEX];//邻接矩阵  NXN
    int numVertexes,numEdges;//图中当前的顶点数和边数
}MGraph;
void CreateMGraph(MGraph*G)  //无向图
{
    int i,j,k,w;
    printf("输入顶点数和边数\n");
    scanf("%d,%d",&G->numVertexes,&G->numEdges);
    for (i=0;i<G->numVertexes;i++)//建立顶点表
    {
        scanf(&G->vexs[i]);
    }

    for (i=0;i<G->numVertexes;i++)  //邻接矩阵初始化
    {
        for (j=0;j<G->numVertexes;j++)
        {
            G->arc[i][j]=INFINITY;
        }
    }

    for(k=0;k<G->numEdges;k++)
    {
        printf("输入边（Vi，Vj）的小标i，下标j和权w：\n")；//w非1时，就可认为是网
        scanf("%d,%d,%d",&i,&j,&w);
        G->arc[i][j]=w;
        G->arc[j][i]=G->arc[i][j];//无向图，矩阵对称，对于有向图此句可省去
    }
}


//邻接表
typedef struct EdgeNode//边表结点
{
    int adjvex;//邻接点域，存储该顶点对应的下标
    EdgeType weight;//存储权值，对于非网图不需要
    struct EdgeNode* next;//指向下一个邻接点
}EdgeNode;

typedef struct VertexNode//顶点表结点
{
    VertexNode data;//顶点域，存储顶点信息
    EdgeNode* firstedge;//边表头指针
}VertexNode,AdjList[MAXVEX];


 typedef struct
 {
    AdjList adjList;
    int numVertexes,numEdges;//图中当前顶点数和边数
 }GraphAdjList;

 void CreateALGraph(GraphAdjList* G)//无向图邻接表的创建
 {
    int i,j,k;
    EdgeNode* e;
    printf("输入顶点数和边数：\n");
    scanf("%d,%d",&G->numVertexes,&G->numEdges);

    for (i=0;i<G->numVertexes;i++)  //输入顶点信息
    {
        scanf(&G->adjList[i].data);
        G->adjList[i].firstedge=NULL;
    }

    for (k=0;k<G->numEdges;k++)
    {
        printf("输入边（Vi，Vj）上的顶点序号：\n");
        scanf("%d,%d",&i,&j);

        e=(EdgeNode*)malloc(sizeof(EdgeNode));  //i要指向j
        e->adjvex=j;
        e->next=G->adjList[i].firstedge;
        G->adjList[i].firstedge=e;

        e=(EdgeNode*)malloc(sizeof(EdgeNode));//j要指向i
        e->adjvex=i;
        e->next=G->adjList[j].firstedge;
        G->adjList[j].firstedge=e;
    }
 }


 //邻接矩阵的深度优先递归算法
 bool visisted[MAX];
 void DFS(MGraph G,int i)
 {
    int j;
    visisted[i]=true;
    printf("%c",G.vexs[i]);//打印顶点也可以做其他操作
    for (j=0;j<G.numVertexes;j++)
    {
        if (G.arc[i][j]==1&&!visisted[j])
        {
            DFS(G,j);//对未访问的邻接顶点递归调用
        }
    }

 }

 //邻接矩阵的深度遍历操作
 void DFSTraverse(MGraph G)
 {
    int i;
    for (i=0;i<G.numVertexes;i++)//初始所有顶点都是未访问过的状态
    {
        visisted[i]=false;
    }
    for (i=0;i<G.numVertexes;i++)//对未访问过的顶点调用DFS，若是连通图，只会执行一次
    {
        if (!visisted[i])
        {
            DFS(G,i);
        }
    }
 }


 //邻接表的深度优先递归算法
 void DFS(GraphAdjList GL,int i)
 {
    EdgeNode *p;
    visisted[i]=true;
    printf("%c",GL.adjList[i].data);
    p=GL.adjList[i].firstedge;
    while (p)
    {
        if (!visisted[p->adjvex])
        {
            DFS(GL,p->adjvex);
        }
        p=p->next;
    }
 }


 //邻接表的深度遍历操作
 void DFSTraverse(GraphAdjList GL)
 {
    int i;
    for (i=0;i<GL.numVertexes;i++)
    {
        visisted[i]=false;
    }
    for (i=0;i<GL.numVertexes;i++)
    {
        if (!visisted[i])
        {
            DFS(GL,i);
        }
    }
 }


 //邻接矩阵的广度遍历算法
 void BFSTraverse(MGraph G)
 {
    int i,j;
    SqQueue Q;
    for(i=0;i<G.numVertexes;i++)
        visisted[i]=false;
    InitQueue(&Q);  //初始化辅助用队列
    for (i=0;i<G.numVertexes;i++)//对每一个顶点做循环
    {
        if (!visisted[i])
        {
            visisted[i]=true;//设置当前点访问过
            printf("%c",G.vexs[i]);
            EnQueue(&Q,i);//将次顶点入队列
            while(!QueueLength(Q)>0)//若当前队列不为空
            {
                DeQueue(&Q,&i);//将队列中元素出队列赋给i
                for (j=0;j<G.numVertexes;j++)
                {
                    if (G.arc[i][j]==1&&!visisted[j])//判断其他顶点若与当前顶点存在边且未访问过
                    {
                        visisted[j]=true;
                        printf("%c",G.vexs[j]);
                        EnQueue(&Q,j);
                    }
                }
            }

        }
    }
 }


 //邻接表的广度遍历算法
 void BFSTraverse(GraphAdjList GL)
 {
    int i;
    EdgeNode* p;
    SqQueue Q;
    for (i=0;i<GL.numVertexes;i++)
    {
        visisted[i]=false;
    }
    InitQueue(&Q);
    for (i=0;i<GL.numVertexes;i++)
    {
        if (!visisted[i])
        {
            visisted[i]=true;
            printf("%c",GL.adjList[i].data);
            EnQueue(&Q,I);
            while(QueueLength(Q)>0)
            {
                DeQueue(&Q,&i);
                p=GL.adjList[i].firstedge;
                while(p)
                {
                    if (!visisted[p->adjvex])
                    {
                        visisted[p->adjvex]=true;
                        printf("%c",GL.adjList[p->adjvex].data);
                        EnQueue(&Q,p->adjvex);
                    }
                    p=p->next;
                }
            }
        }
    }

 }


 //最小生成树 普里姆算法（Prim） 查找遍历每个顶点的最短路径  邻接矩阵
 void MiniSpanTree_Prim(MGraph G)
 {
    int min,i,j,k;
    int adjvex[MAXVEX];//保存相关顶点下标

    int lowcost[MAXVEX];//保存相关顶点间边的权值
    lowcost[0]=0;//初始化第一个权值为0

    adjvex[0]=0;//初始化第一个顶点下标为0
    for (i=1;i<G.numVertexes;i++)//循环除下标为0外的全部顶点
    {
        lowcost[i]=G.arc[0][i];//将V0顶点与之有边的权值存入数组
        adjvex[i]=0;//初始化都为v0的下标
    }
    for (i=1;i<G.numVertexes;i++)
    {
        min=INFINITY;//初始化最小权值
        j=1;k=0;
        while(j<G.numVertexes)//循环全部顶点
        {
            if (lowcost[j]!=0&&lowcost[j]<min)///如果权值不为0且小于min，则让当前权值成为最小值
            {
                min=lowcost[j];
                k=j;//将当前下标最小值下标存入k
            }
            j++;
        }
        printf("(%d,%d)",adjvex[k],k);//打印当前顶点边中权值最小边
        lowcost[k]=0;//将当前顶点的权值设置为0，表示此顶点已经完成任务，不再参与比较
        for (j=1;j<G.numVertexes;j++)//循环所欲顶点
        {
            if (lowcost[j]!=0&&G.arc[k][j]<lowcost[j])
            {//若下标为k的顶点各边权值小于此前这些顶点未被加入生成树权值
                lowcost[j]=G.arc[k][j];//将较小的权值存入lowcost
                adjvex[j]=k;//将下标为k的顶点存入adjvex
            }
        }
    }
 }

 //Kruskal最小生成树
 typedef struct{
    int begin;
    int end;
    int weight;
 }Edge;

 #define MAXEDGE 65535
 void MiniSpanTree_Kruskal(MGraph G)
 {
    int i,n,m;
    Edge edges[MAXEDGE];
    int parent[MAXVEX];
    for (i=0;i<G.numVertexes;i++)
    {
        parent[i]=0;
    }
    for (i=0;i<G.numEdges;i++)
    {
        n=find(parent,edges[i].begin);
        m=find(parent,edges[i].begin);
        if (n!=m)
        {
            parent[n]=m;
            printf("(%d,%d) %d",edges[i].begin,edges[i].end,edges[i].weight);
        }
    }
 }

 int Find(int *parent,int f)
 {
    while(parent[f]>0)
        f=parent[f];
    return f;
 }

 //对于边数少的图用克鲁斯卡尔算法，对稠密图用普利姆算法

 //最短路径      迪杰斯特拉算法（Dijkstra）
 #define MAXVEX 9
 #define INFINITY 65535
 typedef int Pathmatrix[MAXVEX];
 typedef int ShortPathTable[MAXVEX];

 void ShortestPath_Dijkstra(MGraph G,int v0,Pathmatrix*P,ShortPathTable*D)
 {
    int v,w,k,min;
    int final[MAXVEX];//final[w]=1表示求得顶点VO和VW的最短路径
    for (v=0;v<G.numVertexes;v++)//初始化数据
    {
        final[v]=0;//全部顶点初始化为未知最短路径状态
        (*D)[v]=G.arc[v0][v];//将与v0有连线的顶点加上权值
        (*P)[v]=0;//初始化路径数组P为0

    }

    (*D)[v0]=0;//v0至v0路径为0
    final[v0]=1;//v0至v0不需要求路径
    //开始主循环，每次求得v0到某个v顶点的最短路径
    for (v=1;v<G.numVertexes;v++)
    {
        min=INFINITY;//初始化最小距离
        for (w=0;w<G.numVertexes;w++)//寻找离v0最近的顶点
        {
            if (!final[w]&&(*D)[w]<min)
            {
                k=w;
                min=(*D)[w];//w顶点离v0顶点更近
            }
        }
        final[k]=1;//将目前找到的最近的顶点置为1
        for (w=0;w<G.numVertexes;w++)//修正当前最短路径及距离
        {
            //如果经过v顶点的路径比现在这条路径的长度短的话，说明找到了更短的路径，修改D[w]和P[w]
            if (!final[w]&&(min+G.arc[k][w]<(*D)[w]))
            {
                (*D)[w]=min+G.arc[k][w];
                (*P)[w]=k;
            }
        }
    }
 }


 //佛洛依德算法，Floyd，求网图G中各顶点v到其余顶点w最短路径P[v][w]及带权长度D[v][w]
 typedef int Pathmatrix[MAXVEX][MAXVEX];
 typedef int ShortPathTable[MAXVEX][MAXVEX];

 void ShortestPath_Floyd(MGraph G,Pathmatrix *P,ShortPathTable*D)
 {
    int v,w,k;
    for (v=0;v<G.numVertexes;++v)//初始化D与P
    {
        for (w=0;w<G.numVertexes;++w)
        {
            (*D)[v][w]=G.arc[v][w];//D[v][w]值即为对应点的权值
            (*P)[v][w]=w;//初始化P
        }
    }

    for (k=0;k<G.numVertexes;++k)//k代表中转顶点的下标
    {
        for (v=0;v<G.numVertexes;++v)//v代表其实顶点
        {
            for (w=0;w<G.numVertexes;++w)//w代表结束顶点
            {
                if ((*D)[v][w]>(*D)[v][k]+(*D)[k][w])
                {//如果经过下标为k顶点路径比原两点间路径更短，将当前两点间权值设为更小的一个
                    (*D)[v][w]=(*D)[v][k]+(*D)[k][w];
                    (*P)[v][w]=(*P)[v][k];
                }
            }
        }
    }
 //求出的最短路径的显示代码
    for (v=0;v<G.numVertexes;++v)
    {
        for (w=v+1;w<G.numVertexes;w++)
        {
            printf("v%d-v%d weight:%d",v,w,D[v][w]);
            k=P[v][w];//获取第一个路径顶点下标
            printf(" Path:%d",v);//打印原点
            while (k!=w)
            {
                printf("->%d",k);
                k=P[k][w];//获取下一路径顶点下标
            }
            printf("->%d\n",w);//打印终点
        }
        printf("\n");
    }
 }


int _tmain(int argc, _TCHAR* argv[])
{
//  LinkStack S;
//  InitStack(&S);
//  Push(&S,3);
//  Push(&S,21);
//  Push(&S,15);
//  int e;
//  Pop(&S,&e);
//  Pop(&S,&e);
//  Pop(&S,&e);
//  cout<<e<<endl;

//  int i;
//  for (i=0;i<20;i++)
//  {
//      printf("%d\n",Fbi(i));
//  }

    return 0;
}
