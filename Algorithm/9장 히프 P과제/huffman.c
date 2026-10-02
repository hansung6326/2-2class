#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode{
    int weight;
    char name[16];
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct{
    TreeNode *ptree;
    int key;
} element;

typedef struct{
    element* heap;
    int heap_size;
} HeapType;

HeapType* create(int n){
    HeapType* h = (HeapType*)malloc(sizeof(HeapType));
    h->heap = (element*)malloc(sizeof(element) * (n + 1));
    return h;
}

void init(HeapType* h){
    h->heap_size = 0;
}

void insert_min_heap(HeapType* h, element item){
    int i;
    i = ++(h->heap_size);

    while((i != 1) && (item.key < h->heap[i/2].key)){
        h->heap[i] = h->heap[i/2];
        i /= 2;
    }
    h->heap[i] = item;
}

element delete_min_heap(HeapType* h){
    int parent, child;
    element item, temp;

    item = h->heap[1];
    temp = h->heap[h->heap_size--];
    parent = 1;
    child = 2;

    while(child <= h->heap_size){
        if((child <= h->heap_size) && (h->heap[child].key > h->heap[child + 1].key)){
            child++;
        }
        if(temp.key < h->heap[child].key)
            break;
        h->heap[parent] = h->heap[child];
        parent = child;
        child *= 2;
    }
    h->heap[parent] = temp;
    return item;
}

TreeNode* make_tree(TreeNode* left, TreeNode* right){
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->left = left;
    node->right = right;
    return node;
}

void destroy_tree(TreeNode* root){
    if(root == NULL)
        return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

int is_leaf(TreeNode* root){
    return(!(root->left) && !(root->right));
}

void preorder(TreeNode* root){
    if(root == NULL)
        return;
    printf("%s ", root->name);
    preorder(root->left);
    preorder(root->right);
}

void make_codes(TreeNode* root, char buf[], int top, char* table[]){
    if(root->left) {
        buf[top] = '1';
        make_codes(root->left, buf, top+1, table);
    }
    if(root->right){
        buf[top] = '0';
        make_codes(root->right, buf, top+1, table);
    }
    if(is_leaf(root)){
        buf[top] = '\0';
        printf("%c: %s\n", root->name[0], buf);
        table[(unsigned char)root->name[0]] = (char*)malloc(top + 1);
        srtcpy(table[(unsigned char)root->name[0]], buf);
    }
}

int is_duplicate(char ch_list[], int count, char ch){
    for(int i=0; i<count; i++){
        if(ch_list[i] == ch)
            return 1;
    }
    return 0;
}

int read_input(char** ch_out, int** freq_out){
    char fname[256];
    FILE* fp = NULL;
    int n = 0, count = 0;
    char ch;
    int f;
}