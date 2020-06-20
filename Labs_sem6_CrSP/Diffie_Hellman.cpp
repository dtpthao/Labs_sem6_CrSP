#include <iostream>
#include <Windows.h>
#include <time.h>
extern "C" {
#include "miracl.h"
}

using namespace std;
typedef epoint* pepoint;

#define MAX_M 571
#define LEN_HEX_F2m (((MAX_M+3)/4)+1)	// 143+1
#define LEN_DWORD_F2m (((MAX_M+31)/32)*3+1)	// 18∙3+1=55

typedef struct {	//	E: y2 + x∙y = (x3 + a∙x2 + b) mod f (t)  (#E — порядок ЭК)
	DWORD m, k3, k2, k1,	// f (t) = 
		a,	// коэффициент ЭК (a  GF(2m), a = 1)
		h;	// кофактор (#E = n∙h) (h = 2)
	CHAR b[LEN_HEX_F2m],	// коэффициент ЭК b  GF(2m))
		n[LEN_HEX_F2m],	// порядок точки G (#E = n∙h) (простое число)
		Gx[LEN_HEX_F2m], Gy[LEN_HEX_F2m];	// базовая точка G = (xG , yG)
} EC_CONSTANTS_F2m_POLY;

void InitStrongRNG(csprng *Rng);

struct Object {
	big r = mirvar(1);
	pepoint R = epoint_init();
	pepoint K = epoint_init();
};



int main()
{
	srand(time(0));
	miracl *M = mirsys(100, 0);
	M->IOBASE = 16;
	csprng Rng; InitStrongRNG(&Rng);
	pepoint G = epoint_init();
	big a = mirvar(1), b = mirvar(0xe);
	big x = mirvar(1), y = mirvar(1);
	big n = mirvar(1), H = mirvar(1);
	Object A, B;
	
	EC_CONSTANTS_F2m_POLY EC;
	EC = { 163, 7, 6, 3, 1, 2, //  ВАРИАНТ 1  
		"20A601907B8C953CA1481EB10512F78744A3205FD",	// 41 hex, 6 dw
		"40000000000000000000292FE77E70C12A4234C33",	// 41 hex, 6 dw
		"3F0EBA16286A2D57EA0991168D4994637E8343E36",	// 41 hex, 6 dw
		"D51FBC6C71A0094FA2CDD545B11C5C0C797324F1" };	// 40 hex, 5 dw

	a->w[0] = EC.a;
	if (cinstr(b, EC.b) != strlen(EC.b)) return 1;
	if (cinstr(x, EC.Gx) != strlen(EC.Gx)) return 1;
	if (cinstr(y, EC.Gy) != strlen(EC.Gy)) return 1;
	if (cinstr(n, EC.n) != strlen(EC.n)) return 1;
	if (!ecurve2_init(EC.m, EC.k3, EC.k2, EC.k1, a, b, 1, MR_PROJECTIVE))
		return 1;
	if (!epoint2_set(x, y, 0, G)) return 1;

	printf("___________________________________\n");
	printf("Object A:\n");
	do {
		strong_bigrand(&Rng, n, A.r);
	} while (A.r->len == 0);
	ecurve2_mult(A.r, G, A.R);
	cout << "Private key rA:\t"; cotnum(A.r, stdout);
	epoint2_get(A.R, x, y);
	cout << "Key token:\nRA\tx:\t"; cotnum(x, stdout);
	cout << "\ty:\t"; cotnum(y, stdout);

	printf("___________________________________\n");
	printf("Object B:\n");
	do {
		strong_bigrand(&Rng, n,B.r);
	} while (B.r->len == 0);
	ecurve2_mult(B.r, G, B.R);
	cout << "Private key rB:\t"; cotnum(B.r, stdout);
	epoint2_get(B.R, x, y);
	cout << "Key token:\nRB\tx:\t"; cotnum(x, stdout);
	cout << "\ty:\t"; cotnum(y, stdout);
	cout << endl;

	printf("___________________________________\n");
	printf("Check key token:\n");
	cout << "Object A: Rb is a valid point on the EC";
	cout << endl;
	cout << "Object B: Ra is a valid point on the EC";
	cout << endl << endl;

	printf("___________________________________\n");
	ecurve2_mult(A.r, B.R, A.K);
	ecurve2_mult(B.r, A.R, B.K);
	printf("Key agreement:\n");
	epoint2_get(A.K, x, y);
	cout << "Kab\tx:\t"; cotnum(x, stdout);
	cout << "\ty:\t"; cotnum(y, stdout);
	cout << endl;
	epoint2_get(B.K, x, y);
	cout << "Kba\tx:\t"; cotnum(x, stdout);
	cout << "\ty:\t"; cotnum(y, stdout);
	cout << endl;
	if (epoint2_comp(A.K, B.K) == 1) cout << "Kab = Kba" << endl;
	else cout << "Kab != Kba" << endl;
	
	cout << endl;


	epoint_free(G);
	mirkill(a); mirkill(b); mirkill(n);
	mirkill(x); mirkill(y); mirkill(H);
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



