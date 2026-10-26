#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define NONE 999
#define INF 1000000

int num_node;
int** weight;
int* parent;

void set_init(int n){
    for(int i=0; i<n; i++){
        parent[i] = -1;
    }
}
int set_find(int curr){
    if(parent[curr] == -1)
        return curr;
    while(parent[curr] != -1)
        curr = parent[curr];
    return curr;
}
void set_union(int a, int b){
    int root1 = set_find(a);
    int root2 = set_find(b);
    if(root1 != root2)
        parent[root1] = root2;
}

struct Edge{
    int start, end, weight;
};
int compare_min(const void* a, const void* b){
    struct Edge* x = (struct Edge*)a;
    struct Edge* y = (struct Edge*)b;
    return (x->weight - y->weight);
}

int compare_max(const void* a, const void* b){
    struct Edge* x = (struct Edge*)a;
    struct Edge* y = (struct Edge*)b;
    return (y->weight - x->weight);
}
void print_tree(struct Edge tree[], int n, int sum, int type){
    if(type == 0)
        printf("최소비용신장트리 = { ");
    else
        printf("최대비용신장트리 = { ");
    for(int i=0; i<n; i++){
        printf("(%d, %d, %d)", tree[i].start + 1, tree[i].end + 1, tree[i].weight);
        if(i < n - 1)
            printf(", ");
    }
    printf(" }\n");
    printf("가중치 합 = %d\n", sum);
}

void kruskal(int type){
    struct Edge* edges;
    struct Edge* tree;
    struct Edge e;
    int edge_count = 0, edge_accepted = 0;
    int uset, vset, i, sum = 0;
    edges = (struct Edge*)malloc(sizeof(struct Edge) * num_node * num_node);

    for(i=0; i<num_node; i++){
        for(int j=i+1; j<num_node; j++){
            if(weight[i][j] != NONE){
                edges[edge_count].start = i;
                edges[edge_count].end = j;
                edges[edge_count].weight = weight[i][j];
                edge_count++;
            }
        }
    }

    tree = (struct Edge*)malloc(sizeof(struct Edge) * num_node);
    parent = (int*)malloc(sizeof(int) * num_node);
    set_init(num_node);

    if(type == 0){
        qsort(edges, edge_count, sizeof(struct Edge), compare_min);
        printf("\n----- Kruskal 최소비용신장트리 -----\n");
    }
    else{
        qsort(edges, edge_count, sizeof(struct Edge), compare_max);
        printf("\n----- Kruskal 최대비용신장트리 -----\n");
    }

    i = 0;
    while(edge_accepted < num_node - 1 && i < edge_count){
        e = edges[i];
        uset = set_find(e.start);
        vset = set_find(e.end);
        if(uset != vset){
            printf("(%d, %d) 가중치: %d 선택\n", e.start + 1, e.end + 1, e.weight);
            sum += e.weight;
            tree[edge_accepted] = e;
            edge_accepted++;
            set_union(uset, vset);
        }
        i++;
    }

    if(edge_accepted < num_node - 1){
        if(type == 0)
            printf("최소비용신장트리를 구할 수 없다\n");
        else
            printf("최대비용신장트리를 구할 수 없다\n");
    }
    else
        print_tree(tree, edge_accepted, sum, type);

    free(edges);
    free(tree);
    free(parent);
}

int* selected;
int* distance;
int* from;

int get_min_vertex(int n){
    int v = -1;
    for(int i=0; i<n; i++){
        if(!selected[i] && distance[i] != INF){
            if(v == -1 || distance[i] < distance[v])
                v = i;
        }
    }
    return v;
}
int get_max_vertex(int n){
    int v = -1;
    for(int i=0; i<n; i++){
        if(!selected[i] && distance[i] != -1){
            if(v == -1 || distance[i] > distance[v])
                v = i;
        }
    }
    return v;
}

