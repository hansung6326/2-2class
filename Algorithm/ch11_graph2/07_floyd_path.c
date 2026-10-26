/* 11장 그래프 II - Floyd 최단 "경로" 알고리즘 (next[i][j] 로 경로 저장)
 *
 *  for k, i, j :
 *      if dist[i][k] + dist[k][j] < dist[i][j] then
 *          dist[i][j] <- dist[i][k] + dist[k][j]
 *          next[i][j] <- k
 *
 *  Path(i, j):
 *      if dist[i][j] = INF then return "no path"
 *      intermediate <- next[i][j]
 *      if intermediate = null then return " "   // i에서 j로 바로 가는 간선
 *      else return Path(i, intermediate) + intermediate + Path(intermediate, j)
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define INF 1000000

typedef struct GraphType {
	int n;
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int A[MAX_VERTICES][MAX_VERTICES];
int next[MAX_VERTICES][MAX_VERTICES];	/* -1 이면 바로 연결 (null) */

void floyd(GraphType* g)
{
	int i, j, k;
	for (i = 0; i < g->n; i++)
		for (j = 0; j < g->n; j++) {
			A[i][j] = g->weight[i][j];
			next[i][j] = -1;
		}

	for (k = 0; k < g->n; k++)
		for (i = 0; i < g->n; i++)
			for (j = 0; j < g->n; j++)
				if (A[i][k] + A[k][j] < A[i][j]) {
					A[i][j] = A[i][k] + A[k][j];
					next[i][j] = k;
				}
}

/* i와 j 사이에 거쳐가는 정점들을 출력 */
void Path(int i, int j)
{
	int intermediate = next[i][j];
	if (intermediate == -1) return;		/* i -> j 직접 연결 */
	Path(i, intermediate);
	printf(" -> %d", intermediate);
	Path(intermediate, j);
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
	int i, j;

	floyd(&g);

	printf("next[][] (N6)\n");
	for (i = 0; i < g.n; i++) {
		for (j = 0; j < g.n; j++) printf("%3d", next[i][j]);
		printf("\n");
	}

	printf("\n모든 정점 쌍의 최단 경로\n");
	for (i = 0; i < g.n; i++)
		for (j = 0; j < g.n; j++) {
			if (i == j) continue;
			if (A[i][j] == INF) {
				printf("%d -> %d : no path\n", i, j);
				continue;
			}
			printf("%d -> %d : 거리 %2d, 경로 : %d", i, j, A[i][j], i);
			Path(i, j);
			printf(" -> %d\n", j);
		}
	return 0;
}
