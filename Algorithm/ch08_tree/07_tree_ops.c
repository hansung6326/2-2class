/* 8장 트리 - 이진 트리 연산 : 노드 개수, 단말 노드 개수, 높이 */
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

typedef struct TreeNode {
	int data;
	struct TreeNode *left, *right;
} TreeNode;

// 노드 개수 : 각각의 서브트리에 대하여 순환 호출한 다음, 반환되는 값에 1을 더하여 반환
int get_node_count(TreeNode *node)
{
	int count = 0;
	if (node != NULL)
		count = 1 + get_node_count(node->left) +
			get_node_count(node->right);
	return count;
}

// 단말 노드 개수
int get_leaf_count(TreeNode *node)
{
	int count = 0;
	if (node != NULL) {
		if (node->left == NULL && node->right == NULL)
			return 1;
		else
			count = get_leaf_count(node->left) +
				get_leaf_count(node->right);
	}
	return count;
}

// 높이 : 서브 트리들의 반환값 중에서 최대값을 구하여 반환
int get_height(TreeNode *node)
{
	int height = 0;
	if (node != NULL)
		height = 1 + max(get_height(node->left),
			get_height(node->right));
	return height;
}

//               15
//         4            20
//      1           16       25
TreeNode n1 = { 1,  NULL, NULL };
TreeNode n2 = { 4,  &n1,  NULL };
TreeNode n3 = { 16, NULL, NULL };
TreeNode n4 = { 25, NULL, NULL };
TreeNode n5 = { 20, &n3,  &n4 };
TreeNode n6 = { 15, &n2,  &n5 };
TreeNode *root = &n6;

int main(void)
{
	printf("노드의 개수 = %d\n", get_node_count(root));
	printf("단말 노드의 개수 = %d\n", get_leaf_count(root));
	printf("트리의 높이 = %d\n", get_height(root));
	return 0;
}
