#include <iostream>
#include <time.h>
#include <stdio.h>
//#include <string.h>
#include <Windows.h>
extern "C" {
#include "miracl.h"
}

using namespace std;
typedef epoint* pepoint;

void InitStrongRNG(csprng *Rng);
int Enum(pepoint P);

//int HashData(char *str, big H)
//{
//	int len = strlen(str);
//	sha psh;
//	char hash[20];
//	int i;
//	shs_init(&psh);
//	for (i = 0; i < len; i++)
//		shs_process(&psh, str[i]);
//	shs_hash(&psh, hash);
//	bytes_to_big(20, hash, H);
//	return 0;
//}

unsigned int ReadData(char *str) {
	FILE *fp = fopen("Info.txt", "r");
	if (!fp) return 0;
	//char *str = new char[100];
	/*while (!feof(fp)) {
		fgets(str, 100, fp);
	}*/
	fseek(fp, 0, 2);
	int len = ftell(fp);
	fseek(fp, 0, 0);
	fread(str, sizeof(byte), len, fp);
	fclose(fp);
	return len;
}

BOOL GetHash(big H, char *str, unsigned int len)
{
	//unsigned len = ReadData(str);
	//char str[100] = "klfasfjwofjowjwoijwonowijvoiwjfw";
	sha psh;
	char *hash = new char[20];
	int i;
	shs_init(&psh);
	for (i = 0; i < len; i++)
		shs_process(&psh, str[i]);
	shs_hash(&psh, hash);
	bytes_to_big(20, hash, H);
	//HashData(str, H);
	return 0;
}

void ECDSA(pepoint G, big n, big d, big H, csprng Rng, big r, big s)
{
	big k = mirvar(1);
	big tmp = mirvar(1);
	pepoint R = epoint_init();
	do {
		do {
			do strong_bigrand(&Rng, n, k);
			while (k->len == 0 || (k->len == 1 && k->w[0] == 1)
				|| (k->len == n->len && k->w[k->len - 1] == n->w[n->len - 1] - 1));
			ecurve2_mult(k, G, R);
			epoint2_get(R, r, tmp);
			divide(r, n, n);
		} while ((r->len) == 0);
		xgcd(k, n, k, k, k);
		mad(d, r, H, n, s, tmp);
		multiply(k, tmp, s);
		divide(s, n, n);
	} while (s->len == 0);

	mirkill(k); mirkill(tmp);
	epoint_free(R);
}

void mulmod(big x, big y, big n, big z)
{
	multiply(x, y, z);
	divide(z, n, n);
}

BOOL Verify_ECDSA(pepoint G, big n, pepoint Q, big H, big r, big s, csprng Rng) 
{
	if ((r->len & 0x80000000) || (s->len & 0x80000000)
		|| (compare(r, n) >= 0) || (compare(s, n) >= 0))
		return 0;
	big w = mirvar(1);
	big u = mirvar(1);
	big v = mirvar(1);
	//big r2 = mirvar(1);
	pepoint R = epoint_init();
	copy(s, w);
	xgcd(w, n, w, w, w);
	mulmod(w, H, n, u);
	mulmod(w, r, n, v);
	ecurve2_mult2(u, G, v, Q, R); 
	epoint2_norm(R);
	divide(R->X, n, n);
	//cotnum(R->X, stdout);
	mirkill(w); mirkill(u); mirkill(v);
	if (compare(r, R->X)) {
		epoint_free(R);
		return 0;
	}
	epoint_free(R);
	return 1;
}

void ECSS(pepoint G, big n, big d, big H, big r, big s, csprng Rng)
{
	big k = mirvar(1), tmp = mirvar(1);
	pepoint R = epoint_init();
	do {
		do {
			do strong_bigrand(&Rng, n, k);
			while (k->len == 0 || (k->len == 1 && k->w[0] == 1)
				|| (k->len == n->len && k->w[k->len - 1] == n->w[n->len - 1] - 1));
			ecurve2_mult(k, G, R);
			epoint2_norm(R);
			add(H, R->X, r);
			divide(r, n, n);
		} while (r->len == 0);
		multiply(r, d, tmp);
		divide(tmp, n, n);
		subtract(k, tmp, s);
		if (s->len & 0x80000000) add(s, n, s);
		else divide(s, n, n);
	} while (s->len == 0);
	mirkill(k);
	epoint_free(R);
}

BOOL Verify_ECSS(pepoint G, big n, pepoint Q, big H, big r, big s, csprng Rng)
{
	if ((r->len & 0x80000000) || (s->len & 0x80000000)
		|| (compare(r, n) == 1) || (compare(s, n) == 1))
		return 0;
	pepoint R = epoint_init();
	pepoint ptmp = epoint_init();
	ecurve2_mult2(s, G, r, Q, R); 
	epoint2_norm(R);
	//epoint2_get(R, r2, tmp); cout << "R: "; cotnum(R->X, stdout);
	divide(R->X, n, n);
	add(H, R->X, R->X);
	divide(R->X, n, n);
	//cotnum(R->X, stdout);
	
	if (compare(r, R->X)) {
		epoint_free(R);
		return 0;
	}
	epoint_free(R);
	return 1;
}

