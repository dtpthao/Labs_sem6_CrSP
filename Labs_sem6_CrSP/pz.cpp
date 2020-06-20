//#include <iostream>
//#include <stdio.h>
//#include <time.h>
//#include <conio.h>
//#include <string.h>
//using namespace std;
//
//void GenPermutation(unsigned char *Key)
//{
//	int k[16] = { 0 };
//	for (int i = 0; i<16; i++)
//	{
//		do {
//			Key[i] = rand() % 16; k[Key[i]]++;
//		} while (k[Key[i]]>1);
//	}
//}
//
//int CheckPermutation(unsigned char *Key)
//{
//	for (int i = 0; i<16; i++)
//	{
//		if ((i == Key[i]) || (i == Key[i] + 1)
//			|| (i == Key[i] - 1)) return 1;
//	}
//	return 0;
//}
//
////int VerifyPermutation();
//int increases(unsigned char*Key)
//{
//	cout << "Test increases:  ";
//	int O = 0;
//	for (int i = 1; i<16; i++)
//		if (Key[i]>Key[i - 1]) O++;
//	if (fabs((double)O - 8) <= 1.15) {
//		cout << fabs((double)O - 8) << " < 1.15 - Test not performed!" << endl;
//		return 1;
//	}
//	cout << fabs((double)O - 8) << " > 1.15 - Test not performed!" << endl;
//	return 0;
//}
//
//int inversions(unsigned char*Key)
//{
//	cout << "Test inversions:  ";
//	int N = 0;
//	for (int i = 0; i<15; i++) {
//		int L = 0;
//		for (int j = i + 1; j<16; j++)
//			if (Key[i]>Key[j]) L++;
//		N += L;
//	}
//	if (fabs((double)N - 60) <= 10.67) {
//		cout << fabs((double)N - 60) << " < 10.67 - Test not performed!" << endl;
//		return 1;
//	}
//	cout << fabs((double)N - 60) << " > 10.67 - Test performed!" << endl;
//	return 0;
//}
//
//int cycles(unsigned char*Key)
//{
//	cout << "Test cycles:  ";
//	int E = 0; int mas[16] = { 0 };
//	for (int i = 0; i<16; i++) {
//		if (!mas[i]){
//			int x = i; E++;
//			while (Key[x] != i)	{
//				mas[x] = 1;  x = Key[x];
//			}
//			mas[x] = 1;
//		}
//		else continue;
//	}
//	if (fabs(E - 2.77) <= 1.67) {
//		cout << fabs(E - 2.77) << " < 1.67 - Test not performed!" << endl;
//		return 1;
//	}
//	cout << fabs(E - 2.77) << " > 1.67 - Test performed!" << endl;
//	return 0;
//}
//
//int main()
//{
//	srand(time(0));
//
//	unsigned char *K = new unsigned char[16];
//	do GenPermutation(K);
//	while (CheckPermutation(K));
//	cout << "Key:" << endl;
//	for (int i = 0; i < 16; i++) printf("%x  ", i);
//	cout << endl;
//	for (int i = 0; i < 16; i++) printf("%x  ", K[i]);
//	cout << endl << endl;
//	increases(K); 
//	inversions(K);
//	cycles(K);
//
//	cin.get();
//	return 0;
//}
