#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	// extra memory
	int temp; 
	extraMemoryAllocated += sizeof(temp);

	// build max heap
	// heapify down
	for (int i = 1; i < n; i++) {
		int parent = (i - 1) / 2;
		int child = i;
		if (arr[child] > arr[parent]) {
			// heapify up
			while(arr[child] > arr[parent]){
				// swap
				temp = arr[child];
				arr[child] = arr[parent];
				arr[parent] = temp;
				// continue
				child = parent;
				parent = (child - 1) / 2;
			}
		}
	}

	// sort
	for (int i = n - 1; i > 0; i--) {
		// swap
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		// heapify down
		// last indexes sorted
		for (int j = 1; j < i; j++) {
			int parent = (j - 1) / 2;
			int child = j;
			if (arr[child] > arr[parent]) {
				// heapify up
				while(arr[child] > arr[parent]){
					// swap
					temp = arr[child];
					arr[child] = arr[parent];
					arr[parent] = temp;
					// continue
					child = parent;
					parent = (child - 1) / 2;
				}
			}
		}
	}
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	// left < right index
	if(l < r){
		// middle
		int m = (l + r) / 2;

		// left half
		mergeSort(pData, l, m);

		// right half
		mergeSort(pData, m + 1, r);
		
		int i = 0;
		int j = 0;
		
		// extra memory
		int * sort = (int *) malloc((r - l + 1) * sizeof(int));
		extraMemoryAllocated += (r - l + 1) * sizeof(int);
		
		// merge
		while(l + i <= m || m + j + 1 <= r){
			if(m + j + 1 > r){
				sort[i + j] = pData[l + i];
				i++;
			}
			else if(l + i > m){
				sort[i + j] = pData[m + j + 1];
				j++;
			}
			else if(pData[l + i] <= pData[m + j + 1]){
				sort[i + j] = pData[l + i];
				i++;
			}
			else if(pData[l + i] >= pData[m + j + 1]){
				sort[i + j] = pData[m + j + 1];
				j++;
			}
		}
		
		i = 0;

		// copy
		for(int k = l; k <= r; k++){
			pData[k] = sort[i];
			i++;
		}

		// free memory
		free(sort);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}