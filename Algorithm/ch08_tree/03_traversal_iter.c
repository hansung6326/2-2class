/* 8장 트리 - 반복적인 순회 (스택 이용 : 중위, 전위, 후위)
 *               15
 *         4            20
 *      1           16       25
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct TreeNode {
	int data;
	struct TreeNode *left, *right;
} TreeNode;

#define SIZE 100
int top = -1;
TreeNode *stack[SIZE];

void push(TreeNode *p)
{
	if (top < SIZE - 1)
		stack[++top] = p;
}

TreeNode *pop()
{
	TreeNode *p = NULL;
	if (top >= 0)
		p = stack[top--];
	return p;
}

TreeNode *peek()
{
	if (top >= 0) return stack[top];
	return NULL;
}

int is_empty() { return top == -1; }

// 반복적인 중위 순회
void inorder_iter(TreeNode *root)
{
	while (1) {
		for (; root; root = root->left)
			push(root);
		root = pop();
		if (!root) break;
		printf("[%d] ", root->data);
		root = root->right;
	}
}

// 반복적인 전위 순회
void preorder_iter(TreeNode* root)
{
	if (!root) return;
	push(root);
	while (!is_empty()) {
		// Pop the top item from stack and print it
		root = pop();
		printf("[%d] ", root->data);
		// Push right and left children of the popped node to stack
		if (root->right)
			push(root->right);
		if (root->left)
			push(root->left);
	}
}

// 반복적인 후위 순회
void postorder_iter(TreeNode* root)
{
	if (!root) return;
	do {
		while (root) {
			if (root->right != NULL)
				push(root->right);
			push(root);
			root = root->left;
		}
		root = pop();
		if (root->right != NULL && peek() == root->right) {
			pop();			// remove right child from stack
			push(root);		// push root back to stack
			root = root->right;	// change root
		}
		else {
			printf("[%d] ", root->data);
			root = NULL;
		}
	} while (!is_empty());
}

TreeNode n1 = { 1,  NULL, NULL };
TreeNode n2 = { 4,  &n1,  NULL };
TreeNode n3 = { 16, NULL, NULL };
TreeNode n4 = { 25, NULL, NULL };
TreeNode n5 = { 20, &n3,  &n4 };
TreeNode n6 = { 15, &n2,  &n5 };
TreeNode *root = &n6;

int main(void)
{
	printf("중위 순회=");
	inorder_iter(root);
	printf("\n");

	top = -1;
	printf("전위 순회=");
	preorder_iter(root);
	printf("\n");

	top = -1;
	printf("후위 순회=");
	postorder_iter(root);
	printf("\n");
	return 0;
}
