/*
 * 알고리즘 그래프 프로그래밍 과제 (주간반)
 *
 *  실행 방법 : graph graph_B.txt
 *
 *  1. Kruskal : 최소/최대 비용 신장 트리 (union-find 사용)
 *  2. Prim    : 최소/최대 비용 신장 트리 (주간반 출발 노드 = 7)
 *  3. Dijkstra: 시작 노드 -> 다른 모든 노드의 최단 경로
 *  4. Floyd   : 두 노드 사이 최단 경로 + 모든 노드 쌍의 최단 경로
 *
 *  - 노드 번호는 1번부터 출력한다. (배열 인덱스는 0번부터)
 *  - 파일에서 999 는 간선이 없음을 뜻한다.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0
#define NONE  999        /* 파일에서 간선이 없음을 나타내는 값 */
#define INF   1000000    /* 최단 경로 계산에서 사용하는 무한대 */

int num_node;            /* 노드의 개수 */
int **weight;            /* 인접 행렬 weight[i][j] (동적 할당) */


/* ================================================================== */
/*  1. Kruskal 알고리즘 (강의자료 : union-find, struct Edge, qsort)      */
/* ================================================================== */

int *parent;             /* 부모 노드 (-1 이면 자기가 루트) */

void set_init(int n)
{
	int i;
	for (i = 0; i < n; i++)
		parent[i] = -1;
}

/* curr 가 속하는 집합(루트)을 반환한다. */
int set_find(int curr)
{
	if (parent[curr] == -1)
		return curr;                       /* 루트 */
	while (parent[curr] != -1) curr = parent[curr];
	return curr;
}

/* 두 개의 원소가 속한 집합을 합친다. */
void set_union(int a, int b)
{
	int root1 = set_find(a);               /* 노드 a의 루트를 찾는다. */
	int root2 = set_find(b);               /* 노드 b의 루트를 찾는다. */
	if (root1 != root2)                    /* 합한다. */
		parent[root1] = root2;
}

struct Edge {                              /* 간선을 나타내는 구조체 */
	int start, end, weight;
};

/* qsort()에 사용되는 함수 : 가중치 오름차순 (최소비용) */
int compare_min(const void *a, const void *b)
{
	struct Edge *x = (struct Edge *)a;
	struct Edge *y = (struct Edge *)b;
	return (x->weight - y->weight);
}

/* qsort()에 사용되는 함수 : 가중치 내림차순 (최대비용) */
int compare_max(const void *a, const void *b)
{
	struct Edge *x = (struct Edge *)a;
	struct Edge *y = (struct Edge *)b;
	return (y->weight - x->weight);
}

/* type : 0 이면 최소비용, 1 이면 최대비용 신장 트리 */
void kruskal(int type)
{
	struct Edge *edges;
	int edge_count = 0;                    /* 그래프에 있는 간선의 수 */
	int edge_accepted = 0;                 /* 현재까지 선택된 간선의 수 */
	int uset, vset, i, j, sum = 0;
	struct Edge e;

	/* 무방향 그래프이므로 i < j 인 칸만 보면서 간선을 모은다. */
	edges = (struct Edge *)malloc(sizeof(struct Edge) * num_node * num_node);
	for (i = 0; i < num_node; i++)
		for (j = i + 1; j < num_node; j++)
			if (weight[i][j] != NONE) {
				edges[edge_count].start = i;
				edges[edge_count].end = j;
				edges[edge_count].weight = weight[i][j];
				edge_count++;
			}

	parent = (int *)malloc(sizeof(int) * num_node);
	set_init(num_node);

	if (type == 0) {
		qsort(edges, edge_count, sizeof(struct Edge), compare_min);
		printf("\n크루스칼 최소 비용 신장 트리 알고리즘\n");
	} else {
		qsort(edges, edge_count, sizeof(struct Edge), compare_max);
		printf("\n크루스칼 최대 비용 신장 트리 알고리즘\n");
	}

	i = 0;
	while (edge_accepted < num_node - 1 && i < edge_count) {  /* 신장트리 간선의 수 = (n-1) */
		e = edges[i];
		uset = set_find(e.start);          /* 정점 u의 집합 번호 */
		vset = set_find(e.end);            /* 정점 v의 집합 번호 */
		if (uset != vset) {                /* 서로 속한 집합이 다르면 (사이클이 안 생기면) */
			printf("간선 (%d,%d) %d 선택\n", e.start + 1, e.end + 1, e.weight);
			sum += e.weight;
			edge_accepted++;
			set_union(uset, vset);         /* 두개의 집합을 합친다. */
		}
		i++;
	}

	if (edge_accepted < num_node - 1) {
		if (type == 0) printf("최소비용신장트리를 구할 수 없다\n");
		else           printf("최대비용신장트리를 구할 수 없다\n");
	} else {
		printf("가중치 합 = %d\n", sum);
	}

	free(edges);
	free(parent);
}


