#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"

char * 
write_numbers_to_file(int* numbers)
{
    FILE *filePointer;

    filePointer = fopen("output.txt", "w");

    for (int j = 0; j < 100000; j++){
        fprintf(filePointer, "%d \n", numbers[j]);

    }

    fclose(filePointer);
    return (char *)filePointer;
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void selection_sort(int arr[], int n)
{
	int i, j, min_idx;

	for (i = 0; i < n-1; i++){
		min_idx = i;

		for (j = i+1; j < n; j++)
		if (arr[j] < arr[min_idx])
			min_idx = j;

		if(min_idx != i)
			swap(&arr[min_idx], &arr[i]);
	}
}


int * read_numbers_from_file()
{
    FILE *filePointer;
    char file_line[30];
    static int array_numbers[100000];
    int aux = 0;

    filePointer = fopen("input.txt", "r");

    while (fgets(file_line, 30, filePointer) != NULL) {
        char* number_from_line = strtok(file_line, ",");
        while(number_from_line != NULL){
            int n_int = atoi(number_from_line);
            array_numbers[aux] = n_int;
            aux++;

            number_from_line = strtok(NULL, " ");
        }
    }
    fclose(filePointer);
    return array_numbers;
    
}

void random_int_number_generator()
{
    int i, n;
    FILE* fp;

    fp = fopen("input.txt", "w");

    for (i = 0; i < 100000; i++){
        n = rand();
        fprintf(fp, "%d\n", n);
    }
    fclose(fp);
}

char *sort()
{
    int *arr;
    random_int_number_generator();
    arr = read_numbers_from_file();
    selection_sort(arr, 100000);
    char *ret = write_numbers_to_file(arr);

}

// int main()
// {
//     // clock_t start, end;
//     // double time_taken;


//     // start = clock();
//     sort();
//     // end = clock();

//     // time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;

//     // printf("O procedimento está levando %f segundos para executar \n", time_taken);

//     return 0;
// }