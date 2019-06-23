### 四、实验原理：

**1.图的定义**

​	图（graph）G由两个集合V（vertex）和E（edge）构成，记为G=(V, E)。其中：V是顶点的有限集合，记为V(G)；E是连接V中两个不同顶点的边的有限集合，记为E(G)。

**2.图的存储方法**

​	图的**邻接矩阵**是一种采用邻接矩阵数组表示顶点之间相邻关系的存储结构。邻接矩阵类型声明如下：

```C++
#define MAXV <最大顶点个数>
#define INF 32767
typedef struct
{
  int no;							//顶点的编号
  InfoType info;			//顶点的其它信息
}VertexType;
typedef struct
{
  int edges[MAXV][MAXV];
  int n,e;						//顶点数，边数
  VertexType vexs[MAXV];//存放顶点信息
}MatGraph;
```

​	图的**邻接表**是一种顺序和链式存储结合的存储方法。邻接表类型声明如下：

```C++
typedef struct ANode
{
  int adjvex;					//该边的邻接点编号
  struct ANode *nextarc;//指向下一条边的指针
  int weight;
}ArcNode;
typedef struct VNode
{
  InfoType info;
  ArcNode *firstarc;		//指向第一个边结点
}VNode;
typedef struct
{
  VNode adjlist[MAXV];	//邻接表的头结点数组
  int n,e;
}AdjGraph;
```

​	**其他存储方法**：十字链表、邻接多重表等。

**3.路径与最短路径**

​	在一个图G=(V,E)中，从顶点i到顶点j的一条路径是一个顶点序列。路径长度是指一条路径上经过的边的数目。若一条路径上除开始点和结束点可以相同以外，其余顶点均不相同，则称此路径为简单路径。

对于不带权的图，若从一顶点到另一顶点存在着一条路径，则称该路径长度为该路径所经过的边的数目，它等于该路径上的顶点数减1。把路径长度最小的路径称为最短路径，其长度称为最短路径长度或者最短距离。

​	对于带权图，将一条路径上所经边的权之和定义为该路径的路径长度。其中，路径长度最小的路径被称为最短路径。

​	实际上，可以将不带权图上的每条边看成权值为1的边，那么带权图和不带权图的最短距离和最短距离的定义就一样了。

**4.最短路径算法**
	从一个顶点到其余各顶点的最短路径算法：Dijkstra算法

​	图中每对顶点之间的最短路径算法：Floyd算法

### 五、实验目的：

​	通过本实验练习，让学生掌握图在计算机内存、磁盘文件中的存储方法；掌握求图中（任意）两点之间最短路径的算法；学习并掌握图如何存储到数据文件中；自行设计、掌握内存链表的数据结构；进一步提高学生的工程实践能力。

### 六、实验内容：

​	某公园内有n个连通的旅游景点，游客需要查询任意景点间最短路径。请设计并编程实现如下功能：

1) 设计数据结构与界面，输入直接相邻的两个旅游景点的名字以及它们之间的距离；并将每对直接相连的景点间的距离存到磁盘文件中。

2) 设计算法，实现计算给定的两个旅游景点间的最短路径；

3) 对公园的所有旅游景点，设计算法实现计算所有的景点对之间的最短路径，并将最短路径上的各旅游景点及每段路径长度写入磁盘文件AllPath.dat中。

4) 编写程序从文件AllPath.dat中读出所有旅游景点间的最短路径信息，到内存链表中管理；请运用所学的数据结构知识，设计内存链表的数据结构，实现用户输入任意两个旅游景点，能快速地从内存链表查询出两景点间的最短路径。

### 七、实验器材（设备、元器件）：

​	PC机一台，装有C语言集成开发环境。

​	注意：本实验用机为MacOS系统，使用Xcode 编程，默认生成类Unix可执行文件，对中文支持不友好。所以本案例所有内容均为英文输入与输出。

### 八、数据结构与程序

**1.图的邻接表存储结构**

```C++
typedef struct ANode
{
    int adjvex;         //该边邻接点编号
    struct ANode *nextarc;
    char name[MaxSize];
    int length;
}ArcNode;               //边结点类型

typedef struct
{
    ArcNode adjlist[MaxV];        //邻接表头节点数组
    int n,e;
}AdjGraph;
```

**2.图的邻接矩阵存储结构**

