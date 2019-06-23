//
//  main.cpp
//  shortPath
//
//  Created by 赵旭 on 2019/6/8.
//  Copyright © 2019 Zhaox. All rights reserved.
//

//邻接矩阵转化成邻接表存储，然后读入文件？

#include <iostream>

#define MaxV 52
#define MaxSize 20
#define INF 32767

int A[MaxV][MaxV],path[MaxV][MaxV];
int numList=0;

typedef struct
{
    int no;
    char name[MaxSize];
}VertexType;

typedef struct {
    int edges[MaxV][MaxV];
    int n,e;
    VertexType vexs[MaxV];
}MatGraph;

typedef struct
{
    int u;              //边的起始顶点
    int v;              //边的终止顶点
    int w;              //边的权值
}Edge;

typedef struct ANode
{
    int adjvex;         //该边邻接点编号
    struct ANode *nextarc;
    char name[MaxSize];
    int length;
}ArcNode;               //边结点类型

typedef struct VNode
{
    char name[MaxSize];
    ArcNode *firstarc;
}VNode;

typedef struct
{
    VNode adjlist[MaxV];        //邻接表头节点数组
    int n,e;
}AdjGraph;

//将最短路径邻接矩阵转换成邻接表G
void MatToList(MatGraph g,int A[][MaxV],AdjGraph *&G)
{
    int i,j,k,s;
    int apath[MaxV],d;
    ArcNode *p,*tmp;
    G =(AdjGraph *)malloc(sizeof(AdjGraph));
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
            if (A[i][j]!=INF && i!=j)       //顶点i和j之间存在最短路径
            {
                G->adjlist[numList].firstarc=NULL;                    //头结点信息
                for (int cl=0; cl<MaxSize; cl++) {
                    G->adjlist[numList].name[cl]='\0';
                }
                strcpy(G->adjlist[numList].name, g.vexs[i].name);     //头结点的指针域置初值
                
                k=path[i][j];
                d=0;
                apath[d]=j;                 //路径上添加终点
                while (k!=-1 && k!=i) {     //路径上添加中间点
                    d++;
                    apath[d]=k;
                    k=path[i][k];
                }
                d++;
                apath[d]=i;

                p=(ArcNode *)malloc(sizeof(ArcNode));
                p->adjvex=j;
                p->length=A[i][apath[d-1]];
                for (int cl=0; cl<MaxSize; cl++) {
                    p->name[cl]='\0';
                }
                strcpy(p->name, g.vexs[apath[d-1]].name);
 
                G->adjlist[numList].firstarc=p;
                tmp=p;
 
                for (s=d-2; s>=0; s--) {                            //尾插法添加 其它 中间顶点信息
                    p=(ArcNode *)malloc(sizeof(ArcNode));
                    p->adjvex=j;
                    p->length=A[apath[s+1]][apath[s]];
                    for (int cl=0; cl<MaxSize; cl++) {
                        p->name[cl]='\0';
                    }
                    strcpy(p->name, g.vexs[apath[s]].name);
                    tmp->nextarc=p;
                    tmp=p;
                }
                tmp->nextarc=NULL;
                numList++;
            }
        }
    }
    G->n=g.n;
    G->e=g.e;
}

//检查图文件是否存在或者是否为空
//---------------------------------------------------------------------
bool check_nullfile(char const *fileName)
{
    FILE *fp = fopen(fileName, "rb");
    //file not exist
    if (!fp) {
        printf("初始化文件不存在！程序将为您新建一个.\n");
        FILE *fp = fopen(fileName, "wb");
        fclose(fp);
        return false;
    }
    //file already exist
    else {
        char temp;
        
        int res = fscanf(fp, "%c", &temp);
        fclose(fp);
        if (res <= 0)
            return false;
        else
            return true;
    }
}

//在未到达文件尾的情况下，从文件中读取图的信息
//---------------------------------------------------------------------
void infoInit(char const *fileName,char str[MaxSize])
{
    FILE *fp;
    int i=0;
    for (int j=0; j<MaxSize; j++) {
        str[j]='\0';
    }
    
    bool res = check_nullfile(fileName);
    if ((fp = fopen(fileName, "rb"))==NULL) {
        if ((fp=fopen(fileName, "wb"))==NULL) {
            printf("Tips: 不能创建公园景点信息文件\n");
        }
    }
    else{
        while (res && !feof(fp)) {
            fscanf(fp, "%c",&str[i]);
            i++;
        }
    }
    fclose(fp);
}

