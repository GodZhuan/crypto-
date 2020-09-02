#define _CRT_RAND_S  
#include <stdlib.h>  
#include<iostream>
#include <fstream>
#include"aes.h"
#include"ecc.h"
#include"tools.h"
#include "sts.h"
#include "sha256.h"

using namespace std;


string GetRandList(int len)
{
	unsigned int number;
	int err;
	char strRandomList[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '!', '@', '#', '$', '%', '&', '*', '_' };
	string pwd = "";
	for (int i = 0; i < len; i++)
	{
		err = rand_s(&number);
		if (err != 0) {
			printf_s("The rand_s function failed!\n");
		}
		pwd += strRandomList[number%70];//���ȡstrRandomList ����ֵ
	}
	return pwd;
}

void ex_Eulid(mp_int* a, mp_int* b, mp_int* a1, mp_int* b1, mp_int* temp3) {
	if (mp_cmp_d(b, 0) == 0) {
		mp_set(a1, 1);
		mp_set(b1, 0);
		mp_copy(temp3, a);
	}
	else {
		mp_int temp1;
		mp_int temp2;
		mp_init(&temp1);
		mp_init(&temp2);
		mp_mod(a, b, &temp1);
		ex_Eulid(b, &temp1, a1, b1, temp3);
		mp_copy(a1, &temp1);
		mp_copy(b1, a1);
		mp_div(a, b, temp3, &temp2);
		mp_mul(temp3, b1, temp3);
		mp_sub(&temp1, temp3, b1);
	}
}


