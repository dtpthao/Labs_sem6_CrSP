#include <iostream>
#include <Windows.h>
#include <iomanip>

using namespace std;
typedef unsigned char u_char;

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

DWORD alpha[256] = {
	0x00000000, 0xE19FCF13, 0x6B973726, 0x8A08F835, 0xD6876E4C, 0x3718A15F, 0xBD10596A, 0x5C8F9679,
	0x05A7DC98, 0xE438138B, 0x6E30EBBE, 0x8FAF24AD, 0xD320B2D4, 0x32BF7DC7, 0xB8B785F2, 0x59284AE1,
	0x0AE71199, 0xEB78DE8A, 0x617026BF, 0x80EFE9AC, 0xDC607FD5, 0x3DFFB0C6, 0xB7F748F3, 0x566887E0,
	0x0F40CD01, 0xEEDF0212, 0x64D7FA27, 0x85483534, 0xD9C7A34D, 0x38586C5E, 0xB250946B, 0x53CF5B78,
	0x1467229B, 0xF5F8ED88, 0x7FF015BD, 0x9E6FDAAE, 0xC2E04CD7, 0x237F83C4, 0xA9777BF1, 0x48E8B4E2,
	0x11C0FE03, 0xF05F3110, 0x7A57C925, 0x9BC80636, 0xC747904F, 0x26D85F5C, 0xACD0A769, 0x4D4F687A,
	0x1E803302, 0xFF1FFC11, 0x75170424, 0x9488CB37, 0xC8075D4E, 0x2998925D, 0xA3906A68, 0x420FA57B,
	0x1B27EF9A, 0xFAB82089, 0x70B0D8BC, 0x912F17AF, 0xCDA081D6, 0x2C3F4EC5, 0xA637B6F0, 0x47A879E3,
	0x28CE449F, 0xC9518B8C, 0x435973B9, 0xA2C6BCAA, 0xFE492AD3, 0x1FD6E5C0, 0x95DE1DF5, 0x7441D2E6,
	0x2D699807, 0xCCF65714, 0x46FEAF21, 0xA7616032, 0xFBEEF64B, 0x1A713958, 0x9079C16D, 0x71E60E7E,
	0x22295506, 0xC3B69A15, 0x49BE6220, 0xA821AD33, 0xF4AE3B4A, 0x1531F459, 0x9F390C6C, 0x7EA6C37F,
	0x278E899E, 0xC611468D, 0x4C19BEB8, 0xAD8671AB, 0xF109E7D2, 0x109628C1, 0x9A9ED0F4, 0x7B011FE7,
	0x3CA96604, 0xDD36A917, 0x573E5122, 0xB6A19E31, 0xEA2E0848, 0x0BB1C75B, 0x81B93F6E, 0x6026F07D,
	0x390EBA9C, 0xD891758F, 0x52998DBA, 0xB30642A9, 0xEF89D4D0, 0x0E161BC3, 0x841EE3F6, 0x65812CE5,
	0x364E779D, 0xD7D1B88E, 0x5DD940BB, 0xBC468FA8, 0xE0C919D1, 0x0156D6C2, 0x8B5E2EF7, 0x6AC1E1E4,
	0x33E9AB05, 0xD2766416, 0x587E9C23, 0xB9E15330, 0xE56EC549, 0x04F10A5A, 0x8EF9F26F, 0x6F663D7C,
	0x50358897, 0xB1AA4784, 0x3BA2BFB1, 0xDA3D70A2, 0x86B2E6DB, 0x672D29C8, 0xED25D1FD, 0x0CBA1EEE,
	0x5592540F, 0xB40D9B1C, 0x3E056329, 0xDF9AAC3A, 0x83153A43, 0x628AF550, 0xE8820D65, 0x091DC276,
	0x5AD2990E, 0xBB4D561D, 0x3145AE28, 0xD0DA613B, 0x8C55F742, 0x6DCA3851, 0xE7C2C064, 0x065D0F77,
	0x5F754596, 0xBEEA8A85, 0x34E272B0, 0xD57DBDA3, 0x89F22BDA, 0x686DE4C9, 0xE2651CFC, 0x03FAD3EF,
	0x4452AA0C, 0xA5CD651F, 0x2FC59D2A, 0xCE5A5239, 0x92D5C440, 0x734A0B53, 0xF942F366, 0x18DD3C75,
	0x41F57694, 0xA06AB987, 0x2A6241B2, 0xCBFD8EA1, 0x977218D8, 0x76EDD7CB, 0xFCE52FFE, 0x1D7AE0ED,
	0x4EB5BB95, 0xAF2A7486, 0x25228CB3, 0xC4BD43A0, 0x9832D5D9, 0x79AD1ACA, 0xF3A5E2FF, 0x123A2DEC,
	0x4B12670D, 0xAA8DA81E, 0x2085502B, 0xC11A9F38, 0x9D950941, 0x7C0AC652, 0xF6023E67, 0x179DF174,
	0x78FBCC08, 0x9964031B, 0x136CFB2E, 0xF2F3343D, 0xAE7CA244, 0x4FE36D57, 0xC5EB9562, 0x24745A71,
	0x7D5C1090, 0x9CC3DF83, 0x16CB27B6, 0xF754E8A5, 0xABDB7EDC, 0x4A44B1CF, 0xC04C49FA, 0x21D386E9,
	0x721CDD91, 0x93831282, 0x198BEAB7, 0xF81425A4, 0xA49BB3DD, 0x45047CCE, 0xCF0C84FB, 0x2E934BE8,
	0x77BB0109, 0x9624CE1A, 0x1C2C362F, 0xFDB3F93C, 0xA13C6F45, 0x40A3A056, 0xCAAB5863, 0x2B349770,
	0x6C9CEE93, 0x8D032180, 0x070BD9B5, 0xE69416A6, 0xBA1B80DF, 0x5B844FCC, 0xD18CB7F9, 0x301378EA,
	0x693B320B, 0x88A4FD18, 0x02AC052D, 0xE333CA3E, 0xBFBC5C47, 0x5E239354, 0xD42B6B61, 0x35B4A472,
	0x667BFF0A, 0x87E43019, 0x0DECC82C, 0xEC73073F, 0xB0FC9146, 0x51635E55, 0xDB6BA660, 0x3AF46973,
	0x63DC2392, 0x8243EC81, 0x084B14B4, 0xE9D4DBA7, 0xB55B4DDE, 0x54C482CD, 0xDECC7AF8, 0x3F53B5EB 
};

