﻿#include <iostream>
//#include <time.h>

using namespace std;
typedef unsigned char u_char;

u_char constan[4][8] = { { 0xA0, 0x9e, 0x66, 0x7f, 0x3b, 0xcc, 0x90, 0x8b },
    { 0xb6, 0x7a, 0xe8, 0x58, 0x4c, 0xaa, 0x73, 0xb2 },
    { 0xc6, 0xef, 0x37, 0x2f, 0xe9, 0x4f, 0x82, 0xbe },
    { 0x54, 0xff, 0x53, 0xa5, 0xf1, 0xd3, 0x6f, 0x1c } };

u_char S_box[256] = {
    0x70, 0x82, 0x2c, 0xec, 0xb3, 0x27, 0xc0, 0xe5, 0xe4, 0x85, 0x57, 0x35, 0xea, 0x0c, 0xae, 0x41,
    0x23, 0xef, 0x6b, 0x93, 0x45, 0x19, 0xa5, 0x21, 0xed, 0x0e, 0x4f, 0x4e, 0x1d, 0x65, 0x92, 0xbd,
    0x86, 0xb8, 0xaf, 0x8f, 0x7c, 0xeb, 0x1f, 0xce, 0x3e, 0x30, 0xdc, 0x5f, 0x5e, 0xc5, 0x0b, 0x1a,
    0xa6, 0xe1, 0x39, 0xca, 0xd5, 0x47, 0x5d, 0x3d, 0xd9, 0x01, 0x5a, 0xd6, 0x51, 0x56, 0x6c, 0x4d,
    0x8b, 0x0d, 0x9a, 0x66, 0xfb, 0xcc, 0xb0, 0x2d, 0x74, 0x12, 0x2b, 0x20, 0xf0, 0xb1, 0x84, 0x99,
    0xdf, 0x4c, 0xcb, 0xc2, 0x34, 0x7e, 0x76, 0x05, 0x6d, 0xb7, 0xa9, 0x31, 0xd1, 0x17, 0x04, 0xd7,
    0x14, 0x58, 0x3a, 0x61, 0xde, 0x1b, 0x11, 0x1c, 0x32, 0x0f, 0x9c, 0x16, 0x53, 0x18, 0xf2, 0x22,
    0xfe, 0x44, 0xcf, 0xb2, 0xc3, 0xb5, 0x7a, 0x91, 0x24, 0x08, 0xe8, 0xa8, 0x60, 0xfc, 0x69, 0x50,
    0xaa, 0xd0, 0xa0, 0x7d, 0xa1, 0x89, 0x62, 0x97, 0x54, 0x5b, 0x1e, 0x95, 0xe0, 0xff, 0x64, 0xd2,
    0x10, 0xc4, 0x00, 0x48, 0xa3, 0xf7, 0x75, 0xdb, 0x8a, 0x03, 0xe6, 0xda, 0x09, 0x3f, 0xdd, 0x94,
    0x87, 0x5c, 0x83, 0x02, 0xcd, 0x4a, 0x90, 0x33, 0x73, 0x67, 0xf6, 0xf3, 0x9d, 0x7f, 0xbf, 0xe2,
    0x52, 0x9b, 0xd8, 0x26, 0xc8, 0x37, 0xc6, 0x3b, 0x81, 0x96, 0x6f, 0x4b, 0x13, 0xbe, 0x63, 0x2e,
    0xe9, 0x79, 0xa7, 0x8c, 0x9f, 0x6e, 0xbc, 0x8e, 0x29, 0xf5, 0xf9, 0xb6, 0x2f, 0xfd, 0xb4, 0x59,
    0x78, 0x98, 0x06, 0x6a, 0xe7, 0x46, 0x71, 0xba, 0xd4, 0x25, 0xab, 0x42, 0x88, 0xa2, 0x8d, 0xfa,
    0x72, 0x07, 0xb9, 0x55, 0xf8, 0xee, 0xac, 0x0a, 0x36, 0x49, 0x2a, 0x68, 0x3c, 0x38, 0xf1, 0xa4,
    0x40, 0x28, 0xd3, 0x7b, 0xbb, 0xc9, 0x43, 0xc1, 0x15, 0xe3, 0xad, 0xf4, 0x77, 0xc7, 0x80, 0x9e
};

