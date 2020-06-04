#include<iostream>
using namespace std;

int maxWater(int a[], int n) {
	int l[n],r[n], maxs =0;
	l[0] = a[0];
	r[n-1] = a[n-1];
	for (int i = 1; i<n;  i++) {
		l[i] = max(a[i], l[i-1]);
	}
	for (int i = n-2; i >= 0;  i--) {
		r[i] = max(a[i], r[i+1]);
	}

	for (int i=0; i<n; i++){
		maxs += min(l[i],r[i]) - a[i];
	}
	return maxs;
}

int main()  
{  
    //int arr[] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};  
    int arr[] = {2, 1, 3, 4, 6, 5 };  
    int n = sizeof(arr)/sizeof(arr[0]);  
      
    cout << maxWater(arr, n)<<endl;  
      
    return 0;  
}
