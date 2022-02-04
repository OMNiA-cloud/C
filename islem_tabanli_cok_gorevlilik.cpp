#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <ctime>
using namespace std;
                                                                      
int main() {
    LARGE_INTEGER li;
    __int64 CounterStart = 0;
    if (!QueryPerformanceFrequency(&li))
        printf("QueryPerformanceFrequency failed\n");
    double PCFreq = (double)li.QuadPart;


    // Değişkenlerin tanımlanması.
    int  i, j, k, l, m,  ** A, ** B, ** C;

    // Matrislerin oluşturulması
    A = new int* [25];
    B = new int* [25];
    C = new int* [25];
    for (i = 0; i < 25; i++)
        A[i] = new int[25];
    for (i = 0; i < 25; i++)
        B[i] = new int[25];
    for (i = 0; i < 25; i++)
        C[i] = new int[25];
    // Çarpım koşulunun kontrol edilmesi

        // A ve B matrisinin elemenlarının atanması
    for (i = 0; i < 25; i++)
        for (j = 0; j < 25; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    cout << endl;

    // start time
    /*QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
    CRITICAL_SECTION kritik;
    InitializeCriticalSection(&kritik);

    EnterCriticalSection(&kritik); */

    // C matrisi başlangıçta sıfırlandı böylece eksi değer çıkması engellendi

    for (l = 0; l < 25; l++) {
        cout << endl;
        for (m = 0; m < 25; m++) {
             C[l][m]= 0;

        }
        cout << endl;
    }




    //matrislerin çarpılıp sonuc matrisine aktarılması
    for (i = 0; i < 25; i++)
        for (j = 0; j < 25; j++)
            for (k = 0; k < 25; k++)
                C[i][j] += A[i][k] * B[k][j];
    cout << "\nA*B=\n";
    //LeaveCriticalSection(&kritik);
    QueryPerformanceCounter(&li);
    printf("Elapsed time: %.6f s\n",
        ((double)(li.QuadPart - CounterStart)) / PCFreq);
   
   
    // C matrisinin yazdırılması
    for (i = 0; i < 25; i++) {
        cout << endl;
        for (j = 0; j < 25; j++) {
            cout << C[i][j] << " ";

        }
    }

    return 0;
}