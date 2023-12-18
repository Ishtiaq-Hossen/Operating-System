#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Global arrays for the original array and the three subarrays
int array1[50] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}, array2[50], array3[50], array4[50];
int subarray1, subarray2, total;  // Variables to store the sizes of subarrays and the total size

// Function to perform sorting on the first subarray
void *subarray1_func(void *arg) {
    sleep(1);
    printf("\nFirst subarray: ");
    for (int i = 0; i < subarray1; i++) {
        printf("%d ", array2[i]);
    }

    // Bubble sort for the first subarray
    for (int i = 0; i < subarray1; i++) {
        for (int j = 0; j < subarray1 - (i + 1); j++) {
            if (array2[j] > array2[j + 1]) {
                int temp = array2[j];
                array2[j] = array2[j + 1];
                array2[j + 1] = temp;
            }
        }
    }

    printf("\nFirst Sorted array: ");
    for (int i = 0; i < subarray1; i++) {
        printf("%d ", array2[i]);
    }
}

// Function to perform sorting on the second subarray
void *subarray2_func(void *arg) {
    sleep(2);
    printf("\nSecond subarray: ");
    for (int i = 0; i < subarray2; i++) {
        printf("%d ", array3[i]);
    }

    // Bubble sort for the second subarray
    for (int i = 0; i < subarray2; i++) {
        for (int j = 0; j < subarray2 - (i + 1); j++) {
            if (array3[j] > array3[j + 1]) {
                int temp = array3[j];
                array3[j] = array3[j + 1];
                array3[j + 1] = temp;
            }
        }
    }

    printf("\nSecond Sorted array: ");
    for (int i = 0; i < subarray2; i++) {
        printf("%d ", array3[i]);
    }
}

// Function to merge and sort the two subarrays
void *merge_func(void *arg) {
    sleep(3);
    total = subarray1 + subarray2;

    // Copy the first subarray to the merged array
    for (int i = 0; i < subarray1; i++) {
        array4[i] = array2[i];
    }

    int tempsubarray1 = subarray1;

    // Append the second subarray to the merged array
    for (int i = 0; i < subarray2; i++) {
        array4[tempsubarray1] = array3[i];
        tempsubarray1++;
    }

    printf("\nMerged Array: ");
    for (int i = 0; i < total; i++) {
        printf("%d ", array4[i]);
    }

    // Bubble sort for the merged array
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (array4[j + 1] < array4[j]) {
                int temp = array4[j];
                array4[j] = array4[j + 1];
                array4[j + 1] = temp;
            }
        }
    }
}

// Main function
int main(int argc, char const *argv[]) {
    int n = 10;
    pthread_t t1, t2, t3;

    printf("Provided Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", array1[i]);
    }

    int j = 0;

    // Divide the original array into two subarrays
    for (int i = 0; i < n / 2; i++) {
        array2[j] = array1[i];
        j++;
    }

    subarray1 = j;

    int k = 0;

    for (int i = n / 2; i < n; i++) {
        array3[k] = array1[i];
        k++;
    }

    subarray2 = k;

    // Create threads to perform operations on subarrays
    pthread_create(&t1, NULL, subarray1_func, NULL);
    pthread_create(&t2, NULL, subarray2_func, NULL);
    pthread_create(&t3, NULL, merge_func, NULL);

    // Wait for threads to finish execution
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("\nSorted Merged Array: ");
    for (int i = 0; i < total; i++) {
        printf("%d ", array4[i]);
    }

    printf("\n");
    return 0;
}
