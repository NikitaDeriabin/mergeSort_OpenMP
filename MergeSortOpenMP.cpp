#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <iostream>

using namespace std;

void randFill(int* a, int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }
}

void merge(int* a, int l, int m, int r) {
    //sizes of left and right array
    int nl = m - l + 1;
    int nr = r - m;
    int* la = new int[nl];
    int* ra = new int[nr];

    //fill left and right sub-arrays
    for (int i = 0; i < nl; i++)
        la[i] = a[l + i];
    for (int i = 0; i < nr; i++)
        ra[i] = a[m + 1 + i];

    //merge temp arrays to real array
    int i = 0, j = 0, k = l;
    while (i < nl && j < nr) {
        if (la[i] <= ra[j]) {
            a[k++] = la[i++];
        }
        else {
            a[k++] = ra[j++];
        }
    }

    //extra element in left and right temp array
    while (i < nl)
        a[k++] = la[i++];
    while (j < nr)
        a[k++] = ra[j++];


}

void serialMergeSort(int* a, int l, int r) {
    int m;
    if (l < r) {
        m = l + (r - l) / 2;
        serialMergeSort(a, l, m);
        serialMergeSort(a, m + 1, r);
        merge(a, l, m, r);
    }
}
void parallelMergeSort(int* a, int l, int r) {
    int m;
    if (l < r) {
        m = l + (r - l) / 2;
        if (abs(m - l) <= 100 || abs(r - m) <= 100) {
            parallelMergeSort(a, l, m);
            parallelMergeSort(a, m + 1, r);
        }
        else{
            #pragma omp parallel sections 
            {
                #pragma omp section 
                {
                    parallelMergeSort(a, l, m);
                }
                #pragma omp section 
                {
                    parallelMergeSort(a, m + 1, r);
                }
            }
        }
        merge(a, l, m, r);
    }
}

int main()
{
    int n;

    double start;
    double finish;
    double duration;

    cout << "n = ";
    cin >> n;
    int* a = new int[n];
    int* b = new int[n];


    randFill(a, n);
    for (int i = 0; i < n; i++) {
        b[i] = a[i];
    }

    /* for (int i = 0; i < n; i++) {
         cout << a[i] << " ";
     }*/

    start = omp_get_wtime();
    parallelMergeSort(a, 0, n - 1);
    finish = omp_get_wtime();

    duration = finish - start;

    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) cout << "false" << '\n';
    }
             
    cout << "Parallel finished" << '\n';
    cout << "Time of parallel merge sort = " << duration << "\n\n";
    delete[] a;

    time_t st;
    time_t fn;

    st = clock();
    serialMergeSort(b, 0, n - 1);
    fn = clock();

    for (int i = 0; i < n - 1; i++) {
        if (b[i] > b[i + 1]) cout << "false\n";
    }

    cout << "Serial finished" << '\n';
    cout << "Time of serial merge sort = " << (fn-st) / double(CLOCKS_PER_SEC) << '\n';

}