```C++
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
```

**3.全局变量、宏定义与头文件等**

```C++
#include <iostream>

#define MaxV 52
#define MaxSize 20
#define INF 32767

int A[MaxV][MaxV],path[MaxV][MaxV];
int numList=0;
```

**4.主函数**

```C++
int main(int argc, const char * argv[]) 
{
    
    MatGraph g;
    int i,j;
    AdjGraph *G,*P;
    int choice;
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
    //输入与输出图
    inputGraph(g);
    printf("图已建立，如下：\n");
    disGraph(g);
    printf("\n");
    while (1) {
        printf("\n");
        printf("********************公园景点路径********************\n");
        printf("1.查询两个景点之间的最短路径\n");
        printf("2.任意两个景点之间的最短路径\n");
        printf("3.将最短路径保存到 AllPath.dat文件中\n");
        printf("4.从文件中读取，并查询两个景点之间的最短路径\n");
        printf("\n");
        printf("请输入你要进行的操作:");
        scanf("%d",&choice);
        switch (choice) {
            case 1:
                Dijkstra(g);
                break;
            case 2:
                Floyd(g);
                break;
            case 3:
             		Floyd(g);
                MatToList(g, A, G);
                infoFlush("AllPath.dat", G);
                printf("已存入到文件中\n");
                break;
            case 4:
                infoInit("AllPath.dat", P);
                printf("已读取到内存链表中\n");
                Dijkstra(g);
                break;
            default:
                printf("您已退出系统\n");
                return 0;
        }
    }
    return 0;
}
```

**5.检查图文件是否存在或者是否为空**

```C++
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
```

**6.在未到达文件尾的情况下，从文件中读取图的信息**

```C++
void infoInit(char const *fileName,AdjGraph *&G)
{
    FILE *fp;
    int m = 0;
    
    G=(AdjGraph *)malloc(sizeof(AdjGraph));
    
    bool res = check_nullfile(fileName);
    if ((fp = fopen(fileName, "rb"))==NULL) {
        if ((fp=fopen(fileName, "wb"))==NULL) {
            printf("Tips: 不能读取公园景点信息文件\n");
        }
    }
    else{
        while (res && !feof(fp)) {
            
            for (int cl=0; cl<MaxSize; cl++)
                G->adjlist[m].name[cl]='\0';
            
            fscanf(fp, "%s",G->adjlist[m].name);
            
            ArcNode *h=G->adjlist[m].nextarc;
            
            h=(ArcNode*)malloc(sizeof(ArcNode));
            
            for (int cl=0; cl<MaxSize; cl++)
                h->name[cl]='\0';
            
            fscanf(fp,"\t%s", h->name);
            fscanf(fp, "\t%d",&(h->length));
            
            h=h->nextarc;
            
            m++;
        }
    }
    
    fclose(fp);
}

```

**7.将公园景点信息存储到文件中**

```C++
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
        ArcNode *h=G->adjlist[m].nextarc;
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
    
    ArcNode *h=G->adjlist[m].nextarc;
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

```

**8.将最短路径邻接矩阵转换成邻接表（内存链表）G**

```C++
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
                G->adjlist[numList].nextarc=NULL;                    //头结点信息
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
                
                G->adjlist[numList].nextarc=p;
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
```

**9.读取字符串函数**

```C++
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

```

**10.直接插入排序（递增有序）**

```C++
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

```

**11.输入图**

```C++
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

```

**12.输出图**

```C++
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
```

**13.输出某景点出发的一条最短路径**

```C++
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

```

**14.Dijkstra算法求最短路径**

```C++
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

```

**15.输出任意两对景点之间的最短路径**

```C++
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

```

**16.Floyd算法求最短路径**

```C++
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
```

### 九、程序运行结果：

**1.程序控制界面**

​	进入程序，首先需要输入景点总个数、边数，然后依次输入相邻景点的名字、距离，如下图：

​	在本演示中，输入图的有向边与距离为:

​	A->B = 5; A->C = 7; D->A = 3; C->D = 1; B->C = 2

![屏幕快照 2019-06-23 下午8.13.58](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.13.58.png)

**2.输出图**

​	输入完相关邻接景点信息后，程序将自动将图的信息以邻接矩阵的方式显示，如下图：

![屏幕快照 2019-06-23 下午8.21.40](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.21.40.png)