void Print(char *s, u_char S[], int byte) {
	printf("%s:\t", s);
	for (int i = 0; i < byte; i++) printf("%2x, ", S[i]);
	cout << endl;
}

void XOR64(u_char X[8], u_char Y[8], u_char Z[8]) {
    for(int i = 0; i < 8; i++) 
		Z[i] = X[i] ^ Y[i];
	//Print("    XOR", Z, 8);
}

void LeftCirRotation(u_char Y[8], u_char X[16], int len, int side)
{
	unsigned int rem = len & 0x7, shiftbyte, index;
	shiftbyte = len / 8 + (rem ? 1 : 0) + side;
	for (int i = 0; i< 8; i++) {
		index = (shiftbyte + i) & 0xf;
		Y[i] = (X[index] >> (8 - rem)) | (X[(index - 1) & 0xf] << rem);
	}
}

void S_function(u_char X[8])
{
    X[0] = S_box[X[0]];
    X[1] = (S_box[X[1]] << 1) | ((S_box[X[1]] & 0x80) >> 7);
    X[2] = (S_box[X[2]] << 7) | ((S_box[X[2]] & 0xfe) >> 1);
	X[3] = S_box[(X[3] << 1 & 0xff) | ((X[3] & 0x80) >> 7)];
    X[4] = (S_box[X[4]] << 1) | ((S_box[X[4]] & 0x80) >> 7);
    X[5] = (S_box[X[5]] << 7) | ((S_box[X[5]] & 0xfe) >> 1);
    X[6] = S_box[(X[6] << 1 & 0xff) | ((X[6] & 0x80) >> 7)];
    X[7] = S_box[X[7]];
	//Print("    S_function", X, 8);
}

void P_function(u_char X[8])
{
	u_char Y[8];
	Y[0] = X[0] ^ X[2] ^ X[3] ^ X[5] ^ X[6] ^ X[7];
	Y[1] = X[0] ^ X[1] ^ X[3] ^ X[4] ^ X[6] ^ X[7];
	Y[2] = X[0] ^ X[1] ^ X[2] ^ X[4] ^ X[5] ^ X[7];
	Y[3] = X[1] ^ X[2] ^ X[3] ^ X[4] ^ X[5] ^ X[6];
	Y[4] = X[0] ^ X[1] ^ X[5] ^ X[6] ^ X[7];
	Y[5] = X[1] ^ X[2] ^ X[4] ^ X[6] ^ X[7];
	Y[6] = X[2] ^ X[3] ^ X[4] ^ X[5] ^ X[7];
	Y[7] = X[0] ^ X[3] ^ X[4] ^ X[5] ^ X[6];
	for (int i = 0; i < 8; i++) X[i] = Y[i];
	//Print("    P_function", X, 8);
}

void F_function(u_char X[8], u_char k[8], u_char Y[8])
{
	//cout << "  F_function" << endl;
	//Print("  X", X, 8);
	//Print("  k", k, 8);
    XOR64(X, k, Y);
    S_function(Y);
    P_function(Y);
}

void Round(u_char k[][8], int fromindex, u_char KeyL[8], u_char KeyR[8], int toindex)
{
    u_char tmp[8];
	//cout << "\n  Round " << fromindex << endl;
	//Print("  L", KeyL, 8);
	//Print("  R", KeyR, 8);
    F_function(KeyL, k[fromindex], tmp);
	//Print("  return F_function", tmp, 8);
	//Print("  R", KeyR, 8);
    XOR64(tmp, KeyR, KeyR);
	//Print("  final round", KeyR, 8);
	if (fromindex < toindex) {
		fromindex++;
		if (fromindex < toindex) Round(k, fromindex, KeyR, KeyL, toindex);
	}
	else {
		fromindex--;
		if(fromindex > toindex) Round(k, fromindex, KeyR, KeyL, toindex);
	}
}