DWORD inv_alpha[256] = {
	0x00000000, 0x180F40CD, 0x301E8033, 0x2811C0FE, 0x603CA966, 0x7833E9AB, 0x50222955, 0x482D6998,
	0xC078FBCC, 0xD877BB01, 0xF0667BFF, 0xE8693B32, 0xA04452AA, 0xB84B1267, 0x905AD299, 0x88559254,
	0x29F05F31, 0x31FF1FFC, 0x19EEDF02, 0x01E19FCF, 0x49CCF657, 0x51C3B69A, 0x79D27664, 0x61DD36A9,
	0xE988A4FD, 0xF187E430, 0xD99624CE, 0xC1996403, 0x89B40D9B, 0x91BB4D56, 0xB9AA8DA8, 0xA1A5CD65,
	0x5249BE62, 0x4A46FEAF, 0x62573E51, 0x7A587E9C, 0x32751704, 0x2A7A57C9, 0x026B9737, 0x1A64D7FA,
	0x923145AE, 0x8A3E0563, 0xA22FC59D, 0xBA208550, 0xF20DECC8, 0xEA02AC05, 0xC2136CFB, 0xDA1C2C36,
	0x7BB9E153, 0x63B6A19E, 0x4BA76160, 0x53A821AD, 0x1B854835, 0x038A08F8, 0x2B9BC806, 0x339488CB,
	0xBBC11A9F, 0xA3CE5A52, 0x8BDF9AAC, 0x93D0DA61, 0xDBFDB3F9, 0xC3F2F334, 0xEBE333CA, 0xF3EC7307,
	0xA492D5C4, 0xBC9D9509, 0x948C55F7, 0x8C83153A, 0xC4AE7CA2, 0xDCA13C6F, 0xF4B0FC91, 0xECBFBC5C,
	0x64EA2E08, 0x7CE56EC5, 0x54F4AE3B, 0x4CFBEEF6, 0x04D6876E, 0x1CD9C7A3, 0x34C8075D, 0x2CC74790,
	0x8D628AF5, 0x956DCA38, 0xBD7C0AC6, 0xA5734A0B, 0xED5E2393, 0xF551635E, 0xDD40A3A0, 0xC54FE36D,
	0x4D1A7139, 0x551531F4, 0x7D04F10A, 0x650BB1C7, 0x2D26D85F, 0x35299892, 0x1D38586C, 0x053718A1,
	0xF6DB6BA6, 0xEED42B6B, 0xC6C5EB95, 0xDECAAB58, 0x96E7C2C0, 0x8EE8820D, 0xA6F942F3, 0xBEF6023E,
	0x36A3906A, 0x2EACD0A7, 0x06BD1059, 0x1EB25094, 0x569F390C, 0x4E9079C1, 0x6681B93F, 0x7E8EF9F2,
	0xDF2B3497, 0xC724745A, 0xEF35B4A4, 0xF73AF469, 0xBF179DF1, 0xA718DD3C, 0x8F091DC2, 0x97065D0F,
	0x1F53CF5B, 0x075C8F96, 0x2F4D4F68, 0x37420FA5, 0x7F6F663D, 0x676026F0, 0x4F71E60E, 0x577EA6C3,
	0xE18D0321, 0xF98243EC, 0xD1938312, 0xC99CC3DF, 0x81B1AA47, 0x99BEEA8A, 0xB1AF2A74, 0xA9A06AB9,
	0x21F5F8ED, 0x39FAB820, 0x11EB78DE, 0x09E43813, 0x41C9518B, 0x59C61146, 0x71D7D1B8, 0x69D89175,
	0xC87D5C10, 0xD0721CDD, 0xF863DC23, 0xE06C9CEE, 0xA841F576, 0xB04EB5BB, 0x985F7545, 0x80503588,
	0x0805A7DC, 0x100AE711, 0x381B27EF, 0x20146722, 0x68390EBA, 0x70364E77, 0x58278E89, 0x4028CE44,
	0xB3C4BD43, 0xABCBFD8E, 0x83DA3D70, 0x9BD57DBD, 0xD3F81425, 0xCBF754E8, 0xE3E69416, 0xFBE9D4DB,
	0x73BC468F, 0x6BB30642, 0x43A2C6BC, 0x5BAD8671, 0x1380EFE9, 0x0B8FAF24, 0x239E6FDA, 0x3B912F17,
	0x9A34E272, 0x823BA2BF, 0xAA2A6241, 0xB225228C, 0xFA084B14, 0xE2070BD9, 0xCA16CB27, 0xD2198BEA,
	0x5A4C19BE, 0x42435973, 0x6A52998D, 0x725DD940, 0x3A70B0D8, 0x227FF015, 0x0A6E30EB, 0x12617026,
	0x451FD6E5, 0x5D109628, 0x750156D6, 0x6D0E161B, 0x25237F83, 0x3D2C3F4E, 0x153DFFB0, 0x0D32BF7D,
	0x85672D29, 0x9D686DE4, 0xB579AD1A, 0xAD76EDD7, 0xE55B844F, 0xFD54C482, 0xD545047C, 0xCD4A44B1,
	0x6CEF89D4, 0x74E0C919, 0x5CF109E7, 0x44FE492A, 0x0CD320B2, 0x14DC607F, 0x3CCDA081, 0x24C2E04C,
	0xAC977218, 0xB49832D5, 0x9C89F22B, 0x8486B2E6, 0xCCABDB7E, 0xD4A49BB3, 0xFCB55B4D, 0xE4BA1B80,
	0x17566887, 0x0F59284A, 0x2748E8B4, 0x3F47A879, 0x776AC1E1, 0x6F65812C, 0x477441D2, 0x5F7B011F,
	0xD72E934B, 0xCF21D386, 0xE7301378, 0xFF3F53B5, 0xB7123A2D, 0xAF1D7AE0, 0x870CBA1E, 0x9F03FAD3,
	0x3EA637B6, 0x26A9777B, 0x0EB8B785, 0x16B7F748, 0x5E9A9ED0, 0x4695DE1D, 0x6E841EE3, 0x768B5E2E,
	0xFEDECC7A, 0xE6D18CB7, 0xCEC04C49, 0xD6CF0C84, 0x9EE2651C, 0x86ED25D1, 0xAEFCE52F, 0xB6F3A5E2 
};

