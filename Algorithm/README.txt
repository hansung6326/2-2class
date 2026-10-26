강의자료(C로 쉽게 풀어쓴 자료구조) 프로그램 모음
=====================================================

■ 8장 트리 (ch08_tree)
  01_link_tree.c          링크 표현법 (n1, n2, n3 노드 연결)
  02_traversal.c          중위 / 전위 / 후위 순회 (순환 호출)
  03_traversal_iter.c     반복적인 순회 (스택 이용 : 중위, 전위, 후위)
  04_level_order.c        레벨 순회 (원형 큐 이용)
  05_expression_tree.c    수식 트리 계산 (결과 45)
  06_dir_size.c           디렉토리 용량 계산 (결과 850)
  07_tree_ops.c           노드 개수, 단말 노드 개수, 높이
  08_thread_tree.c        스레드 이진 트리 중위 순회
  09_bst.c                이진 탐색 트리 (탐색, 삽입, 삭제)
  10_bst_dictionary.c     이진 탐색 트리 응용 : 영어 사전 (i/d/s/p/q 메뉴)

■ 9장 우선순위 큐 (ch09_priority_queue)
  01_heap.c               최대 히프 삽입(upheap) / 삭제(downheap)
  02_heap_sort.c          히프 정렬
  03_lpt.c                머신 스케줄링 LPT 방법 (최소 히프)
  04_huffman.c            허프만 코드

■ 10장 그래프 I (ch10_graph1)
  01_adj_mat.c            인접 행렬 구현
  02_adj_list.c           인접 리스트 구현
  03_dfs_mat.c            깊이 우선 탐색 (인접 행렬)
  04_dfs_list.c           깊이 우선 탐색 (인접 리스트)
  05_dfs_stack.c          깊이 우선 탐색 (명시적 스택)
  06_bfs_mat.c            너비 우선 탐색 (인접 행렬)
  07_bfs_list.c           너비 우선 탐색 (인접 리스트)
  08_connected_component.c 연결 성분 찾기

■ 11장 그래프 II (ch11_graph2)
  01_spanning_tree_dfs.c  DFS 를 이용한 신장 트리
  02_kruskal.c            Kruskal 최소 비용 신장 트리 (union-find)
  03_prim.c               Prim 최소 비용 신장 트리
  04_dijkstra.c           Dijkstra 최단 경로 (STEP 별 distance / found 출력)
  05_dijkstra_path.c      Dijkstra 최단 "경로" (prev[] 로 경로 출력)
  06_floyd.c              Floyd 최단 경로 (A 행렬 단계별 출력)
  07_floyd_path.c         Floyd 최단 "경로" (next[][] 로 경로 출력)
  08_topo_sort.c          위상 정렬


■ 강의자료와 다르게 고친 부분
  - 슬라이드에 코드가 일부만 있는 경우(큐, 스택, main 함수 등)는 채워 넣어서
    파일 하나만으로 실행되게 했다.
  - void main() 은 int main(void) 로 바꾸었다.
  - 02_kruskal.c : 강의자료 주석대로 g->n 은 간선의 수이므로 정점의 수(7)를
    kruskal() 에 따로 넘겨준다.
  - 03_prim.c : 강의자료의 "tree[v] <- u (추가?)" 부분을 구현해서 선택된 간선도 출력한다.
  - 05_expression_tree.c, 08_thread_tree.c : 변수 이름 exp 가 수학 함수 exp 와
    겹쳐서 exp_root 로 바꾸었다.
  - 인접 리스트 버전(02, 04, 07)은 무방향 그래프라서 간선을 양쪽 방향으로 넣었다.
    리스트 맨 앞에 삽입하기 때문에 방문 순서가 인접 행렬 버전과 다를 수 있다.
