#include <stdio.h>
#include <malloc.h>

void main(int argc, char *argv[])
{
	FILE *fp;
	int num_node;
	int *weight;
	int i, j;

	if (argc != 2) {
		printf("<Usage>: graph graph.txt\n");
		return;
	}
	fp = fopen(argv[1], "r");
	if (fp != NULL) {
		fscanf(fp, "%d", &num_node);
		weight = (int *)malloc(sizeof(int) * num_node * num_node);
		for (i = 0; i < num_node; i++) 
			for (j = 0; j < num_node; j++) 
				fscanf(fp, "%d", &weight[i*num_node + j]);
		printf("\n");
		for (i = 0; i < num_node; i++) {
			for (j = 0; j < num_node; j++) 
				printf("%d ", weight[i*num_node + j]);
			printf("\n");
		}
	}

	// MST program


	// shortest path program


	fclose(fp);
}