/* ================================================================== */
/*  2. Prim 알고리즘 (강의자료 : selected[], distance[], get_min_vertex)  */
/* ================================================================== */

int *selected;           /* 신장 트리에 포함되었는지 표시 */
int *distance;           /* 신장 트리까지의 최소(최대) 가중치 */
int *from;               /* distance 값을 만든 트리 쪽 정점 (간선 출력용) */

/* 선택되지 않은 정점 중 distance 가 가장 작은 정점을 반환 */
int get_min_vertex(int n)
{
	int v = -1, i;
	for (i = 0; i < n; i++)
		if (!selected[i] && distance[i] != INF)
			if (v == -1 || distance[i] < distance[v]) v = i;
	return v;            /* -1 이면 더 이상 연결된 정점이 없음 */
}

/* 선택되지 않은 정점 중 distance 가 가장 큰 정점을 반환 */
int get_max_vertex(int n)
{
	int v = -1, i;
	for (i = 0; i < n; i++)
		if (!selected[i] && distance[i] != -1)
			if (v == -1 || distance[i] > distance[v]) v = i;
	return v;
}

/* s : 출발 정점, type : 0 최소 / 1 최대 */
void prim(int s, int type)
{
	int i, u, v, sum = 0;
	int no_value = (type == 0) ? INF : -1;   /* 아직 연결 간선이 없을 때의 값 */

	selected = (int *)malloc(sizeof(int) * num_node);
	distance = (int *)malloc(sizeof(int) * num_node);
	from     = (int *)malloc(sizeof(int) * num_node);

	for (u = 0; u < num_node; u++) {
		distance[u] = no_value;
		selected[u] = FALSE;
		from[u] = -1;
	}
	distance[s] = 0;

	if (type == 0) printf("\n프림 최소 비용 신장 트리 알고리즘 (출발 노드 %d)\n", s + 1);
	else           printf("\n프림 최대 비용 신장 트리 알고리즘 (출발 노드 %d)\n", s + 1);

	for (i = 0; i < num_node; i++) {
		if (type == 0) u = get_min_vertex(num_node);
		else           u = get_max_vertex(num_node);

		if (u == -1) {                     /* 연결되지 않은 정점이 남아 있음 */
			if (type == 0) printf("최소비용신장트리를 구할 수 없다\n");
			else           printf("최대비용신장트리를 구할 수 없다\n");
			free(selected); free(distance); free(from);
			return;
		}

		selected[u] = TRUE;
		if (from[u] != -1) {               /* 출발 정점이 아니면 간선 출력 */
			printf("간선 (%d,%d) %d 선택\n", from[u] + 1, u + 1, weight[from[u]][u]);
			sum += weight[from[u]][u];
		}

		/* 새로 들어온 u 와 인접한 정점들의 distance 갱신 */
		for (v = 0; v < num_node; v++) {
			if (selected[v] || weight[u][v] == NONE || u == v) continue;
			if ((type == 0 && weight[u][v] < distance[v]) ||
			    (type == 1 && weight[u][v] > distance[v])) {
				distance[v] = weight[u][v];
				from[v] = u;               /* tree[v] <- u */
			}
		}
	}
	printf("가중치 합 = %d\n", sum);

	free(selected); free(distance); free(from);
}


/* ================================================================== */
/*  3. Dijkstra 최단 경로 (강의자료 : distance[], found[], prev[], choose) */
/* ================================================================== */

int *found;              /* 최단 경로가 확정된 정점 표시 */
int *prev;               /* 최단 경로에서 바로 앞 정점 */