void prim(int s, int type){
    int u, v, sum = 0, cnt = 0;
    int no_value;
    struct Edge* tree;

    if(type == 0)
        no_value = INF;
    else
        no_value = -1;

    selected = (int*)malloc(sizeof(int) * num_node);
    distance = (int*)malloc(sizeof(int) * num_node);
    from = (int*)malloc(sizeof(int) * num_node);
    tree = (struct Edge*)malloc(sizeof(struct Edge) * num_node);

    for(u=0; u<num_node; u++){
        distance[u] = no_value;
        selected[u] = FALSE;
        from[u] = -1;
    }
    distance[s] = 0;

    if(type == 0)
        printf("\n----- Prim 최소비용신장트리 (출발 노드 %d) -----\n", s + 1);
    else
        printf("\n----- Prim 최대비용신장트리 (출발 노드 %d) -----\n", s + 1);

    for(int i=0; i<num_node; i++){
        if(type == 0)
            u = get_min_vertex(num_node);
        else
            u = get_max_vertex(num_node);

        if(u == -1){
            if(type == 0)
                printf("최소비용신장트리를 구할 수 없다\n");
            else
                printf("최대비용신장트리를 구할 수 없다\n");
            free(selected);
            free(distance);
            free(from);
            free(tree);
            return;
        }

        selected[u] = TRUE;
        if(from[u] != -1){
            printf("(%d, %d) 가중치: %d 선택\n", from[u] + 1, u + 1, weight[from[u]][u]);
            sum += weight[from[u]][u];
            tree[cnt].start = from[u];
            tree[cnt].end = u;
            tree[cnt].weight = weight[from[u]][u];
            cnt++;
        }

        for(v=0; v<num_node; v++){
            if(selected[v] || weight[u][v] == NONE || u == v)
                continue;
            if((type == 0 && weight[u][v] < distance[v]) || (type == 1 && weight[u][v] > distance[v])){
                distance[v] = weight[u][v];
                from[v] = u;
            }
        }
    }
    print_tree(tree, cnt, sum, type);

    free(selected);
    free(distance);
    free(from);
    free(tree);
}

int* found;
int* prev;