void FL_function(u_char X[8], u_char kl[8])
{
	//cout << "\n  FL_function" << endl;
	//Print("  L", X, 8);
	//Print("  kl", kl, 8);
    u_char Y[8], tmp1 = 0, tmp2 = 0, n;
    for(int i = 3; i >= 0; i--) {
        tmp1 = tmp2;
        Y[i+4] = (X[i] & kl[i]);
        tmp2 = (Y[i+4] & 0x80) >> 7;
        if (i != 3) {
			n = i + 4;
            Y[i+4] = ((Y[i+4] << 1) | tmp1) ^ X[i+4];
            Y[i] = (Y[i+4] | kl[i+4]) ^ X[i];
        }
    }
    Y[7] = ((Y[7] << 1) | tmp2) ^ X[7];
    Y[3] = (Y[7] | kl[7]) ^ X[3];
	for (int i = 0; i < 8; i++) X[i] = Y[i];
	//Print("  return FL", Y, 8);
}

void invFL_function(u_char Y[8], u_char kl[8])
{
	//cout << "\n  invFL_function" << endl;
	//Print("  R", Y, 8);
	//Print("  kl", kl, 8);
    u_char X[8], tmp1 = 0, tmp2 = 0;
    for(int i = 3; i >= 0; i--) {
        tmp1 = tmp2;
        X[i] = (Y[i+4] | kl[i+4]) ^ Y[i];
        X[i+4] = (X[i] & kl[i]);
        tmp2 = (X[i+4] & 0x80) >> 7;
        if (i != 3) X[i+4] = ((X[i+4] << 1) | tmp1) ^ Y[i+4];
    }
    X[7] = ((X[7] << 1) | tmp2) ^ Y[7];
	for (int i = 0; i < 8; i++) Y[i] = X[i];
	//Print("  return invFL", X, 8);
}

void GenSubKeys(u_char Key[16], u_char kw[4][8], u_char k[18][8], u_char kl[4][8])
{
	cout << "GenKey:" << endl;
    u_char KaL[8], KaR[8], KA[16];
    for(int i = 0; i< 8; i++) {
        KaL[i] = Key[i];
        KaR[i] = Key[i + 8];
    }
    Round(constan, 0, KaL, KaR, 2);
    XOR64(KaL, Key, KaL);
    for(int i = 0; i < 8; i++) KaR[i] = KaR[i] ^ Key[i + 8];
    Round(constan, 2, KaL, KaR, 4);
    for(int i = 0; i < 8; i++) {
		KA[i] = KaL[i];
		KA[i + 8] = KaR[i];
        kw[0][i] = Key[i];
        kw[1][i] = Key[i + 8];
        k[0][i] = KaL[i];
        k[1][i] = KaR[i];
    }
	Print("   KA", KA, 16);
    LeftCirRotation(k[2], Key, 15, 0);
    LeftCirRotation(k[3], Key, 15, 8);
    LeftCirRotation(k[4], KA, 15, 0);
    LeftCirRotation(k[5], KA, 15, 8);
    
    LeftCirRotation(kl[0], KA, 30, 0);
    LeftCirRotation(kl[1], KA, 30, 8);
    
    LeftCirRotation(k[6], Key, 45, 0);
    LeftCirRotation(k[7], Key, 45, 8);
    LeftCirRotation(k[8], KA, 45, 0);
    LeftCirRotation(k[9], Key, 60, 8);
    LeftCirRotation(k[10], KA, 60, 0);
    LeftCirRotation(k[11], KA, 60, 8);
    
    LeftCirRotation(kl[2], Key, 77, 0);
    LeftCirRotation(kl[3], Key, 77, 8);
    
    LeftCirRotation(k[12], Key, 94, 0);
    LeftCirRotation(k[13], Key, 94, 8);
    LeftCirRotation(k[14], KA, 94, 0);
    LeftCirRotation(k[15], KA, 94, 8);
    LeftCirRotation(k[16], Key, 111, 0);
    LeftCirRotation(k[17], Key, 111, 8);
    
    LeftCirRotation(kw[2], KA, 111, 0);
    LeftCirRotation(kw[3], KA, 111, 8);
}