//将公园景点信息存储到文件中
//---------------------------------------------------------------------
void infoFlush(char const *fileName,AdjGraph *G)
{
    FILE *fp;
    AdjGraph *p=G;
    int m,savecount = 0;
    
    if ((fp = fopen(fileName, "wb"))==NULL) {
        printf("Tips: 不能打开公园景点信息文件\n");
        return;
    }
    for (m=0; m<numList-1; m++) {
        ArcNode *h=G->adjlist[m].firstarc;
        fprintf(fp,"%s\t",p->adjlist[m].name);
        while (h->nextarc!=NULL) {
            fprintf(fp, "%s\t",h->name);
            fprintf(fp, "%d\t",h->length);
            h=h->nextarc;
            savecount++;
        }
        fprintf(fp, "%s\t",h->name);
        fprintf(fp, "%d\n",h->length);
        h=h->nextarc;
        savecount++;
    }
    
    ArcNode *h=G->adjlist[m].firstarc;
    fprintf(fp,"%s\t",p->adjlist[m].name);
    while (h->nextarc!=NULL) {
        fprintf(fp, "%s\t",h->name);
        fprintf(fp, "%d\t",h->length);
        h=h->nextarc;
        savecount++;
    }
    fprintf(fp, "%s\t",h->name);
    fprintf(fp, "%d",h->length);
    h=h->nextarc;
    savecount++;

    fclose(fp);
    if (savecount>0) {
        printf("景点路径信息已存入文件中\n");
    }
    else
        printf("未存入任何景点路径信息到文件中\n");
}

//读取字符串函数
int read_line(char str[],int n)
{
    int ch, i=0;
    while (isspace(ch=getchar()))
        ;
    while (ch!='\n' && ch != EOF) {
        if (i<n) {
            str[i++]=ch;
        }
        ch=getchar();
    }
    str[i]='\0';
    return i;
}

//直接插入排序,递增有序
void InsertSort(Edge R[],int n)
{
    int i,j;
    Edge tmp;
    for (i=1; i<n; i++) {
        if (R[i].w<R[i-1].w) {
            tmp.u=R[i].u;
            tmp.v=R[i].v;
            tmp.w=R[i].w;
            j=i-1;
            do {
                R[j+1].u=R[j].u;
                R[j+1].v=R[j].v;
                R[j+1].w=R[j].w;
                j--;
            } while (j>=0 && R[j].w>tmp.w);
            R[j+1].u=tmp.u;
            R[j+1].v=tmp.v;
            R[j+1].w=tmp.w;
        }
    }
}

void inputGraph(MatGraph &g)
{
    int i,j,m=0,p=0;
    int dis;
    int loop=0;

    do {
        loop++;
        printf("第%d次循环\n",loop);
        printf("输入相邻的两个景点名字(英语)\n");
        printf("请输入第一个景点(英语): ");
        read_line(g.vexs[g.n+1].name, MaxSize);
        
        while (m<g.n) {
            if (strcmp(g.vexs[m].name, g.vexs[g.n+1].name)==0) {
                break;
            }
            m++;
        }
        
        if (m == g.n) {
            while (p<g.n) {
                if (strlen(g.vexs[p].name)==0) {
                    strcpy(g.vexs[p].name, g.vexs[g.n+1].name);
                    break;
                }
                p++;
                
            }
        }
        
        printf("请输入第二个景点(英语): ");
        read_line(g.vexs[g.n+2].name, MaxSize);
        m=0;p=0;
        
        while (m<g.n) {
            if (strcmp(g.vexs[m].name, g.vexs[g.n+2].name)==0) {
                break;
            }
            m++;
        }
        if (m == g.n) {
            while (p<g.n) {
                if (strlen(g.vexs[p].name)==0) {
                    strcpy(g.vexs[p].name, g.vexs[g.n+2].name);
                    break;
                }
                p++;
            }
        }
        
        printf("请输入这两个景点之间的距离:");
        scanf("%d",&dis);
        
        for (i=0; i<g.n; i++) {
            for (j=0; j<g.n; j++) {
                if ((strcmp(g.vexs[i].name, g.vexs[g.n+1].name)==0) && (strcmp(g.vexs[j].name, g.vexs[g.n+2].name))==0) {
                    g.edges[i][j]=dis;
                }
            }
        }
        m=0;p=0;
        printf("\n");
        
    } while (loop < g.e);
    
}

void disGraph(MatGraph g)
{
    int i,j;
    printf("输出图:\n");
    for (int k=0; k<g.n; k++) {
        printf("\t%s",g.vexs[k].name);
    }
    printf("\n");
    for (i=0;i<g.n;i++)
    {
        printf("%s",g.vexs[i].name);
        for (j=0;j<g.n;j++)
            if (g.edges[i][j]!=INF)
                printf("\t%d",g.edges[i][j]);
            else
                printf("\t%s","∞");
        printf("\n");
    }
    
}

void disPath(MatGraph g,int dist[],int path[],int S[],int m,int p)
//输出顶点出发的所有最短路径
{
    int j,k;
    int apath[MaxV],d;
    
    if (S[p]==1&&p!=m) {
        printf("从景点%s到景点%s的最短路径长度为：%d\t路径为: ",g.vexs[m].name,g.vexs[p].name,dist[p]);
        d=0;apath[d]=p;
        k=path[p];
        if (k==-1) {
            printf("无路径\n");
        }
        else
        {
            while (k!=m) {
                d++;
                apath[d]=k;
                k=path[k];
            }
            d++;
            apath[d]=m;
            printf("%s",g.vexs[apath[d]].name);
            for (j=d-1; j>=0; j--) {
                printf(",%s",g.vexs[apath[j]].name);
            }
            printf("\n");
        }
    }
}

