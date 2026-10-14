/*
 * 알고리즘 그래프 프로그래밍 과제 (주간반)
 *  - 입력 : graph_B.txt (명령행 인자로 전달)   예) graph graph_B.txt
 *  - 1. Kruskal 알고리즘 : 최소/최대 비용 신장 트리
 *  - 2. Prim    알고리즘 : 최소/최대 비용 신장 트리 (주간반 출발 노드: 7)
 *  - 3. Dijkstra 최단 경로 : 시작 노드 -> 모든 노드 (길이 + 경로)
 *  - 4. Floyd    최단 경로 : 임의의 출발/도착 노드 쌍 (길이 + 경로)
 *                            + 모든 노드 쌍의 최단 경로 길이와 경로
 *  - 노드 번호는 1번부터, 999 는 간선이 없음을 의미
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* 한글 출력 설정                                                       */
/*  소스의 한글(UTF-8)을 유니코드로 바꿔 콘솔에 직접 출력하므로            */
/*  콘솔 코드페이지(CP949/UTF-8)나 컴파일러(gcc/Visual Studio)와 상관없이  */
/*  한글이 깨지지 않는다.                                                */
/* ------------------------------------------------------------------ */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
#ifdef _WIN32
#include <windows.h>
#include <stdarg.h>

int u8printf(const char *fmt, ...)
{
	static char    buf[8192];
	static wchar_t wbuf[8192];
	va_list ap;
	int n, wn;
	DWORD mode, written;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	va_start(ap, fmt);
	n = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (GetConsoleMode(h, &mode)) {          /* 콘솔 창이면 유니코드로 출력 */
		wn = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, 8192);
		if (wn > 1)
			WriteConsoleW(h, wbuf, (DWORD)(wn - 1), &written, NULL);
	} else {                                 /* 파일로 리다이렉트된 경우 */
		fputs(buf, stdout);
	}
	return n;
}
#define printf u8printf
#endif

#define NO_EDGE  999        /* 파일에서 간선 없음을 나타내는 값 */
#define INF      1000000    /* 최단 경로 계산용 무한대 */
#define MIN_TREE 0
#define MAX_TREE 1

typedef struct {
	int u, v;      /* 노드 번호 (0부터, 출력할 때 +1) */
	int w;         /* 가중치 */
} Edge;

int num_node;      /* 노드 개수 */
int *weight;       /* 인접 행렬 (num_node * num_node, 동적 할당) */

#define W(i, j) weight[(i) * num_node + (j)]

/* 간선이 존재하는지 검사 (자기 자신, 999 는 간선이 아님) */
int has_edge(int i, int j)
{
	return i != j && W(i, j) != NO_EDGE;
}