void CamelliaEncrypt(u_char M128[16], u_char kw64[4][8], u_char k64[18][8],
                     u_char kl64[4][8], u_char C128[16])
{
    u_char L[8], R[8];
    for(int i = 0; i< 8; i++) {
        L[i] = kw64[0][i] ^ M128[i];
        R[i] = kw64[1][i] ^ M128[i + 8];
    }
	//Print("  L", L, 8);
	//Print("  R", R, 8);
    Round(k64, 0, L, R, 6);
    FL_function(L, kl64[0]);
    invFL_function(R, kl64[1]);
    Round(k64, 6, L, R, 12);
    FL_function(L, kl64[2]);
    invFL_function(R, kl64[3]);
    Round(k64, 12, L, R, 18);
    for(int i = 0; i < 8; i++) {
        C128[i] = kw64[2][i] ^ R[i];
        C128[i + 8] = kw64[3][i] ^ L[i];
    }
}

void CamelliaDecryption(u_char C128[16], u_char kw64[4][8], u_char k64[18][8],
	u_char kl64[4][8], u_char M128[16])
{
	u_char L[8], R[8];
	for (int i = 0; i< 8; i++) {
		R[i] = kw64[2][i] ^ C128[i];
		L[i] = kw64[3][i] ^ C128[i + 8];
	}
	//Print("L", L, 8);
	//Print("R", R, 8);
	Round(k64, 17, R, L, 11);
	FL_function(R, kl64[3]);
	invFL_function(L, kl64[2]);
	Round(k64, 11, R, L, 5);
	FL_function(R, kl64[1]);
	invFL_function(L, kl64[0]);
	Round(k64, 5, R, L, -1);
	for (int i = 0; i < 8; i++) {
		M128[i] = kw64[0][i] ^ L[i];
		M128[i + 8] = kw64[2][i] ^ R[i];
	}
}


int main()
{
   /* u_char Key128[16] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
    },
    M128[16] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
    },
    C128[16];*/

	u_char Key[4][16] = {
		{ 0x41, 0x49, 0xd2, 0xad, 0xed, 0x94, 0x56, 0x68,
		0x1e, 0xc8, 0xb5, 0x11, 0xd9, 0xe7, 0xee, 0x04 },
		{ 0x47, 0xe8, 0xfb, 0x06, 0x3d, 0xd4, 0xfe, 0x4a,
		0xb4, 0x30, 0xa7, 0x3a, 0xf7, 0x72, 0x02, 0x06 },
		{ 0x40, 0xbc, 0x89, 0x81, 0x24, 0x19, 0x54, 0xa6,
		0x0a, 0x94, 0x2b, 0x4a, 0x43, 0x34, 0xd1, 0xdb },
		{ 0x3d, 0xa9, 0x3f, 0x26, 0x79, 0xde, 0xcb, 0x10,
		0x44, 0x22, 0xe0, 0x73, 0x32, 0xf7, 0xe3, 0xfe }
	},
	M[4][16] = {
		{ 0x2a, 0x9b, 0x0b, 0x74, 0xf4, 0xc5, 0xdc, 0x62,
		0x39, 0xb7, 0x06, 0x3a, 0x50, 0xa7, 0x94, 0x6e },
		{ 0x0f, 0x9d, 0x74, 0xfc, 0x31, 0xca, 0x65, 0x4f,
		0x92, 0x1a, 0x60, 0x6c, 0x02, 0x4e, 0x70, 0x84 },
		{ 0x98, 0x04, 0x8d, 0xd5, 0xd9, 0x8b, 0x1f, 0x8d,
		0xbb, 0xc6, 0xc7, 0xb2, 0x38, 0xc9, 0xb9, 0x48 },
		{ 0xcc, 0xa0, 0xf0, 0xf0, 0xba, 0x45, 0x96, 0xc4,
		0xd9, 0xc1, 0x0e, 0x1c, 0xf5, 0xdf, 0xf8, 0x2e }
	},
	C[4][16];

    u_char kw64[4][8], k64[18][8], kl64[4][8];
    
	for (int i = 0; i < 4; i++) {
		Print("Key", Key[i], 16);
		Print("M", M[i], 16);

		GenSubKeys(Key[i], kw64, k64, kl64);
		cout << "Encryption:\n";
		CamelliaEncrypt(M[i], kw64, k64, kl64, C[i]);

		Print("   M", M[i], 16);
		Print("   C", C[i], 16);

		cout << "Decryption:" << endl;
		CamelliaDecryption(C[i], kw64, k64, kl64, M[i]);

		Print("   C", C[i], 16);
		Print("   M", M[i], 16);
		cout << endl << endl;
	}

	/*Print("Key", Key128, 16);
	Print("M", M128, 16);

	GenSubKeys(Key128, kw64, k64, kl64);
	cout << "kw:" << endl;
	for (int i = 0; i < 4; i++) {
		printf("  kw[%d]", i);
		Print("", kw64[i], 8);
	}
	cout << "kl:" << endl;
	for (int i = 0; i < 4; i++) {
		printf("  kl[%d]", i);
		Print("", kl64[i], 8);
	}
	cout << "k:" << endl;
	for (int i = 0; i < 18; i++) {
		printf("  k[%d]", i);
		Print("", k64[i], 8);
	}
	cout << "Encryption\n";
    CamelliaEncrypt(M128, kw64, k64, kl64, C128);

	Print("M", M128, 16);
	Print("C", C128, 16);

	cout << "Decryption" << endl;
	CamelliaDecryption(C128, kw64, k64, kl64, M128);
	
	Print("C", C128, 16);
	Print("M", M128, 16);*/

	//u_char S_box2[256];
	//u_char S_box3[256];
	//u_char S_box4[256];
	//for (int i = 0; i < 256; i++) {
	//	S_box2[i] = (S_box[i] << 1) | ((S_box[i] & 0x80) >> 7);
	//	S_box3[i] = (S_box[i] << 7) | ((S_box[i] & 0xfe) >> 1);
	//	S_box4[i] = S_box[(i << 1 & 0xff) | ((i & 0x80) >> 7)];
	//}
	/*cout << endl << endl;
	for (int i = 0; i < 256; i+=16) {
		for (int j = 0; j < 16; j++)
			printf("%4.0d, ", S_box[i +j]);
		cout << endl;
	}cout << endl;*/
	//for (int i = 0; i < 16; i++) {
	//	for (int j = 0; j < 16; j++)
	//		printf("%x, ", S_box3[(i << 4) + j]);
	//	cout << endl;
	//}cout << endl;
	//for (int i = 0; i < 16; i++) { //cout << i << ": ";
	//	for (int j = 0; j < 16; j++)
	//		printf("%x, ", S_box4[(i << 4) + j]);
	//	cout << endl;
	//}
     
	system("pause");
    return 0;
}



