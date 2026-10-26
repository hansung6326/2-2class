/* 8장 트리 - 링크 표현법 프로그램
 *      n1
 *     /  |
 *   n2   n3
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct TreeNode {
	int data;
	struct TreeNode *left, *right;
} TreeNode;

int main(void)
{
	TreeNode *n1, *n2, *n3;

	n1 = (TreeNode *)malloc(sizeof(TreeNode));
	n2 = (TreeNode *)malloc(sizeof(TreeNode));
	n3 = (TreeNode *)malloc(sizeof(TreeNode));

	n1->data = 10;		// 첫 번째 노드를 설정한다.
	n1->left = n2;
	n1->right = n3;
	n2->data = 20;		// 두 번째 노드를 설정한다.
	n2->left = NULL;
	n2->right = NULL;
	n3->data = 30;		// 세 번째 노드를 설정한다.
	n3->left = NULL;
	n3->right = NULL;

	printf("루트 = %d, 왼쪽 자식 = %d, 오른쪽 자식 = %d\n",
		n1->data, n1->left->data, n1->right->data);

	free(n1); free(n2); free(n3);
	return 0;
}
