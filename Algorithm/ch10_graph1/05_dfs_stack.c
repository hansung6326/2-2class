/* 10장 그래프 I - 깊이 우선 탐색(DFS) : 명시적 스택 사용 버전
 *
 * DFS-iterative(G, v):
 *   스택 S를 생성한다.
 *   S.push(v)
 *   while (not is_empty(S)) do
 *       v = S.pop()
 *       if (v가 방문되지 않았으면)
 *           v를 방문되었다고 표시
 *           for all u ∈ (v에 인접한 정점) do
 *               if (u가 아직 방문되지 않았으면)
 *                   S.push(u)
 */
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 50
#define MAX_STACK_SIZE 100

typedef struct GraphType {
	int n;
	int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;

/* ===== 스택 ===== */
typedef struct {
	int data[MAX_STACK_SIZE];
	int top;
} StackType;

void init_stack(StackType *s) { s->top = -1; }
int is_empty(StackType *s) { return (s->top == -1); }
void push(StackType *s, int item)
{
	if (s->top >= MAX_STACK_SIZE - 1) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	s->data[++(s->top)] = item;
}
int pop(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	return s->data[(s->top)--];
}

/* ===== 그래프 ===== */
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

// 명시적 스택을 이용한 깊이 우선 탐색
void dfs_iterative(GraphType* g, int v)
{
	StackType s;
	int u;
	init_stack(&s);
	push(&s, v);
	while (!is_empty(&s)) {
		v = pop(&s);
		if (!visited[v]) {
			visited[v] = TRUE;
			printf("정점 %d -> ", v);
			/* 번호가 작은 정점부터 방문하도록 큰 번호부터 push */
			for (u = g->n - 1; u >= 0; u--)
				if (g->adj_mat[v][u] && !visited[u])
					push(&s, u);
		}
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

	printf("깊이 우선 탐색(스택)\n");
	dfs_iterative(g, 0);
	printf("\n");
	free(g);
	return 0;
}