int choose(int distance[], int n, int found[]){
    int min = INF, minpos = -1;
    for(int i=0; i<n; i++){
        if(distance[i] < min && !found[i]){
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}

void print_path(int v){
    if(prev[v] != -1){
        print_path(prev[v]);
        printf(" -> ");
    }
    printf("%d", v + 1);
}

void shortest_path(int start){
    int i, u, w;

    distance = (int*)malloc(sizeof(int) * num_node);
    found = (int*)malloc(sizeof(int) * num_node);
    prev = (int*)malloc(sizeof(int) * num_node);

    for(i=0; i<num_node; i++){
        if(weight[start][i] == NONE){
            distance[i] = INF;
            prev[i] = -1;
        }
        else{
            distance[i] = weight[start][i];
            prev[i] = start;
        }
        found[i] = FALSE;
    }
    found[start] = TRUE;
    distance[start] = 0;
    prev[start] = -1;

    for(i=0; i<num_node-1; i++){
        u = choose(distance, num_node, found);
        if(u == -1)
            break;
        found[u] = TRUE;
        for(w=0; w<num_node; w++){
            if(!found[w] && weight[u][w] != NONE){
                if(distance[u] + weight[u][w] < distance[w]){
                    distance[w] = distance[u] + weight[u][w];
                    prev[w] = u;
                }
            }
        }
    }

    printf("\n----- Dijkstra 최단경로 (시작 노드 %d) -----\n", start + 1);
    for(i=0; i<num_node; i++){
        if(i == start)
            continue;
        if(distance[i] == INF)
            printf("%d -> %d : 경로 없음\n", start + 1, i + 1);
        else{
            printf("%d -> %d : 길이 %d,  경로 : ", start + 1, i + 1, distance[i]);
            print_path(i);
            printf("\n");
        }
    }

    free(distance);
    free(found);
    free(prev);
}

int** A;
int** next;

void floyd_path(int i, int j){
    int k = next[i][j];
    if(k == -1)
        return;
    floyd_path(i, k);
    printf(" -> %d", k + 1);
    floyd_path(k, j);
}

void floyd(void){
    int i, j, k;

    A = (int**)malloc(sizeof(int*) * num_node);
    next = (int**)malloc(sizeof(int*) * num_node);
    for(i=0; i<num_node; i++){
        A[i] = (int*)malloc(sizeof(int) * num_node);
        next[i] = (int*)malloc(sizeof(int) * num_node);
    }

    for(i=0; i<num_node; i++){
        for(j=0; j<num_node; j++){
            if(weight[i][j] == NONE)
                A[i][j] = INF;
            else
                A[i][j] = weight[i][j];
            next[i][j] = -1;
        }
    }

    for(k=0; k<num_node; k++){
        for(i=0; i<num_node; i++){
            for(j=0; j<num_node; j++){
                if(A[i][k] + A[k][j] < A[i][j]){
                    A[i][j] = A[i][k] + A[k][j];
                    next[i][j] = k;
                }
            }
        }
    }
}

void floyd_menu(void){
    int s, d, i, j;

    floyd();

    while(1){
        printf("출발 노드와 도착 노드를 입력하세요.(1 ~ %d)\n--> ", num_node);
        if(scanf("%d %d", &s, &d) != 2){
            while(getchar() != '\n');
            s = d = 0;
        }
        if(s >= 1 && s <= num_node && d >= 1 && d <= num_node)
            break;
        printf("잘못된 노드 번호입니다.\n");
    }
    s--;
    d--;

    printf("\n----- Floyd 최단 경로 -----\n");
    if(A[s][d] >= INF)
        printf("%d -> %d : 경로 없음\n", s + 1, d + 1);
    else{
        printf("%d -> %d : 길이 %d,  경로 : %d", s + 1, d + 1, A[s][d], s + 1);
        floyd_path(s, d);
        if(s != d)
            printf(" -> %d", d + 1);
        printf("\n");
    }

    printf("\n----- 모든 노드 쌍의 최단경로 길이 -----\n    ");
    for(j=0; j<num_node; j++){
        printf("%4d", j + 1);
    }
    printf("\n");
    for(i=0; i<num_node; i++){
        printf("%4d", i + 1);
        for(j=0; j<num_node; j++){
            if(A[i][j] >= INF)
                printf("   *");
            else
                printf("%4d", A[i][j]);
        }
        printf("\n");
    }

    printf("\n----- 모든 노드 쌍의 최단 경로 -----\n");
    for(i=0; i<num_node; i++){
        for(j=0; j<num_node; j++){
            if(i == j)
                continue;
            if(A[i][j] >= INF){
                printf("%d -> %d : 경로 없음\n", i + 1, j + 1);
                continue;
            }
            printf("%d -> %d : 길이 %2d,  경로 : %d", i + 1, j + 1, A[i][j], i + 1);
            floyd_path(i, j);
            printf(" -> %d\n", j + 1);
        }
    }

    for(i=0; i<num_node; i++){
        free(A[i]);
        free(next[i]);
    }
    free(A);
    free(next);
}

int input_node(void){
    int node;
    while(1){
        printf("출발 노드를 선택하세요.(1 ~ %d)\n--> ", num_node);
        if(scanf("%d", &node) != 1){
            while(getchar() != '\n');
            node = 0;
        }
        if(node >= 1 && node <= num_node)
            return node - 1;
        printf("잘못된 노드 번호입니다.\n");
    }
}

int input_type(void){
    int type;
    while(1){
        printf("Min: 0, Max: 1\n--> ");
        if(scanf("%d", &type) != 1){
            while(getchar() != '\n');
            type = -1;
        }
        if(type == 0 || type == 1)
            return type;
        printf("0또는 1을 입력하세요\n");
    }
}

int main(int argc, char* argv[]){
    FILE* fp = NULL;
    char fname[256];
    int menu;

    if(argc == 2){
        fp = fopen(argv[1], "r");
        if(fp == NULL)
            printf("파일 열기 실패 : %s는 없는 파일입니다.\n", argv[1]);
    }
    else
        printf("<Usage>: graph graph_B.txt\n");

    while(fp == NULL){
        printf("입력 파일 이름 : ");
        scanf("%s", fname);
        fp = fopen(fname, "r");
        if(fp == NULL)
            printf("파일 열기 실패 : %s는 없는 파일입니다.\n", fname);
    }

    if(fscanf(fp, "%d", &num_node) != 1 || num_node < 1){
        printf("파일 형식 오류 : 노드 개수를 읽을 수 없습니다.\n");
        fclose(fp);
        return 0;
    }
    weight = (int**)malloc(sizeof(int*) * num_node);
    for(int i=0; i<num_node; i++){
        weight[i] = (int*)malloc(sizeof(int) * num_node);
    }
    for(int i=0; i<num_node; i++){
        for(int j=0; j<num_node; j++){
            if(fscanf(fp, "%d", &weight[i][j]) != 1){
                printf("파일 형식 오류 : 인접 행렬 값이 부족합니다.\n");
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);

    printf("\n----- 인접 행렬 (노드 %d개) -----\n", num_node);
    for(int i=0; i<num_node; i++){
        for(int j=0; j<num_node; j++){
            printf("%4d", weight[i][j]);
        }
        printf("\n");
    }

    while(1){
        printf("\n1~5 중에서 입력하세요(1:Kruskal, 2:Prim, 3:Dijkstra, 4:Floyd, 5:종료)\n");
        printf("선택 : ");
        if(scanf("%d", &menu) != 1){
            while(getchar() != '\n');
            menu = 0;
        }

        if(menu == 5)
            break;
        else if(menu == 1)
            kruskal(input_type());
        else if(menu == 2){
            int type = input_type();
            prim(input_node(), type);
        }
        else if(menu == 3)
            shortest_path(input_node());
        else if(menu == 4)
            floyd_menu();
        else
            printf("1, 2, 3, 4, 5 중에서 선택하세요.\n");
    }
    printf("프로그램을 종료합니다.\n");

    for(int i=0; i<num_node; i++){
        free(weight[i]);
    }
    free(weight);

    return 0;
}