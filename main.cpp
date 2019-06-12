//
//  main.cpp
//  shortPath
//
//  Created by 赵旭 on 2019/6/8.
//  Copyright © 2019 Zhaox. All rights reserved.
//

#include <iostream>

#define MaxV 52
#define MaxSize 20
#define INF 32767

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

void disGraph(MatGraph g)
{
    int i,j;
    printf("输出图:\n");
    for (i=0;i<g.n;i++)
    {
        for (j=0;j<g.n;j++)
            if (g.edges[i][j]!=INF)
                printf("%4d",g.edges[i][j]);
            else
                printf("%4s","∞");
        printf("\n");
    }

}

void inputGraph(MatGraph &g)
{
    int i,j,m=0,p=0;
    int dis;
    int loop=0;
    char choice;

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
        
        printf("是否仍存在直接相邻的景点？(Y/N) ");
        getchar();
        scanf("%c",&choice);
        printf("\n");
        
    } while (choice == 'Y' ||choice == 'y');
    
}

//Kruskal Algorithm
void Kruskal(MatGraph g)
{
    int k,i,j;
    int sn1,sn2,u1,v1;
    k=0;
    int vset[MaxV];
    Edge E[MaxV];
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
            if (g.edges[i][j]!=0 && g.edges[i][j]!=INF) {       //由g产生边集E
                E[k].u=i;
                E[k].v=j;
                E[k].w=g.edges[i][j];
                k++;
            }
        }
    }
    InsertSort(E, k);
    for (i=0; i<g.n; i++) {
        vset[i]=i;
    }
    k=1;
    j=0;
    while (k<g.n) {
        u1=E[j].u;              //取一个边的两个顶点
        v1=E[j].v;
        sn1=vset[u1];
        sn2=vset[v1];
        if (sn1!=sn2) {        //两顶点属于不同的集合
            printf("(%d,%d):%d\n",u1,v1,E[j].w);    //输出最小生成树的一条边
            k++;
            for (i=0; i<g.n; i++) {                 //两个集合统一编号
                if (vset[i]==sn2) {
                    vset[i]=sn1;
                }
            }
        }
        j++;                //扫描下一条边
    }
}

void disAllShort(MatGraph g,int A[][MaxV],int path[][MaxV])
{
    int i,j,k,s;
    int apath[MaxV],d;
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
            if (A[i][j]!=INF && i!=j) {
                printf("从%c到%c的路径为: ",i,j);
                k=path[i][j];
                d=0;apath[d]=j;
                while (k!=-1 && k!=i) {
                    d++;apath[d]=k;
                    k=path[i][k];
                }
                d++;apath[d]=i;
                printf("%d",apath[d]);
                for (s=d-1; s>=0; s--) {
                    printf(",%c",apath[s]);
                }
                printf("\t路径长度: %d\n",A[i][j]);
            }
        }
    }
}

//Floyd Algorithm
void Floyd(MatGraph g)
{
    int A[MaxV][MaxV],path[MaxV][MaxV];
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
    
    printf("请输入景点总个数(至多50个):");
    scanf("%d",&g.n);
    printf("请输入总的边数:");
    scanf("%d",&g.e);
    //初始化图
    for (i=0; i<g.n; i++) {
        for (j=0; j<g.n; j++) {
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
    
    printf("\nKruskal: \n");
    Kruskal(g);
    printf("\nFloyd: \n");
    Floyd(g);
    
    return 0;
   
}