int main()
{
	srand(time(0));
	miracl *M = mirsys(100, 0);
	M->IOBASE = 16;
	csprng Rng; InitStrongRNG(&Rng);
	pepoint G = epoint_init();
	pepoint Q = epoint_init();
	big a = mirvar(1), b = mirvar(0xe);
	big x = mirvar(1), y = mirvar(1);
	big n = mirvar(1), H = mirvar(1);
	big d = mirvar(1);
	big r = mirvar(1);
	big s = mirvar(1);
	big r2 = mirvar(1);
	big s2 = mirvar(1);
	if (cinstr(b, "20A601907B8C953CA1481EB10512F78744A3205FD") != 41) {
		return 1;
	}
	cinstr(x, "3F0EBA16286A2D57EA0991168D4994637E8343E36");
	cinstr(y, "D51FBC6C71A0094FA2CDD545B11C5C0C797324F1");
	cinstr(n, "40000000000000000000292FE77E70C12A4234C33");


	if (!ecurve2_init(163, 7, 6, 3, a, b, 1, MR_PROJECTIVE)) {
		return 1;
	}
		
	if (!epoint2_set(x, y, 0, G)) return 1;
	do {
		strong_bigrand(&Rng, n, d);
	} while (d->len == 0 || (d->len == 1 && d->w[0] == 1)
		|| (d->len == n->len && d->w[d->len - 1] == n->w[n->len - 1] - 1));
	ecurve2_mult(d, G, Q);

	int correct = 0, lenData,tmp;
	char *str = new char[1000];
	lenData = ReadData(str);
	//tmp = rand() % lenData;
	//str[tmp] = 0;
	GetHash(H, str, lenData);
	ECDSA(G, n, d, H, Rng, r, s);
	cout << "ECDSA: \n"; 
	cout << "H: "; cotnum(H, stdout);
	cout << "r: "; cotnum(r, stdout);
	cout << "s: "; cotnum(s, stdout);
	cout << endl;
	cout << "Verify ECDSA: \n";
	correct = Verify_ECDSA(G, n, Q, H, r, s, Rng);
	cout << "r': "; cotnum(r, stdout);
	cout << "Result: "; 
	if (correct) cout << "r = r'" << endl;
	else cout << "r >< r'" << endl;
	cout << "______________________________________________" << endl << endl;
	cout << "ECSS: \n";
	ECSS(G, n, d, H, r2, s2, Rng);
	cout << "H: "; cotnum(H, stdout);
	cout << "r: "; cotnum(r2, stdout);
	cout << "s: "; cotnum(s2, stdout);
	cout << endl;
	cout << "Verify ECSS: \n";
	correct = Verify_ECSS(G, n, Q, H, r2, s2, Rng);
	cout << "r': "; cotnum(r, stdout);
	cout << "Result: ";
	if (correct) cout << "r = r'" << endl;
	else cout << "r >< r'" << endl;
	cout << endl;

	//unsigned int correct = 0, lenData, tmp;
	//char *Data = new char[1000];
	//char c;
	//lenData = ReadData(Data);
	//
	//
	//
	//for (int i = 0; i < 100; i++) {
	//	tmp = rand() % lenData;
	//	//cout << lenData << "\t" << tmp << endl;
	//	Data[tmp] = 'a';
	//	
	//	GetHash(H, Data, lenData);
	//	
	//	ECDSA(G, n, d, H, Rng, r, s);
	//	correct += Verify_ECDSA(G, n, Q, H, r, s, Rng);
	//}
	//printf("The frequency of correct ECDSA: %d/100\n", correct);

	epoint_free(G);
	epoint_free(Q);
	mirkill(a); mirkill(b); mirkill(n);
	mirkill(x); mirkill(y);
	mirkill(d); mirkill(H);
	mirkill(r); mirkill(s);
	mirkill(r2); mirkill(s2);
	mirexit();
	system("pause");
	return 0;
}

void InitStrongRNG(csprng *Rng)
{
	srand(time(NULL));
	char raw[100];
	for (int i = 0; i < 100; i++)
		raw[i] = (char)rand();
	strong_init(Rng, 100, raw, time(NULL));
}

int Enum(pepoint P)
{
	pepoint Q = epoint_init();
	epoint2_copy(P, Q);
	big n = mirvar(1);
	while (!point_at_infinity(Q)) {
		n->w[0]++;
		ecurve2_mult(n, Q, Q);
	}
	epoint_free(Q);
	return n->w[0];
}