**3.查询给定两个景点间的最短路径**

​	使用Dijkstra算法，并稍加改动，查询给定两个景点之间的最短路径，如下图

![屏幕快照 2019-06-23 下午8.26.03](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.26.03.png)

**4.查询任意两个景点之间的最短路径**

​	使用Floyd算法，查询任意两个景点之间的最短路径，如下图：

![屏幕快照 2019-06-23 下午8.28.29](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.28.29.png)

**5.将最短路径保存到AllPath.dat文件中**

​	将任意两个景点之间的最短路径保存到AllPath.dat中，如下图:

![屏幕快照 2019-06-23 下午8.29.55](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.29.55.png)

​	文件中内容如下：

![屏幕快照 2019-06-23 下午8.30.51](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.30.51.png)

**6.从文件中读取，并查询两个景点之间的最短路径**

​	从文件中读取到内存链表中，并查询给定两个景点之间的最短路径，如下图

![屏幕快照 2019-06-23 下午8.32.10](/Users/zhaoxu/Desktop/屏幕快照 2019-06-23 下午8.32.10.png)



### 十、实验结论：

​	**1.任务1:输入图并存储图**

​	首先让用户输入所有景点数、边数，然后循环，依次让用户输入景点信息、边的信息，并将之存储到图的邻接矩阵中。

​	**2.任务2:输出图**

​	循环遍历图的邻接矩阵，将图的信息输出

**3.任务3:查询给定两个景点之间的最短路径**

​	在Dijkstra算法的基础上，稍加改动。让用户输入开始与终止景点，查询这两个景点之间的最短路径。

**4.任务4:查询任意两个景点之间的最短路径**

​	使用Floyd算法，查询任意两个景点之间的最短路径。

**5.任务5:将任意两个景点之间的最短路径保存到AllPath.dat中**

​	使用MatToList(),将图的邻接矩阵存储方法转换成邻接表存储，然后使用infoFlush()、check_nullfile()函数，将景点信息写入到文件AllPath.dat中。

**6.任务6:从文件中读取景点信息，并查询给定两个景点之间的 最短路径**

​	使用infoInit()函数，将文件中景点信息读取到内存链表中，然后让用户输入起始与终止景点，查询这两个景点之间的最短路径。



### 十一、总结及心得体会：

​	**1.总结**

​	通过实验，进一步掌握了图的存储结构，掌握了如何将图写入到文件中，掌握如何从文件中读取图的信息；而且进一步理解、掌握了求最短路径的算法Dijkstra算法与Floyd算法。

​	**2.实验代码可改进之处**

​	1）将图写入文件，与从文件中读取图的信息比较复杂，这里采用的方法也比较麻烦。将邻接矩阵存储的图转化成邻接表表示。

​	因此，可以考虑进一步优化存储结构，在邻接表的基础上简化该结构，以简化代码。

​	2）输入图比较麻烦，交互做得还不够好。本程序一开始就需要用户输入所有景点个数与路径数，这一点在交互比较麻烦。

​	而且，如果用户输入"非法"，会直接退出系统。可以考虑对用户的输入进行限制等。

​	**3.心得与体会**

​	1）首先是如何将用户输入的相邻景点之间的信息存储到图中。最后我采用循环遍历的方法，比较每个结构体内的name,判断将这个信息存储到何处。

​		这里我使用了一个技巧，将用户输入的名字存到g.n+1,与g.n+2的位置，避免了冲突，以便于找到合适位置之后进行替换。

​	2）对Dijkstra算法的改进。Dijkstra算法是求一个顶点到其余各顶点的最短路径。而本实验只要求给定两个景点之间的 最短路径。

​		因此，我加入了循环与比对，找到对应的结点，只将这两个特定景点之间的最短路径输出。

​	3）内存链表的设计。如何将图的信息存储到文件中。我最终借助邻接表，先将图的邻接矩阵换成邻接表存储，然后将邻接表写入到文件中。

​	**4.感想**

​	本次实验耗时较久，我也遇到了很多困难，但我认识到，很多问题只要理解一些基本操作，算法，在此基础上稍加改进，就可以解决很多问题。而这些小的改进才是个人思考的结晶，只有在思考这些改动的过程中，才能进一步体会到编程的乐趣，体会到成就感。

