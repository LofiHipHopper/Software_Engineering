//with the help of https://www.geeksforgeeks.org/quick-sort/
#include <iostream>

using namespace std;

int partition(int arr[], int left, int right)
{
    int pivot = arr[right];
    int i = left - 1;
    int temp; //for copy

    for (int j = left; j <= right - 1; j++) {
      
        if (arr[j] < pivot) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

        }
    }

    temp = arr[i+1];
    arr[i+1] = arr[right];
    arr[right] = temp;


    return (i + 1);
}


void quickSort(int arr[], int left, int right)
{
    if (left < right) {

        int index = partition(arr, left, right);

        quickSort(arr, left, index - 1);

        quickSort(arr, index + 1, right);

    }
}


void main()
{
    int arr[] = { 5, 5, 4, 0, 2, 1, 3, 8, 9, 6 };

    int n = sizeof(arr) / sizeof(arr[int(0)]);


    quickSort(arr, 0, n - 1);

    for (int i = 0; i < n; i++)
        cout << arr[i] << endl;

    
}