/*
 void SubBytes(u_char *state, u_char *S_box)
 {
 for(int i =0; i < 16; i++) {
 state[i] = S_box[state[i]];
 }
 }
 
 void ShiftRows(u_char *state)
 {
 u_char tmp;
 tmp = state[1];
 state[1] = state[5];
 state[5] = state[9];
 state[9] = state[13];
 state[13] = tmp;
 
 tmp = state[2];
 state[2] = state[10];
 state[10] = tmp;
 tmp = state[6];
 state[6] = state[14];
 state[14] = tmp;
 
 tmp = state[3];
 state[3] = state[15];
 state[15] = state[11];
 state[11] = state[7];
 state[7] = tmp;
 }
 //u_char tmp[3], index;
 //for(int i = 1; i < 4; i++) {
 //index = i;
 //for(; j <= i + 4 * (i%4); j+=4) {
 tmp[j] = state[j];
 state[j] = state[i + 4 * (i % 4)];
 }
 for(;j <= i+12; i+=4) {
 state[j] = tmp[
 }
 }
 
 void MixColumns(u_char *state)
 {
 u_char mulMas[16] = {
 0x02, 0x03, 0x01, 0x01,
 0x01, 0x02, 0x03, 0x01,
 0x01, 0x01, 0x02, 0x03,
 0x03, 0x01, 0x01, 0x02
 }
 for(int i = 0; i < 16; i+=4)
 for(int j = 0; j < 4; j++)
 state[i + j] = (state[i + j] * mulMas[j]) & (state[i + j + 1] * mulMas[j+1]) &
 (state[i+j+2] * mulMas[j+2]) & (state[i+j+3] * mulMas[j+3]);
 }
 
 void AddRoundKey()
 {
 
 }
 
 void EncryptAES
 {
 
 }
 
 int main() {
 
 unsigned char S_box[256] = {
 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
 };
 
 return 0;
 }
 */