#define AlphaMul(w) (w << 8) ^ alpha[w >> 24]
#define AlphaInvMul(y) (y >> 8) ^ inv_alpha[y & 0xff]
#define FSM(x,y,z) (x+y)^z

struct State {
	DWORD a[16];
	DWORD b[2];
};

void PrintState(State S, int time)
{
	cout << "State S[" << time << "]:\n";
	for (int i = 15; i >= 0; i--) {
		printf("%02d: 0x%08x ", i, S.a[i]);
		if (i == 8) cout << endl;
	}
	printf("\nb1: 0x%08x b2: 0x%08x\n\n", S.b[0], S.b[1]);
}

#define Mul02(x) ((x & 0x80) ? (x<<1)^0x11B : x<<1)
#define Mul03(x) (Mul02(x)^x)

DWORD T(DWORD w)
{
	/*u_char mulMas[16] = {
		0x02, 0x03, 0x01, 0x01,
		0x01, 0x02, 0x03, 0x01,
		0x01, 0x01, 0x02, 0x03,
		0x03, 0x01, 0x01, 0x02
	};*/
	DWORD Sw[4] = { 0 }, r[4] = { 0 };
	Sw[0] = S_box[w & 0xff];
	Sw[1] = S_box[(w & 0xff00) >> 8];
	Sw[2] = S_box[(w & 0xff0000) >> 16];
	Sw[3] = S_box[(w & 0xff000000) >> 24];
	r[0] = Mul02(Sw[0]) ^ Mul03(Sw[1]) ^ Sw[2] ^ Sw[3];
	r[1] = Sw[0] ^ Mul02(Sw[1]) ^ Mul03(Sw[2]) ^ Sw[3];
	r[2] = Sw[0] ^ Sw[1] ^ Mul02(Sw[2]) ^ Mul03(Sw[3]);
	r[3] = Mul03(Sw[0]) ^ Sw[1] ^ Sw[2] ^ Mul02(Sw[3]);
	return r[0] ^ (r[1] << 8) ^ (r[2] << 16) ^ (r[3] << 24);
}