/* 확정되지 않은 정점 중 distance 가 가장 작은 정점 */
int choose(int distance[], int n, int found[])
{
	int i, min, minpos;
	min = INF;
	minpos = -1;
	for (i = 0; i < n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

/* prev[] 를 거꾸로 따라가며 경로를 출력 (재귀) */
void print_path(int v)
{
	if (prev[v] != -1) {
		print_path(prev[v]);
		printf(" -> ");
	}
	printf("%d", v + 1);
}

void shortest_path(int start)
{
	int i, u, w;

	distance = (int *)malloc(sizeof(int) * num_node);
	found    = (int *)malloc(sizeof(int) * num_node);
	prev     = (int *)malloc(sizeof(int) * num_node);

	for (i = 0; i < num_node; i++) {       /* 초기화 */
		if (weight[start][i] == NONE) {
			distance[i] = INF;
			prev[i] = -1;
		} else {
			distance[i] = weight[start][i];
			prev[i] = start;
		}
		found[i] = FALSE;
	}
	found[start] = TRUE;                   /* 시작 정점 방문 표시 */
	distance[start] = 0;
	prev[start] = -1;

	for (i = 0; i < num_node - 1; i++) {
		u = choose(distance, num_node, found);
		if (u == -1) break;                /* 더 이상 갈 수 있는 정점이 없음 */
		found[u] = TRUE;
		for (w = 0; w < num_node; w++)
			if (!found[w] && weight[u][w] != NONE)
				if (distance[u] + weight[u][w] < distance[w]) {
					distance[w] = distance[u] + weight[u][w];
					prev[w] = u;           /* 경로 저장 */
				}
	}

	printf("\nDijkstra 최단 경로 (시작 노드 %d)\n", start + 1);
	for (i = 0; i < num_node; i++) {
		if (i == start) continue;
		if (distance[i] == INF) {
			printf("%d -> %d : 경로 없음\n", start + 1, i + 1);
		} else {
			printf("%d -> %d : 길이 %d,  경로 : ", start + 1, i + 1, distance[i]);
			print_path(i);
			printf("\n");
		}
	}

	free(distance); free(found); free(prev);
}


/* ================================================================== */
/*  4. Floyd 최단 경로 (강의자료 : A[i][j], next[i][j])                  */
/* ================================================================== */

int **A;                 /* A[i][j]    : i 에서 j 까지 최단 거리 */
int **next;              /* next[i][j] : i 에서 j 로 갈 때 거쳐가는 정점 k (-1 이면 바로 연결) */

/* i 와 j 사이에서 거쳐가는 정점들을 출력 : Path(i,k) + k + Path(k,j) */
void floyd_path(int i, int j)
{
	int k = next[i][j];
	if (k == -1) return;                   /* 바로 연결되어 있으면 중간 정점 없음 */
	floyd_path(i, k);
	printf(" -> %d", k + 1);
	floyd_path(k, j);
}

void floyd(void)
{
	int i, j, k;

	A    = (int **)malloc(sizeof(int *) * num_node);
	next = (int **)malloc(sizeof(int *) * num_node);
	for (i = 0; i < num_node; i++) {
		A[i]    = (int *)malloc(sizeof(int) * num_node);
		next[i] = (int *)malloc(sizeof(int) * num_node);
	}

	for (i = 0; i < num_node; i++)         /* 배열 A의 초기값 = weight */
		for (j = 0; j < num_node; j++) {
			if (weight[i][j] == NONE) A[i][j] = INF;
			else                      A[i][j] = weight[i][j];
			next[i][j] = -1;
		}

	for (k = 0; k < num_node; k++)
		for (i = 0; i < num_node; i++)
			for (j = 0; j < num_node; j++)
				if (A[i][k] + A[k][j] < A[i][j]) {
					A[i][j] = A[i][k] + A[k][j];
					next[i][j] = k;        /* k 를 거쳐가는 것이 더 짧음 */
				}
}

void floyd_menu(void)
{
	int s, d, i, j;

	floyd();

	printf("출발 노드와 도착 노드를 입력하세요.(1 ~ %d)\n--> ", num_node);
	scanf("%d %d", &s, &d);
	s--; d--;                              /* 1번부터 -> 0번부터 */

	printf("\nFloyd 최단 경로\n");
	if (s < 0 || s >= num_node || d < 0 || d >= num_node) {
		printf("잘못된 노드 번호입니다.\n");
	} else if (A[s][d] >= INF) {
		printf("%d -> %d : 경로 없음\n", s + 1, d + 1);
	} else {
		printf("%d -> %d : 길이 %d,  경로 : %d", s + 1, d + 1, A[s][d], s + 1);
		floyd_path(s, d);
		if (s != d) printf(" -> %d", d + 1);
		printf("\n");
	}

	/* 모든 노드 쌍의 최단 경로 길이 (표) */
	printf("\n모든 노드 쌍의 최단 경로 길이\n    ");
	for (j = 0; j < num_node; j++) printf("%4d", j + 1);
	printf("\n");
	for (i = 0; i < num_node; i++) {
		printf("%4d", i + 1);
		for (j = 0; j < num_node; j++) {
			if (A[i][j] >= INF) printf("   *");
			else                printf("%4d", A[i][j]);
		}
		printf("\n");
	}

	/* 모든 노드 쌍의 최단 경로 */
	printf("\n모든 노드 쌍의 최단 경로\n");
	for (i = 0; i < num_node; i++)
		for (j = 0; j < num_node; j++) {
			if (i == j) continue;
			if (A[i][j] >= INF) {
				printf("%d -> %d : 경로 없음\n", i + 1, j + 1);
				continue;
			}
			printf("%d -> %d : 길이 %2d,  경로 : %d", i + 1, j + 1, A[i][j], i + 1);
			floyd_path(i, j);
			printf(" -> %d\n", j + 1);
		}

	for (i = 0; i < num_node; i++) {
		free(A[i]);
		free(next[i]);
	}
	free(A);
	free(next);
}


/* ================================================================== */
/*  main                                                               */
/* ================================================================== */

/* 1 ~ num_node 사이의 노드 번호를 입력받아 0부터 시작하는 번호로 반환 */
int input_node(void)
{
	int node;
	while (1) {
		printf("출발 노드를 선택하세요.(1 ~ %d)\n--> ", num_node);
		scanf("%d", &node);
		if (node >= 1 && node <= num_node) return node - 1;
		printf("잘못된 노드 번호입니다.\n");
	}
}

/* 0(최소) 또는 1(최대) 입력 */
int input_type(void)
{
	int type;
	while (1) {
		printf("Min = 0, Max = 1\n--> ");
		scanf("%d", &type);
		if (type == 0 || type == 1) return type;
		printf("0또는 1이 아닙니다. 다시 입력해주세요\n");
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char filename[100];
	int i, j, menu;

	/* 명령행 인자로 파일 이름을 받는다. (없으면 직접 입력) */
	if (argc == 2) {
		fp = fopen(argv[1], "r");
	} else {
		printf("<Usage>: graph graph.txt\n");
		printf("파일 이름을 입력하세요 : ");
		scanf("%s", filename);
		fp = fopen(filename, "r");
	}
	if (fp == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return 0;
	}

	/* 노드 개수를 읽고 인접 행렬을 동적 할당한다. */
	fscanf(fp, "%d", &num_node);
	weight = (int **)malloc(sizeof(int *) * num_node);
	for (i = 0; i < num_node; i++)
		weight[i] = (int *)malloc(sizeof(int) * num_node);

	for (i = 0; i < num_node; i++)
		for (j = 0; j < num_node; j++)
			fscanf(fp, "%d", &weight[i][j]);
	fclose(fp);

	printf("\n노드 개수 : %d\n", num_node);
	for (i = 0; i < num_node; i++) {
		for (j = 0; j < num_node; j++)
			printf("%4d", weight[i][j]);
		printf("\n");
	}

	while (1) {
		printf("\n-------------------------------------\n");
		printf("1. Kruskal MST\n");
		printf("2. Prim MST\n");
		printf("3. Dijkstra SP(Shortest Path)\n");
		printf("4. Floyd SP(Shortest Path)\n");
		printf("5. Exit\n");
		printf("-------------------------------------\n");
		printf("원하는 메뉴의 번호를 입력하세요.\n--> ");
		if (scanf("%d", &menu) != 1) break;

		if (menu == 1) {
			kruskal(input_type());
		} else if (menu == 2) {
			int type = input_type();
			prim(input_node(), type);
		} else if (menu == 3) {
			shortest_path(input_node());
		} else if (menu == 4) {
			floyd_menu();
		} else if (menu == 5) {
			break;
		} else {
			printf("1 ~ 5 사이의 번호를 입력하세요.\n");
		}
	}

	for (i = 0; i < num_node; i++)
		free(weight[i]);
	free(weight);
	return 0;
}