int main(int argc, char* argv[])
{

	SHA256 sha256;
	int index,enDoIndex, ret;
	char szFullPath[_MAX_PATH], szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFileName[_MAX_FNAME], szExt[_MAX_EXT];
	std::string fullPath,dirPath;
	cout << "crypto++" << endl;
	cout << "1.AES�ӽ���" << endl;
	cout << "2.ECC�ӽ���" << endl;
	cout << "3.ECDSAǩ����STS��ԿЭ��Э��" << endl;
	cout << "4.ElGamal����ǩ��" << endl;
	cout << "5.SHA256��ϣɢ��" << endl;
	cout << "6.RC4��ʽ�ӽ���" << endl;
	cout << "6.DES�ӽ���" << endl;
	cin >> index;
	cout << "������Ҫ�����ļ���λ��" << endl;
	cin >> szFullPath;
	ret = _splitpath_s(szFullPath, szDrive, szDrive ? _MAX_DRIVE : 0, szDir, szDir ? _MAX_DIR : 0, szFileName, szFileName ? _MAX_FNAME : 0, szExt, szExt ? _MAX_EXT : 0);
	if (ret)return ret;
	cout << "1.����" << endl;
	cout << "2.����" << endl;
	cin >> enDoIndex;
	dirPath += szDrive ? szDrive : "";
	dirPath += szDir ? szDir : "";
	fullPath = dirPath;
	switch (enDoIndex)
	{
	case 1:
		fullPath += szFileName ? szFileName : "";
		fullPath += "cipher.txt";
		break;
	case 2:
		std::string outFName;
		cout << "������ܺ���ļ���������hello.cpp��" << endl;
		cin >> outFName;
		fullPath += outFName;
		break;
	}
	switch (index)
	{
	case 1: {
		AES a;
		string keyStr;
		byte key[16];
		// ��Կ��չ
		word w[4 * (Nr + 1)];
		bitset<128> data;
		byte plain[16];
		ifstream in;
		ofstream out;
		switch (enDoIndex)
		{
		case 1:
			keyStr = GetRandList(16);
			cout <<"��ԿΪ��"<< keyStr<<"(��ע�⸴�Ʊ���)"<<endl;
			charToByte(key, keyStr);
			a.KeyExpansion(key, w);
			in.open(szFullPath, ios::binary);
			out.open(fullPath, ios::binary);
			while (in.read((char*)&data, sizeof(data)))
			{
				divideToByte(plain, data);
				a.encrypt(plain, w);
				data = mergeByte(plain);
				out.write((char*)&data, sizeof(data));
				data.reset();  // ��0
			}
			in.close();
			out.close();
			cout << "press any key to shutdown" << endl;
			std::cin.get();
			break;
		case 2:
			cout << "��������Կ��";
			cin >> keyStr;
			if (keyStr.size() == 16) {
				charToByte(key, keyStr);
				a.KeyExpansion(key, w);
				// ���� cipher.txt����д��ͼƬ flower1.jpg
				in.open(szFullPath, ios::binary);
				out.open(fullPath, ios::binary);
				while (in.read((char*)&data, sizeof(data)))
				{
					divideToByte(plain, data);
					a.decrypt(plain, w);
					data = mergeByte(plain);
					out.write((char*)&data, sizeof(data));
					data.reset();  // ��0
				}
				in.close();
				out.close();
				cout << "press any key to shutdown" << endl;
				std::cin.get();
			}
			else {
				cout << "��Կ��������";
				cout << "press any key to shutdown" << endl;
				std::cin.get();
			}	
			break;
		}
	}
		  break;
	case 2: {
		ECC e;
		size_t written;

		cout << "\n          ������ʵ����Բ���ߵļ��ܽ���" << endl;

		cout << "\n------------------------------------------------------------------------\n" << endl;

		mp_int GX;
		mp_int GY;
		mp_int K;//˽����Կ
		mp_int A;
		mp_int B;
		mp_int QX;
		mp_int QY;
		mp_int P;//Fp�е�p(������P)


		mp_init(&GX);
		mp_init(&GY);
		mp_init(&K);
		mp_init(&A);
		mp_init(&B);
		mp_init(&QX);
		mp_init(&QY);
		mp_init(&P);
		char temp[800] = { 0 };
		char tempA[800] = { 0 };
		char tempB[800] = { 0 };
		char tempGX[800] = { 0 };
		char tempGY[800] = { 0 };
		char tempK[800] = { 0 };
		char tempQX[800] = { 0 };
		char tempQY[800] = { 0 };


		switch (enDoIndex)
		{
		case 1:
			time_t t;
			srand((unsigned)time(&t));

			printf("��Բ���ߵĲ�������(��ʮ������ʾ):\n");

			e.GetPrime(&P, P_LONG);
			printf("������ P ��:\n");
			
			mp_to_radix(&P, temp, SIZE_MAX, &written, 10);
			printf("%s\n", temp);

			e.GetPrime(&A, 30);

			printf("���߲��� A ��:\n");
			mp_to_radix(&A, tempA, SIZE_MAX, &written, 10);
			printf("%s\n", tempA);

			e.Get_B_X_Y(&GX, &GY, &B, &A, &P);

			
			printf("���߲��� B ��:\n");
			mp_to_radix(&B, tempB, SIZE_MAX, &written, 10);
			printf("%s\n", tempB);
			
			printf("����G��X������:\n");
			mp_to_radix(&GX, tempGX, SIZE_MAX, &written, 10);
			printf("%s\n", tempGX);
			
			printf("����G��Y������:\n");
			mp_to_radix(&GY, tempGY, SIZE_MAX, &written, 10);
			printf("%s\n", tempGY);
			if (!e.GetPrime(&K, KEY_LONG)) {
				printf("˽Կ K ��:\n");
				mp_to_radix(&K, tempK, SIZE_MAX, &written, 10);
				printf("%s\n", tempK);
			};

			e.Ecc_points_mul(&QX, &QY, &GX, &GY, &K, &A, &P);
			
			printf("��ԿX������:\n");
			mp_to_radix(&QX, tempQX, SIZE_MAX, &written, 10);
			printf("%s\n", tempQX);
			
			printf("��ԿY������:\n");
			mp_to_radix(&QY, tempQY, SIZE_MAX, &written, 10);
			printf("%s\n", tempQY);

			//������Կ����Կ�ļ������ļ���
			e.Ecc_saveKey(tempK, tempA, temp, dirPath);

			printf("\n------------------------------------------------------------------------\n");
			e.Ecc_encipher(&QX, &QY, &GX, &GY, &A, &P, szFullPath, fullPath);//����
			
			break;
		case 2:
			printf("\n------------------------------------------------------------------------\n");
			e.Ecc_loadKey(&K, &A, &P, dirPath);
			e.Ecc_decipher(&K, &A, &P, szFullPath, fullPath);//����

			break;
		}
		mp_clear(&GX);
		mp_clear(&GY);
		mp_clear(&K);//˽����Կ
		mp_clear(&A);
		mp_clear(&B);
		mp_clear(&QX);
		mp_clear(&QY);
		mp_clear(&P);//Fp�е�p(������P)
	}break;
	case 3: {
		ECC ecc;
		STS sts;
		int lon;
		mp_err(err);
		size_t written;

		mp_int p;//pΪ��ȫ����
		mp_int a;//����Ԫ
		mp_int p_1;//p-1

		mp_int rA;//�����rA
		mp_int sA;//a**rA mod p

		mp_int rB;//�����rB
		mp_int sB;//a**rB mod p

		mp_int K;//sA**rB mod p

		mp_int GX;//����G��x����
		mp_int GY;//����G��y����
		mp_int n;//����G�Ľ�
		mp_int d;//˽����Կ
		mp_int h;//h����Բ���������е�ĸ���m��n������̵���������
		mp_int k;//�����(2 to n-2)
		mp_int A;//����Epϵ��A
		mp_int B;//����Epϵ��B
		mp_int PX;//dG��x����
		mp_int PY;//dG��x����
		mp_int X1;//kG��x����
		mp_int Y1;//kG��y����
		mp_int u1X;//u1.GX��x����
		mp_int u1Y;//u1.GY��y����
		mp_int u2X;//u2.PX��x����
		mp_int u2Y;//u2.PY��y����
		mp_int X2;//u1.GX+u2.PX��x����
		mp_int Y2;//u2.PY+u2.PX��y����
		mp_int v;//x2 mod n
		bool zero = false;

		mp_int r;//x1 mod n
		mp_int P;//Fp�е�p(������P)

		mp_int k1;//k����Ԫ
		mp_int s1;//s����Ԫ
		mp_int n1;//n����Ԫ
		mp_int temp;
		mp_int Hm;//H(m)
		mp_int s;//k**-1(H(m)+dA*r) mod n

		mp_int u1;//H(m)s**(-1) mod n
		mp_int u2;//rs**(-1)mod n

		string path;
		const char* rP = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
		const char* rGX = "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
		const char* rGY = "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
		const char* rn = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";


		if ((err = mp_init(&Hm)) != MP_OKAY) {
			printf("Error initializing the Hm. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&s)) != MP_OKAY) {
			printf("Error initializing the s. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&p)) != MP_OKAY) {
			printf("Error initializing the p. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&a)) != MP_OKAY) {
			printf("Error initializing the a. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&rA)) != MP_OKAY) {
			printf("Error initializing the rA. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&sA)) != MP_OKAY) {
			printf("Error initializing the sA. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&rB)) != MP_OKAY) {
			printf("Error initializing the rB. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&sB)) != MP_OKAY) {
			printf("Error initializing the sB. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&K)) != MP_OKAY) {
			printf("Error initializing the K. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&GX)) != MP_OKAY) {
			printf("Error initializing the GX. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&GY)) != MP_OKAY) {
			printf("Error initializing the GY. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init_set(&n, 1)) != MP_OKAY) {
			printf("Error initializing the n. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init_set(&h, 1)) != MP_OKAY) {
			printf("Error initializing the h. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&d)) != MP_OKAY) {
			printf("Error initializing the d. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&k)) != MP_OKAY) {
			printf("Error initializing the k. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init_set(&A, 0)) != MP_OKAY) {
			printf("Error initializing the A. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init_set(&B, 7)) != MP_OKAY) {
			printf("Error initializing the B. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&PX)) != MP_OKAY) {
			printf("Error initializing the PX. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&PY)) != MP_OKAY) {
			printf("Error initializing the PY. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&X1)) != MP_OKAY) {
			printf("Error initializing the X1. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&Y1)) != MP_OKAY) {
			printf("Error initializing the Y1. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&u1X)) != MP_OKAY) {
			printf("Error initializing the u1X. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&u1Y)) != MP_OKAY) {
			printf("Error initializing the u1Y. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&u2X)) != MP_OKAY) {
			printf("Error initializing the u2X. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&u2Y)) != MP_OKAY) {
			printf("Error initializing the u2Y. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&X2)) != MP_OKAY) {
			printf("Error initializing the X2. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&Y2)) != MP_OKAY) {
			printf("Error initializing the Y2. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&v)) != MP_OKAY) {
			printf("Error initializing the v. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&r)) != MP_OKAY) {
			printf("Error initializing the r. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&P)) != MP_OKAY) {
			printf("Error initializing the P. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&k1)) != MP_OKAY) {
			printf("Error initializing the k1. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&s1)) != MP_OKAY) {
			printf("Error initializing the s1. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&n1)) != MP_OKAY) {
			printf("Error initializing the n1. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&temp)) != MP_OKAY) {
			printf("Error initializing the temp. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&u1)) != MP_OKAY) {
			printf("Error initializing the u1. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_init(&u2)) != MP_OKAY) {
			printf("Error initializing the u2. %s",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		n.alloc = 512;
		if ((err = mp_read_radix(&P, rP, 0x10)) != MP_OKAY) {
			printf("mp_read_radix failed: \"%s\"\n",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_read_radix(&GX, rGX, 0x10)) != MP_OKAY) {
			printf("mp_read_radix failed: \"%s\"\n",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_read_radix(&GY, rGY, 0x10)) != MP_OKAY) {
			printf("mp_read_radix failed: \"%s\"\n",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}
		if ((err = mp_read_radix(&n, rn, 0x10)) != MP_OKAY) {
			printf("mp_read_radix failed: \"%s\"\n",
				mp_error_to_string(err));
			return EXIT_FAILURE;
		}

		

		cout << "�������������λ����";
		cin >> lon;
		sts.GetPrime(&p, &a, lon);
		mp_init_copy(&p_1, &p);
		mp_sub_d(&p_1, 2, &p_1);

		do { mp_rand(&rA, lon); } while (mp_cmp(&rA, &p_1) != -1 && mp_cmp_d(&rA, 1) != 1);
		do { mp_rand(&rB, lon); } while (mp_cmp(&rB, &p_1) != -1 && mp_cmp_d(&rB, 1) != 1);
		mp_exptmod(&a, &rA, &p, &sA);
		mp_exptmod(&a, &rB, &p, &sB);
		mp_exptmod(&sA, &rB, &p, &K);

		char tempN[800] = { 0 };
		printf("����G�Ľ� ��:\n");
		mp_to_radix(&n, tempN, SIZE_MAX, &written, 10);
		printf("%s\n", tempN);

		do {
			ecc.GetPrime(&d, KEY_LONG);
		} while (mp_cmp(&d, &n) != -1);

		char tempD[800] = { 0 };
		printf("˽Կ d ��:\n");
		mp_to_radix(&d, tempD, SIZE_MAX, &written, 10);
		printf("%s\n", tempD);

	L:
		mp_rand(&k, 10);
		while (mp_cmp(&k, &n) == 1)
			mp_div_2(&k, &k);

		char tempK[800] = { 0 };
		printf("�����k��:\n");
		mp_to_radix(&k, tempK, SIZE_MAX, &written, 10);
		printf("%s\n", tempK);

		ecc.Ecc_points_mul(&PX, &PY, &GX, &GY, &d, &A, &P);
		ecc.Ecc_points_mul(&X1, &Y1, &GX, &GY, &k, &A, &P);
		char tempPX[800] = { 0 };
		printf("��ԿX������:\n");
		mp_to_radix(&PX, tempPX, SIZE_MAX, &written, 10);
		printf("%s\n", tempPX);

		char tempPY[800] = { 0 };
		printf("��ԿY������:\n");
		mp_to_radix(&PY, tempPY, SIZE_MAX, &written, 10);
		printf("%s\n", tempPY);

		mp_mod(&X1, &n, &r);
		if (mp_cmp_d(&r, 0) == 0)goto L;
		char tempR[800] = { 0 };
		printf("x1 mod n��:\n");
		mp_to_radix(&r, tempR, SIZE_MAX, &written, 10);
		printf("%s\n", tempR);

		ex_Eulid(&k, &n, &k1, &n1, &temp);

		char tempK1[800] = { 0 };
		printf("k**-1��:\n");
		while (mp_cmp_d(&k1, 0) != 1)
			mp_add(&k1, &n, &k1);
		mp_to_radix(&k1, tempK1, SIZE_MAX, &written, 10);
		printf("%s\n", tempK1);

		mp_mulmod(&k, &k1, &n, &temp);
		char tempT[800] = { 0 };
		printf("k*k**-1 mod n��:\n");
		mp_to_radix(&temp, tempT, SIZE_MAX, &written, 10);
		printf("%s\n", tempT);

		cout << "������Ҫ����ɢ��ֵ�ļ���λ�ã�";
		cin >> path;
		string sh = sha256.ShaFile(path);
		mp_read_radix(&Hm, sh.c_str(), 10);
		char tempSHA[800] = { 0 };
		printf("SHA��:\n");
		mp_to_radix(&Hm, tempSHA, SIZE_MAX, &written, 0x10);
		printf("%s\n", tempSHA);

		mp_mul(&d, &r, &temp);
		mp_add(&Hm, &temp, &temp);
		mp_mulmod(&k1, &temp, &n, &s);
		char tempS[800] = { 0 };
		printf("s ��:\n");
		mp_to_radix(&s, tempS, SIZE_MAX, &written, 10);
		printf("%s\n", tempS);

		if (mp_cmp_d(&s, 0) == 0)goto L;
		ex_Eulid(&s, &n, &s1, &n1, &temp);

		mp_mulmod(&s, &s1, &n, &temp);
		char tempS1[800] = { 0 };
		printf("s*s**-1 mod n��:\n");
		mp_to_radix(&temp, tempS1, SIZE_MAX, &written, 10);
		printf("%s\n", tempS1);

		if (mp_cmp(&r, &n) == -1 && mp_cmp_d(&r, 0) == 1) {
			if (mp_cmp(&s, &n) == -1 && mp_cmp_d(&s, 0) == 1) {
				mp_mulmod(&Hm, &s1, &n, &u1);
				mp_mulmod(&r, &s1, &n, &u2);
				ecc.Ecc_points_mul(&u1X, &u1Y, &GX, &GY, &u1, &A, &P);
				ecc.Ecc_points_mul(&u2X, &u2Y, &PX, &PY, &u2, &A, &P);
				ecc.Two_points_add(&u1X, &u1Y, &u2X, &u2Y, &X2, &Y2, &A, zero, &P);
				mp_mod(&X2, &n, &v);
				char tempV[800] = { 0 };
				printf("v ��:\n");
				mp_to_radix(&v, tempV, SIZE_MAX, &written, 10);
				printf("%s\n", tempV);
				char tempR[800] = { 0 };
				printf("r ��:\n");
				mp_to_radix(&r, tempR, SIZE_MAX, &written, 10);
				printf("%s\n", tempR);
				if (mp_cmp(&v, &r) == 0)cout << "����ǩ��";
			}
		}
		mp_clear(&Hm);
		mp_clear(&s);
		mp_clear(&p);
		mp_clear(&a);
		mp_clear(&rA);
		mp_clear(&sA);
		mp_clear(&rB);
		mp_clear(&sB);
		mp_clear(&K);
		mp_clear(&GX);
		mp_clear(&GY);
		mp_clear(&n);
		mp_clear(&h);
		mp_clear(&d);
		mp_clear(&k);
		mp_clear(&A);
		mp_clear(&B);
		mp_clear(&PX);
		mp_clear(&PY);
		mp_clear(&X1);
		mp_clear(&Y1);
		mp_clear(&u1X);
		mp_clear(&u1Y);
		mp_clear(&u2X);
		mp_clear(&u2Y);
		mp_clear(&X2);
		mp_clear(&Y2);
		mp_clear(&v);
		mp_clear(&r);
		mp_clear(&P);
		mp_clear(&k1);
		mp_clear(&s1);
		mp_clear(&n1);
		mp_clear(&temp);
		mp_clear(&u1);
		mp_clear(&u2);
	}break;
	case 4: {
		ECC ecc;
		STS sts;


		int lon;
		string path;//��Ϣm��·��	
		size_t written;
		mp_int p;//pΪ��ȫ����
		mp_int a;//����Ԫ
		mp_int p_1;//p-1
		mp_int x;//�����x
		mp_int y;//a**x mod p
		mp_int k;//k����Zp* ��k��p-1����
		mp_int r;//a**k mod p
		mp_int s;//a**x mod p
		mp_int sha;//sha256��ɢ��ֵ
		mp_int a1;//k����Ԫ
		mp_int b1;//p-1����Ԫ
		mp_int temp3;
		mp_init(&p);
		mp_init(&a);
		mp_init(&p_1);
		mp_init(&x);
		mp_init(&y);
		mp_init(&a);
		mp_init(&k);
		mp_init(&r);
		mp_init(&s);
		mp_init(&sha);
		mp_init(&a1);
		mp_init(&b1);
		mp_init(&temp3);
		cout << "�������������λ����";
		cin >> lon;
		sts.GetPrime(&p, &a, lon);
		mp_sub_d(&p, 1, &p_1);
		do { mp_rand(&x, lon); } while (mp_cmp(&x, &p_1) != -1 && mp_cmp_d(&x, 1) != 1);//1<=x<p-1
		mp_exptmod(&a, &x, &p, &y);
		char tempY[800] = { 0 };
		printf("y��:\n");
		mp_to_radix(&y, tempY, SIZE_MAX, &written, 10);
		printf("%s\n", tempY);
		do {
			mp_rand(&k, lon / 26);
			mp_gcd(&k, &p_1, &r);//����
		} while (mp_cmp(&k, &p_1) != -1 || mp_cmp_d(&k, 1) != 1 || mp_cmp_d(&r, 1) != 0);//1<=k<p-1��k��p-1����
		mp_exptmod(&a, &k, &p, &r);
		char tempR[800] = { 0 };
		printf("a**k mod p��:\n");
		mp_to_radix(&r, tempR, SIZE_MAX, &written, 10);
		printf("%s\n", tempR);
		//cout << "������Ҫ����ɢ��ֵ���ļ���λ�ã�";
		//cin >> path;
		string str = sha256.ShaFile(szFullPath);
		mp_read_radix(&sha, str.c_str(), 10);
		char tempSHA[800] = { 0 };
		printf("SHA��:\n");
		mp_to_radix(&sha, tempSHA, SIZE_MAX, &written, 0x10);
		printf("%s\n", tempSHA);
		mp_mul(&x, &r, &s);
		mp_sub(&sha, &s, &s);
		ex_Eulid(&k, &p_1, &a1, &b1, &temp3);
		while (mp_cmp_d(&a1, 0) != 1)
			mp_add(&a1, &p_1, &a1);
		char tempA1[800] = { 0 };
		printf("k**-1��:\n");
		mp_to_radix(&a1, tempA1, SIZE_MAX, &written, 10);
		printf("%s\n", tempA1);
		mp_mulmod(&k, &a1, &p_1, &temp3);
		char tempT[800] = { 0 };
		printf("k*k**-1 mod p��:\n");
		mp_to_radix(&a1, tempT, SIZE_MAX, &written, 10);
		printf("%s\n", tempT);
	}break;
	case 5: {
		string path;
		mp_int s;
		size_t written;
		mp_init(&s);
		cout << "������Ҫ����ɢ��ֵ�ļ���λ�ã�";
		cin >> path;
		string a = sha256.ShaFile(path);
		mp_read_radix(&s, a.c_str(), 10);
		char tempSHA[800] = { 0 };
		printf("SHA��:\n");
		mp_to_radix(&s, tempSHA, SIZE_MAX, &written, 0x10);
		printf("%s\n", tempSHA);

	}break;
	case 6: {
		string path;
		mp_int s;
		size_t written;
		mp_init(&s);
		cout << "������Ҫ����ɢ��ֵ�ļ���λ�ã�";
		cin >> path;
		string a = sha256.ShaFile(path);
		mp_read_radix(&s, a.c_str(), 10);
		char tempSHA[800] = { 0 };
		printf("SHA��:\n");
		mp_to_radix(&s, tempSHA, SIZE_MAX, &written, 0x10);
		printf("%s\n", tempSHA);

	}break;
	default:
		break;
	}
}