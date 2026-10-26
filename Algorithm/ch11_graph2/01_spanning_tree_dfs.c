/* 11장 그래프 II - 신장 트리 알고리즘 (DFS 이용)
 *
 * depth_first_search(v):
 *     v를 방문되었다고 표시;
 *     for all u ∈ (v에 인접한 정점) do
 *         if (u가 아직 방문되지 않았으면)
 *             then (v,u)를 신장 트리 간선이라고 표시;
 *                  depth_first_search(u)
 */
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 50
typedef struct GraphType {
	int n;
	int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int visited[MAX_VERTICES];

void init(GraphType* g)
{
	int r, c;
	g->n = 0;
	for (r = 0; r < MAX_VERTICES; r++)
		for (c = 0; c < MAX_VERTICES; c++)
			g->adj_mat[r][c] = 0;
}

void insert_vertex(GraphType* g, int v)
{
	if (((g->n) + 1) > MAX_VERTICES) {
		fprintf(stderr, "그래프: 정점의 개수 초과");
		return;
	}
	g->n++;
}

void insert_edge(GraphType* g, int start, int end)
{
	if (start >= g->n || end >= g->n) {
		fprintf(stderr, "그래프: 정점 번호 오류");
		return;
	}
	g->adj_mat[start][end] = 1;
	g->adj_mat[end][start] = 1;
}

void dfs_spanning_tree(GraphType* g, int v)
{
	int u;
	visited[v] = TRUE;
	for (u = 0; u < g->n; u++)
		if (g->adj_mat[v][u] && !visited[u]) {
			printf("신장 트리 간선 (%d, %d)\n", v, u);
			dfs_spanning_tree(g, u);
		}
}

int main(void)
{
	GraphType *g;
	int i;
	g = (GraphType *)malloc(sizeof(GraphType));
	init(g);
	for (i = 0; i < 4; i++)
		insert_vertex(g, i);
	insert_edge(g, 0, 1);
	insert_edge(g, 0, 2);
	insert_edge(g, 0, 3);
	insert_edge(g, 1, 2);
	insert_edge(g, 2, 3);

	dfs_spanning_tree(g, 0);
	free(g);
	return 0;
}
