/* 11장 그래프 II - Dijkstra의 최단 "경로" 알고리즘 (prev[] 로 경로 저장)
 *
 *  prev[v] = -1;            // 경로 저장을 위해 사용
 *  ...
 *  if distance[u]+weight[u][z] < distance[z]
 *      then distance[z] <- distance[u]+weight[u][z];
 *           prev[z] = u;
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100
#define INF 1000000

typedef struct GraphType {
	int n;
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int distance[MAX_VERTICES];
int found[MAX_VERTICES];
int prev[MAX_VERTICES];		/* 최단거리로 u에 이르게 된 직전 정점 */

int choose(int distance[], int n, int found[])
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i < n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void shortest_path(GraphType* g, int start)
{
	int i, u, w;
	for (i = 0; i < g->n; i++) {
		distance[i] = g->weight[start][i];
		found[i] = FALSE;
		prev[i] = (g->weight[start][i] != INF) ? start : -1;
	}
	found[start] = TRUE;
	distance[start] = 0;
	prev[start] = -1;
	for (i = 0; i < g->n - 1; i++) {
		u = choose(distance, g->n, found);
		if (u == -1) break;
		found[u] = TRUE;
		for (w = 0; w < g->n; w++)
			if (!found[w])
				if (distance[u] + g->weight[u][w] < distance[w]) {
					distance[w] = distance[u] + g->weight[u][w];
					prev[w] = u;
				}
	}
}

/* prev[]를 거꾸로 따라가며 경로 출력 (재귀) */
void print_path(int v)
{
	if (prev[v] != -1) {
		print_path(prev[v]);
		printf(" -> ");
	}
	printf("%d", v);
}

int main(void)
{
	GraphType g = { 7,
	{{ 0,  7,  INF, INF,   3,  10, INF },
	{ 7,  0,    4,  10,   2,   6, INF },
	{ INF,  4,    0,   2, INF, INF, INF },
	{ INF, 10,    2,   0,  11,   9,   4 },
	{ 3,  2,  INF,  11,   0, INF,   5 },
	{ 10,  6,  INF,   9, INF,   0, INF },
	{ INF, INF, INF,   4,   5, INF,   0 } }
	};
	int i;

	shortest_path(&g, 0);
	printf("prev[] = ");
	for (i = 0; i < g.n; i++) printf("%d ", prev[i]);
	printf("\n\n");
	for (i = 0; i < g.n; i++) {
		printf("0 -> %d : 거리 %2d, 경로 : ", i, distance[i]);
		print_path(i);
		printf("\n");
	}
	return 0;
}