/* ------------------------------------------------------------------ */
/* 입력 보조 함수                                                       */
/* ------------------------------------------------------------------ */
void clear_input(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

/* 창이 바로 닫히지 않도록 Enter 를 기다린 뒤 종료 */
void pause_exit(void)
{
	printf("\n종료하려면 Enter 키를 누르세요...");
	fflush(stdout);
	getchar();
}

/* 최소(0) / 최대(1) 선택 */
int input_min_max(void)
{
	int sel;
	while (1) {
		printf("Min = 0, Max = 1\n--> ");
		if (scanf("%d", &sel) == 1 && (sel == MIN_TREE || sel == MAX_TREE))
			return sel;
		printf("0또는 1이 아닙니다. 다시 입력해주세요\n");
		clear_input();
	}
}

/* 1 ~ num_node 범위의 노드 번호 입력, 0부터 시작하는 인덱스로 반환 */
int input_node(const char *msg)
{
	int node;
	while (1) {
		printf("%s(1 ~ %d)\n--> ", msg, num_node);
		if (scanf("%d", &node) == 1 && node >= 1 && node <= num_node)
			return node - 1;
		printf("잘못된 노드 번호입니다. 다시 입력해주세요\n");
		clear_input();
	}
}

const char *tree_name(int type)
{
	return type == MIN_TREE ? "최소비용신장트리" : "최대비용신장트리";
}

/* ------------------------------------------------------------------ */
/* 1. Kruskal 알고리즘 (Union-Find 사용)                                 */
/* ------------------------------------------------------------------ */
int sort_type;     /* qsort 비교 함수에서 사용할 정렬 방향 */

int compare_edge(const void *a, const void *b)
{
	const Edge *x = (const Edge *)a;
	const Edge *y = (const Edge *)b;
	if (x->w != y->w)
		return sort_type == MIN_TREE ? x->w - y->w : y->w - x->w;
	if (x->u != y->u) return x->u - y->u;
	return x->v - y->v;
}

/* parent[i] == -1 이면 i 가 집합의 대표(루트) */
int set_find(int *parent, int i)
{
	while (parent[i] != -1)
		i = parent[i];
	return i;
}

void set_union(int *parent, int a, int b)
{
	parent[b] = a;          /* 루트 b 를 루트 a 밑으로 붙인다 */
}

void kruskal(int type)
{
	Edge *edges, *tree;
	int *parent;
	int edge_cnt = 0, accepted = 0, sum = 0;
	int i, j, ru, rv;

	edges  = (Edge *)malloc(sizeof(Edge) * num_node * num_node);
	tree   = (Edge *)malloc(sizeof(Edge) * num_node);
	parent = (int *)malloc(sizeof(int) * num_node);

	/* 무방향 그래프이므로 i < j 인 간선만 수집 */
	for (i = 0; i < num_node; i++)
		for (j = i + 1; j < num_node; j++)
			if (has_edge(i, j)) {
				edges[edge_cnt].u = i;
				edges[edge_cnt].v = j;
				edges[edge_cnt].w = W(i, j);
				edge_cnt++;
			}

	/* 최소 : 오름차순, 최대 : 내림차순 정렬 */
	sort_type = type;
	qsort(edges, edge_cnt, sizeof(Edge), compare_edge);

	for (i = 0; i < num_node; i++)
		parent[i] = -1;

	for (i = 0; i < edge_cnt && accepted < num_node - 1; i++) {
		ru = set_find(parent, edges[i].u);
		rv = set_find(parent, edges[i].v);
		if (ru != rv) {                 /* 사이클이 생기지 않으면 채택 */
			set_union(parent, ru, rv);
			tree[accepted++] = edges[i];
		}
	}

	printf("\n[Kruskal %s]\n", tree_name(type));
	if (accepted != num_node - 1) {
		printf("%s를 구할 수 없다\n", tree_name(type));
	} else {
		for (i = 0; i < accepted; i++) {
			printf("(%d,%d) %d \n", tree[i].u + 1, tree[i].v + 1, tree[i].w);
			sum += tree[i].w;
		}
		printf("Total sum = %d\n", sum);
	}

	free(edges);
	free(tree);
	free(parent);
}

/* ------------------------------------------------------------------ */
/* 2. Prim 알고리즘                                                     */
/* ------------------------------------------------------------------ */
void prim(int type, int start)
{
	int *selected, *key, *from;
	Edge *tree;
	int i, k, u, cnt = 0, sum = 0;

	selected = (int *)malloc(sizeof(int) * num_node);
	key      = (int *)malloc(sizeof(int) * num_node);   /* 트리까지의 최선 가중치 */
	from     = (int *)malloc(sizeof(int) * num_node);   /* 그 간선의 트리 쪽 노드 */
	tree     = (Edge *)malloc(sizeof(Edge) * num_node);

	for (i = 0; i < num_node; i++) {
		selected[i] = 0;
		key[i] = -1;          /* -1 : 아직 트리와 연결되는 간선이 없음 */
		from[i] = -1;
	}
	key[start] = 0;

	for (k = 0; k < num_node; k++) {
		/* 트리에 없는 노드 중 key 가 가장 작은(최대 트리면 가장 큰) 노드 선택 */
		u = -1;
		for (i = 0; i < num_node; i++) {
			if (selected[i] || key[i] == -1) continue;
			if (u == -1 ||
			    (type == MIN_TREE && key[i] < key[u]) ||
			    (type == MAX_TREE && key[i] > key[u]))
				u = i;
		}
		if (u == -1) break;   /* 더 이상 연결할 수 있는 노드가 없음 */

		selected[u] = 1;
		if (from[u] != -1) {
			tree[cnt].u = from[u];
			tree[cnt].v = u;
			tree[cnt].w = W(from[u], u);
			cnt++;
		}

		/* 새로 들어온 u 를 통해 key 값 갱신 */
		for (i = 0; i < num_node; i++) {
			if (selected[i] || !has_edge(u, i)) continue;
			if (key[i] == -1 ||
			    (type == MIN_TREE && W(u, i) < key[i]) ||
			    (type == MAX_TREE && W(u, i) > key[i])) {
				key[i] = W(u, i);
				from[i] = u;
			}
		}
	}

	printf("\n[Prim %s] 출발 노드 : %d\n", tree_name(type), start + 1);
	if (cnt != num_node - 1) {
		printf("%s를 구할 수 없다\n", tree_name(type));
	} else {
		for (i = 0; i < cnt; i++) {
			printf("(%d,%d) %d \n", tree[i].u + 1, tree[i].v + 1, tree[i].w);
			sum += tree[i].w;
		}
		printf("Total sum = %d\n", sum);
	}

	free(selected);
	free(key);
	free(from);
	free(tree);
}

/* ------------------------------------------------------------------ */
/* 3. Dijkstra 최단 경로                                                */
/* ------------------------------------------------------------------ */
/* prev[] 를 따라 거꾸로 올라가며 경로를 출력 (재귀) */
void print_dijkstra_path(int *prev, int v)
{
	if (prev[v] != -1) {
		print_dijkstra_path(prev, prev[v]);
		printf(" -> ");
	}
	printf("%d", v + 1);
}

void dijkstra(int start)
{
	int *dist, *found, *prev;
	int i, k, u;

	dist  = (int *)malloc(sizeof(int) * num_node);
	found = (int *)malloc(sizeof(int) * num_node);
	prev  = (int *)malloc(sizeof(int) * num_node);   /* 경로상 직전 노드 */

	for (i = 0; i < num_node; i++) {
		dist[i] = has_edge(start, i) ? W(start, i) : INF;
		prev[i] = has_edge(start, i) ? start : -1;
		found[i] = 0;
	}
	dist[start] = 0;
	prev[start] = -1;
	found[start] = 1;

	for (k = 0; k < num_node - 1; k++) {
		/* 아직 확정되지 않은 노드 중 거리가 가장 짧은 노드 */
		u = -1;
		for (i = 0; i < num_node; i++)
			if (!found[i] && dist[i] < INF && (u == -1 || dist[i] < dist[u]))
				u = i;
		if (u == -1) break;
		found[u] = 1;

		for (i = 0; i < num_node; i++)
			if (!found[i] && has_edge(u, i) && dist[u] + W(u, i) < dist[i]) {
				dist[i] = dist[u] + W(u, i);
				prev[i] = u;
			}
	}

	printf("\n[Dijkstra 최단 경로] 시작 노드 : %d\n", start + 1);
	for (i = 0; i < num_node; i++) {
		if (i == start) continue;
		if (dist[i] >= INF) {
			printf("%d -> %d : No path!\n", start + 1, i + 1);
			continue;
		}
		printf("%d -> %d : Dist[%d] = %d \t경로 : ", start + 1, i + 1, i + 1, dist[i]);
		print_dijkstra_path(prev, i);
		printf("\n");
	}

	free(dist);
	free(found);
	free(prev);
}

/* ------------------------------------------------------------------ */
/* 4. Floyd 최단 경로                                                   */
/* ------------------------------------------------------------------ */
int *A;      /* A[i][j]   : i -> j 최단 거리 */
int *via;    /* via[i][j] : i -> j 최단 경로가 거쳐가는 중간 노드 (-1 이면 직접 연결) */

#define AA(i, j)  A[(i) * num_node + (j)]
#define VIA(i, j) via[(i) * num_node + (j)]

/* i 와 j 사이의 중간 노드들을 순서대로 출력 (i, j 자신은 제외) */
void print_floyd_middle(int i, int j)
{
	int k = VIA(i, j);
	if (k == -1) return;
	print_floyd_middle(i, k);
	printf(" -> %d", k + 1);
	print_floyd_middle(k, j);
}

void print_floyd_path(int i, int j)
{
	if (AA(i, j) >= INF) {
		printf("No path!");
		return;
	}
	printf("%d", i + 1);
	print_floyd_middle(i, j);
	if (i != j) printf(" -> %d", j + 1);
}

void floyd(void)
{
	int i, j, k;

	A   = (int *)malloc(sizeof(int) * num_node * num_node);
	via = (int *)malloc(sizeof(int) * num_node * num_node);

	for (i = 0; i < num_node; i++)
		for (j = 0; j < num_node; j++) {
			if (i == j)               AA(i, j) = 0;
			else if (has_edge(i, j))  AA(i, j) = W(i, j);
			else                      AA(i, j) = INF;
			VIA(i, j) = -1;
		}

	/* k 를 거쳐가는 경로가 더 짧으면 그 값으로 갱신 */
	for (k = 0; k < num_node; k++)
		for (i = 0; i < num_node; i++)
			for (j = 0; j < num_node; j++)
				if (AA(i, k) < INF && AA(k, j) < INF &&
				    AA(i, k) + AA(k, j) < AA(i, j)) {
					AA(i, j) = AA(i, k) + AA(k, j);
					VIA(i, j) = k;
				}
}

void floyd_menu(void)
{
	int s, d, i, j;

	floyd();

	s = input_node("출발 노드를 입력하세요.");
	d = input_node("도착 노드를 입력하세요.");

	printf("\n[Floyd 최단 경로]\n");
	if (AA(s, d) >= INF) {
		printf("%d -> %d : No path!\n", s + 1, d + 1);
	} else {
		printf("%d -> %d : shortest path length (%d)\n", s + 1, d + 1, AA(s, d));
		printf("%d -> %d : shortest path : ", s + 1, d + 1);
		print_floyd_path(s, d);
		printf("\n");
	}

	/* 모든 노드 쌍의 최단 경로 길이 (행렬) */
	printf("\n[모든 노드 쌍의 최단 경로 길이]\n    ");
	for (j = 0; j < num_node; j++) printf("%5d", j + 1);
	printf("\n");
	for (i = 0; i < num_node; i++) {
		printf("%3d ", i + 1);
		for (j = 0; j < num_node; j++) {
			if (AA(i, j) >= INF) printf("%5s", "INF");
			else                 printf("%5d", AA(i, j));
		}
		printf("\n");
	}

	/* 모든 노드 쌍의 최단 경로 */
	printf("\n[모든 노드 쌍의 최단 경로]\n");
	for (i = 0; i < num_node; i++)
		for (j = 0; j < num_node; j++) {
			if (i == j) continue;
			if (AA(i, j) >= INF) {
				printf("%d -> %d : No path!\n", i + 1, j + 1);
				continue;
			}
			printf("%d -> %d : 길이 %3d, 경로 : ", i + 1, j + 1, AA(i, j));
			print_floyd_path(i, j);
			printf("\n");
		}

	free(A);
	free(via);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
int main(int argc, char *argv[])
{
	FILE *fp;
	int i, j, menu;
	char fname_buf[256];
	char *filename;

	if (argc == 2) {
		filename = argv[1];
	} else {
		/* 더블클릭 등으로 인자 없이 실행한 경우 : 파일 이름을 직접 입력받음 */
		printf("ERROR: <Usage> graph graph.txt\n");
		printf("그래프 파일 이름을 입력하세요 (예: graph_B.txt)\n--> ");
		if (scanf("%255s", fname_buf) != 1) {
			pause_exit();
			return 1;
		}
		clear_input();
		filename = fname_buf;
	}
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("ERROR: %s 파일을 열 수 없습니다. (exe 와 같은 폴더에 있는지 확인하세요)\n", filename);
		pause_exit();
		return 1;
	}

	/* 노드 개수를 읽고 인접 행렬을 동적 할당 */
	if (fscanf(fp, "%d", &num_node) != 1 || num_node <= 0) {
		printf("ERROR: 노드 개수를 읽을 수 없습니다.\n");
		fclose(fp);
		pause_exit();
		return 1;
	}
	weight = (int *)malloc(sizeof(int) * num_node * num_node);
	for (i = 0; i < num_node; i++)
		for (j = 0; j < num_node; j++)
			if (fscanf(fp, "%d", &W(i, j)) != 1) {
				printf("ERROR: 인접 행렬 데이터가 부족합니다.\n");
				free(weight);
				fclose(fp);
				pause_exit();
				return 1;
			}
	fclose(fp);

	printf("\n노드 개수 : %d\n", num_node);
	for (i = 0; i < num_node; i++) {
		for (j = 0; j < num_node; j++)
			printf("%4d", W(i, j));
		printf("\n");
	}

	while (1) {
		printf("\n-------------------------------------\n");
		printf("1. Kruskal MST                       \n");
		printf("2. Prim MST                          \n");
		printf("3. Dijkstra SP(Shortest Path)        \n");
		printf("4. Floyd SP(Shortest Path)           \n");
		printf("5. Exit                              \n");
		printf("-------------------------------------\n");
		printf("원하는 메뉴의 번호를 입력하세요.     \n--> ");

		if (scanf("%d", &menu) != 1) {
			if (feof(stdin)) break;
			clear_input();
			continue;
		}

		switch (menu) {
		case 1:
			kruskal(input_min_max());
			break;
		case 2: {
			int type = input_min_max();
			int start = input_node("출발 노드를 선택하세요.");
			prim(type, start);
			break;
		}
		case 3:
			dijkstra(input_node("출발 노드를 선택하세요."));
			break;
		case 4:
			floyd_menu();
			break;
		case 5:
			free(weight);
			return 0;
		default:
			printf("1 ~ 5 사이의 번호를 입력하세요.\n");
		}
	}

	free(weight);
	return 0;
}