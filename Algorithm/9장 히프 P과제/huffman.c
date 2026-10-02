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

    printf("입력 파일 이름 : ");
    if(fgets(fname, sizeof(fname), stdin) == NULL)
        return 0;
    fname[strcspn(fname, "\r\n")] = '\0';

    if(fname[0] != '\0'){
        fp = fopen(fname, "r");
        if(fp == NULL){
            printf("파일 열기 실패 : %s\n", fname);
            return 0;
        }
        if(fscnaf(fp, "%d", &n) != 1)
            n = 0;
    }
    else{
        printf("문자 개수? ");
        if(scanf("%d", &n) != 1)
            n = 0;
    }
    if(n < 2){
        printf("문자는 2개 이상 입력해야 합니다.\n");
        if(fp)
            fclose(fp);
        return 0;
    }

    char* ch_list = (char*)malloc(n);
    int* freq = (int*)malloc(sizeof(int) * n);

    for(int i=0; i<n; i++){
        if(fp){
            if(fscanf(fp, " %c %d", &ch, &f) != 2)
                break;
            if(is_duplicate(ch_list, count, ch)){
                printf("중복된 문자 %c 제외\n", ch);
                continue;
            }
        }
        else{
            printf("문자? ");
            scanf(" %c", &ch);
            if(is_duplicate(ch_list, count, ch)){
                printf("이미 입력한 문자입니다. 다시 입력하세요.\n");
                i--;
                continue;
            }
            printf("빈도수? ");
            scanf("%d", &f);

        }
        ch_list[count] = ch;
        freq[count] = f;
        count++;
    }
    if(fp){
        fclose(fp);
        printf("읽은 자료 %d개: ", count);
        for(int i=0; i<count; i++){
            printf("%c(%d) ", ch_list[i], freq[i]);
        }
        printf("\n");
    }
    
    *ch_out = ch_list;
    *freq_out = freq;
    return count;
}

TreeNode* huffman_tree(int freq[], char ch_list[], int n){
    int i;
    TreeNode *node, *x;
    HeapType* heap;
    element e, e1, e2;

    heap = create(n);
    init(heap);
    for(i=0; i<n; i++){
        node = make_tree(NULL, NULL);
        node->name[0] = ch_list[i];
        node->name[1] = '\0';
        e.key = node->weight = freq[i];
        e.ptree = node;
        insert_min_heap(heap, e);
    }
    for(i = 1; i<n; i++){
        e1 = delete_min_heap(heap);
        e2 = delete_min_heap(heap);
        
        x = make_tree(e1.ptree, e2.ptree);
        sprintf(x->name, "H-%d", i);
        e.key = x->weight = e1.key + e2.key;
        e.ptree = x;
        printf("%s(%d) + %s(%d) -> %s(%d)\n", e1.ptree->name, e1.key, e2.ptree->name, e2.key, x->name, e.key);
        insert_min_heap(heap, e);
    }

    e = delete_min_heap(heap);
    free(heap->heap);
    free(heap);
    return e.ptree;

}

int main(void){
    char* ch_list;
    int* freq;
    char* table[256] = {NULL};
    int n, i;

    n = read_input(&ch_list, &freq);
    if(n < 2)
        return 1;
    
    printf("\n----- 허프만 트리 생성 -----\n");
    TreeNode* root = huffman_tree(freq, ch_list, n);
    printf("\n----- 전위 순회 결과 -----\n");
    preorder(root);
    printf("\n");

    printf("\n----- 허프만 코드(%d개) -----\n", n);
    char* buf = (char*)malloc(n + 1);
    make_codes(root, buf, 0, table);
    free(buf);

    int fixed_bits = 0;
    while((1 << fixed_bits)< n){
        fixed_bits++;
    }
    long total_freq = 0, huff_total = 0;
    for(i=0; i<n; i++){
        total_freq += freq[i];
        huff_total += (long)freq[i] * (long)strlen(table[(unsigned char)ch_list[i]]);
    }

    printf("\n----- 비트 수 비교 -----\n");
    printf("고정 길이 %d 비트 : %ld bits\n", fixed_bits, total_freq *fixed_bits);
    printf("허프만 코드 \t : %ld bits\n", huff_total);

    int max_len;
    char fmt[20];
    char *str, *code;

    printf("\n----- 인코딩 -----\n");
    printf("문자열 최대 길이? ");
    scanf("%d", &max_len);
    str = (char*)malloc(max_len + 1);
    sprintf(fmt, "%s", max_len);
    printf("문자열? ");
    scanf(fmt, str);

    int code_len = 0, ok = 1;
    for(i=0; str[i]; i++){
        if(table[(unsigned char)str[i]] == NULL){
            printf("%c는 코드표에 없는 문자입니다.\n", str[i]);
            ok = 0;
            break;
        }
        code_len += (int)strlen(table[(unsigned char)str[i]]);
    }
    if(ok){
        code = (char*)malloc(code_len + 1);
        code[0] = '\0';
        for(i = 0; str[i]; i++){
            strcat(code, table[(unsigned char)str[i]]);
        }
        printf("코드열: %s\n", code);
        free(code)
    }
    free(str);

    printf("\n----- 디코딩 -----");
    printf("코드열 최대 길이? ");
    scanf("%d", &max_len);
    code = (char*)malloc(max_len + 1);
    sprintf(fmt, "%s", max_len);
    printf("코드열? ");
    scanf(fmt, code);

    TreeNode* p = root;
    ok = 1;
    printf("문자열: ");
    for(i=0; code[i]; i++){
        if(code[i] == '1')
            p = p->left;
        else if(code[i] == '0')
            p = p->right;
        else{
            ok = 0;
            break;
        }
        if(is_leaf(p)){
            putchar(p->name[0]);
            p = root;
        }
    }

    printf("\n");

    if(!ok)
        printf("0과 1만 입력해야 합니다.\n");
    else if(p != root)
        printf("코드열이 중간에 끝났습니다.\n");
    
    free(code);

    for(i = 0; i < 256; i++){
        free(table[i]);
    }
    destroy_tree(root);
    free(ch_list);
    free(freq);

    return 0;
}

