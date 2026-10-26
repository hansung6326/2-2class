/* 10장 그래프 I - 연결 성분 찾기
 * DFS 탐색 프로그램의 visited[v] = TRUE; 를 visited[v] = count; 로 교체
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50
typedef struct GraphType {
	int n;
	int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int visited[MAX_VERTICES];	// 0 이면 방문하지 않음, 1 이상이면 연결 성분 번호
int count;

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

void dfs_mat(GraphType* g, int v)
{
	int w;
	visited[v] = count;			// 정점 v에 연결 성분 번호 표시
	for (w = 0; w < g->n; w++)
		if (g->adj_mat[v][w] && !visited[w])
			dfs_mat(g, w);
}

void find_connected_component(GraphType *g)
{
	int i;
	count = 0;
	for (i = 0; i < g->n; i++)
		if (!visited[i]) {		// 방문되지 않았으면
			count++;
			dfs_mat(g, i);
		}
}

int main(void)
{
	GraphType *g;
	int i;
	g = (GraphType *)malloc(sizeof(GraphType));
	init(g);
	for (i = 0; i < 5; i++)
		insert_vertex(g, i);
	insert_edge(g, 0, 1);
	insert_edge(g, 0, 2);
	insert_edge(g, 3, 4);

	find_connected_component(g);
	printf("연결 성분의 개수 = %d\n", count);
	printf("정점  visited\n");
	for (i = 0; i < g->n; i++)
		printf("  %d      %d\n", i, visited[i]);
	free(g);
	return 0;
}
