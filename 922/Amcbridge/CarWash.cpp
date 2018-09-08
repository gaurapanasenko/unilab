#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
	int arr[4][3] = {{0,0,0},{20,10,15},{7,2,5},{5,0,0}};
	int total = 0, x=0, y[4],z;
	char buffer [33];
	string output = "";
	while (x != -1) {
		cin >> x;
		if (x == -1) break;
		for (int i=3;i>=0;i--) {
			y[i]=x%10;
			x/=10;
		}
		z = 0;
		if (y[1]) z += arr[y[0]][0];
		if (y[2]) z += arr[y[0]][1];
		if (y[3]) z += arr[y[0]][2];
		total += z;
		sprintf(buffer,"%i ",z);
		output = output + string(buffer) + " ";
	}
	cout << total << " " << output << "-1";
}