void Dijkstra(MatGraph g)
{
    int dist[MaxV],path[MaxV];
    int S[MaxV];                                    //S[i]=1表示顶点i在S中,S[i]=0表示顶点i在U中
    int MINdis,i,j,u = 0;
    int m,p;
    char start[MaxSize],end[MaxSize];
    
    printf("请输入起始景点名称: ");
    read_line(start, MaxSize);
    printf("请输入终止景点名称: ");
    read_line(end, MaxSize);
    
    //查找两者对应的顶点
    for (m=0; m<g.n; m++) {
        if(strcmp(start, g.vexs[m].name)==0)
            break;
    }
    if (m==g.n) {
        printf("Failed:没找到对应的起始景点\n");
        return;
    }
    
    for (p=0; p<g.n; p++) {
        if (strcmp(end, g.vexs[p].name)==0) {
            break;
        }
    }
    if (p==g.n) {
        printf("Failed:没找到对应的终止景点\n");
    }
    
    for (i=0; i<g.n; i++) {
        dist[i]=g.edges[m][i];                      //距离初始化
        S[i]=0;
        if (g.edges[m][i]<=INF) {                   //路径初始化
            path[i]=m;
        }
        else
            path[i]=-1;
    }
    S[m]=1;                                        //源点编号v放入S中
    path[m]=0;
    for (i=0; i<g.n-1; i++) {
        MINdis=INF;
        for (j=0; j<g.n; j++) {
            if (S[j]==0&&dist[j]<MINdis) {
                u=j;
                MINdis=dist[j];
            }
        }
        S[u]=1;
        for (j=0; j<g.n; j++) {
            if (S[j]==0) {
                if (g.edges[u][j]<INF && dist[u]+g.edges[u][j]<dist[j]) {
                    dist[j]=dist[u]+g.edges[u][j];
                    path[j]=u;
                }
            }
        }
    }
    disPath(g, dist, path, S, m, p);
}

void disAllShort(MatGraph g,int A[][MaxV],int path[][MaxV])
{
    int i,j,k,s;
    int apath[MaxV],d;
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
            if (A[i][j]!=INF && i!=j) {
                printf("从%s到%s的路径为: ",g.vexs[i].name,g.vexs[j].name);
                k=path[i][j];
                d=0;apath[d]=j;
                while (k!=-1 && k!=i) {
                    d++;apath[d]=k;
                    k=path[i][k];
                }
                d++;apath[d]=i;
                printf("%s",g.vexs[apath[d]].name);
                for (s=d-1; s>=0; s--)
                {
                    printf(",%s",g.vexs[apath[s]].name);
                }
                printf("\t路径长度: %d\n",A[i][j]);
            }
        }
    }
}
/*
int visited[MaxV]={0};
void DFS(AdjGraph *G,int v)
{
    ArcNode *p;
    visited[v]=1;
    printf("%d",v);
    p=G->adjlist[v].firstarc;
    while (p!=NULL) {
        if (visited[p->adjvex]==0) {
            DFS(G, p->adjvex);
        }
        p=p->nextarc;
    }
}
*/
//Floyd Algorithm
void Floyd(MatGraph g)
{
    int i,j,k;
    //A和path初始化
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
            A[i][j]=g.edges[i][j];
            if (i!=j && g.edges[i][j]!=INF) {
                path[i][j]=i;
            }
            else
                path[i][j]=-1;
        }
    }
    for (k=0; k<g.n; k++) {
        for (i=0; i<g.n; i++) {
            for (j=0; j<g.n; j++) {
                if (A[i][j]>A[i][k] + A[k][j]) {
                    A[i][j]=A[i][k]+A[k][j];            //修改最短路径长度
                    path[i][j]=path[k][j];              //修改最短路径
                }
            }
        }
    }
    disAllShort(g,A,path);
}

int main(int argc, const char * argv[]) {

    MatGraph g;
    int i,j;
    AdjGraph *G;
    
    printf("请输入景点总个数(至多50个):");
    scanf("%d",&g.n);
    printf("请输入总的边数:");
    scanf("%d",&g.e);
    //初始化图
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
            if (i==j) {
                g.edges[i][j]=0;
            }
            else
                g.edges[i][j]=INF;
        }
    }
    
    //初始化顶点
    for (i=0; i<g.n; i++) {
        g.vexs[i].no= i ;
        strcpy(g.vexs[i].name,"\0");
        
    }
    //输出图
    inputGraph(g);
    disGraph(g);
    printf("\n");

//    Dijkstra(g);
//    printf("\n");
//    printf("\nFloyd: \n");
    Floyd(g);
    MatToList(g, A, G);
//    DFS(G, 0);
    printf("\n");
    infoFlush("AllPath.dat", G);
    return 0;
   
}

//将最短路径上的各旅游景点及每段路径长度写入磁盘文件AllPath.dat      借助disAllShort()?
//从文件中读取所有旅游景点之间的最短路径信息，到内存链表中管理