State Next(State S, bool mode, int time)
{
	State S1;
	if (time == -29);
	S1.b[1] = T(S.b[0]);
	S1.b[0] = (S.b[1] + S.a[5]) & 0xffffffff;
	for (int i = 0; i < 15; i++) {
		S1.a[i] = S.a[i + 1];
	}
	S1.a[15] = AlphaMul(S.a[0]) ^ S.a[2] ^ AlphaInvMul(S.a[11]);
	if (mode) {
		S1.a[15] ^= FSM(S.a[15], S.b[0], S.b[1]);
		//PrintState(S1, time);
		if (time < -1) S1 = Next(S1, 1, time + 1);
	}
	//else PrintState(S1, time);
	return S1;
}

State Init(DWORD K[4], DWORD IV[16])
{
	State S34, S33, S1, S0;
	for (int i = 0; i < 4; i++) {
		S34.a[15 - i] = S34.a[15 - i - 8] = K[3 - i];
		S34.a[15 - i - 4] = S34.a[15 - i - 12] = ~K[3 - i];
	}
	for (int i = 0; i < 15; i++) {
		S33.a[i] = S34.a[i];
	}
	S33.a[15] = S34.a[15] ^ IV[0];
	S33.a[12] = S34.a[12] ^ IV[1];
	S33.a[10] = S34.a[10] ^ IV[2];
	S33.a[9] = S34.a[9] ^ IV[3];
	S33.b[0] = S33.b[1] = 0;
	//PrintState(S34, -34);
	//PrintState(S33, -33);
	S1 = Next(S33, 1, -32);
	return Next(S1, 0, 0);
}

DWORD Strm(State S) {
	return FSM(S.a[15], S.b[0], S.b[1]) ^ S.a[0];
}

int main()
{
	//DWORD key[4] = { 0x00000000, 0x00000000, 0x00000000, 0x80000000};
	DWORD key[2][4] = { { 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA },
						{ 0x00000000, 0x00000000, 0x00000000, 0x80000000 } };
	//DWORD IV[4] = {0};
	DWORD IV[2][4] = { {0}, { 1,2,3,4 } };
	DWORD Z[8] = { 0 };
	State S[8];
	//S[0]= Init(key, IV);
	//Z[0] = Strm(S[0]);
	/*for (int i = 1; i < 8; i++) {
		S[i] = Next(S[i - 1], 0, i);
		Z[i] = Strm(S[i]);
	}
	cout << "Z: ";
	for (int i = 0; i < 8; i++) {
		printf("%08x ", Z[i]);
	}
	cout << endl;*/

	for (int ik = 0; ik < 2; ik++) {
		for (int iI = 0; iI < 2; iI++) {
			printf("Key: %08x %08x %08x %08x\n", key[ik][0], key[ik][1], key[ik][2], key[ik][3]);
			printf("IV : %08x %08x %08x %08x\n", IV[iI][0], IV[iI][1], IV[iI][2], IV[iI][3]);
			S[0] = Init(key[ik], IV[iI]);
			Z[0] = Strm(S[0]);
			for (int i = 1; i < 8; i++) {
				S[i] = Next(S[i - 1], 0, i);
				Z[i] = Strm(S[i]);
			}
			cout << "Z: ";
			for (int i = 0; i < 8; i++) {
				printf("%08X ", Z[i]);
			}
			cout << endl << endl;
		}
		
	}
	


	system("pause");
	return 0;
}

