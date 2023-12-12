#include <iostream>

int main() {
    int n, m;
    std::cin >> n >> m;
    int** arr = new int*[n];
    for (int i = 0 ; i < n ; i ++)
        arr[i] = new int[m];
    for (int i = 0 ; i < n ; i ++)
        for (int j = 0 ; j < m ; j ++)
            std::cin >> arr[i][j];
    int max = arr[0][0], min = arr[0][0];
    int max_row = 0, min_row = 0;
    for (int i = 0 ; i < n ; i ++)
        for (int j = 0 ; j < m ; j ++) {
            // max check
            if (max < arr[i][j]) {
                max = arr[i][j];
                max_row = i;
            }
            if (min > arr[i][j]) {
                min = arr[i][j];
                min_row = i;
            }
        }
    for (int j = 0 ; j < m ; j ++) {
        int tmp = arr[min_row][j];
        arr[min_row][j] = arr[max_row][j];
        arr[max_row][j] = tmp;
    }
    for (int i = 0 ; i < n ; i ++, std::cout << "\n")
        for (int j = 0 ; j < m ; j ++, std::cout << " ")
            std::cout << arr[i][j];
    for (int i = 0 ; i < n ; i ++)
        delete[] arr[i];
    delete[] arr;
}