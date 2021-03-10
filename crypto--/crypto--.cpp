#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <chrono>
#include "aes.h"
#include "ecc.h"
#include "tools.h"
#include "sts.h"
#include "sha256.h"
#include "sm4.h"
#include "zuc.h"
#include "sm3.h"
#include "crypto--.h"
#include "fileproc.h"
#include "ecdsa.h"
using namespace crypto__;
using std::cout;
using std::cin;
mp_err err;
int main(int argc, char* argv[])
{
	if (argc < 4)return 1;
	//ѡ�����ļ���������Ϣ
	if (!strcmp(argv[1], "-f"))
		config.contentsTypeMode = contentsType::File;
	else if (!strcmp(argv[1], "-m"))
		config.contentsTypeMode = contentsType::Message;

	//ѡ����ܻ����ǽ��ܻ����ǹ�ϣ
	if (!strcmp(argv[2], "-e"))
		config.cryptoTypeMode = cryptoType::Encrypt;
	else if (!strcmp(argv[2], "-d"))
		config.cryptoTypeMode = cryptoType::Decrypt;
	else if (!strcmp(argv[2], "-h"))
		config.cryptoTypeMode = cryptoType::Hash;

	switch (config.cryptoTypeMode)
	{
	case cryptoType::Hash: {
		if (!strcmp(argv[3], "sha256"))
			config.cryptoGraphicMode = cryptoGraphic::SHA256;
		else if (!strcmp(argv[3], "zuc"))
			config.cryptoGraphicMode = cryptoGraphic::ZUC;
	}break;
	case cryptoType::Encrypt:
	case cryptoType::Decrypt: {
		//ѡ���㷨
		if (!strcmp(argv[3], "aes"))
			config.cryptoGraphicMode = cryptoGraphic::AES;
		else if (!strcmp(argv[3], "ecc"))
			config.cryptoGraphicMode = cryptoGraphic::ECC;
		else if (!strcmp(argv[3], "ecdsa"))
			config.cryptoGraphicMode = cryptoGraphic::ECDSA;
		else if (!strcmp(argv[3], "ElGamal"))
			config.cryptoGraphicMode = cryptoGraphic::ElGamal;
		else if (!strcmp(argv[3], "rc4"))
			config.cryptoGraphicMode = cryptoGraphic::RC4;
		else if (!strcmp(argv[3], "sm3"))
			config.cryptoGraphicMode = cryptoGraphic::SM3;
		else if (!strcmp(argv[3], "sm4"))
			config.cryptoGraphicMode = cryptoGraphic::SM4;
		else if (!strcmp(argv[3], "zuc"))
			config.cryptoGraphicMode = cryptoGraphic::ZUC;
	}break;
	}
	CRYPTO__ c;

}

CRYPTO__::CRYPTO__()
{
	SHA256 sha256;
	int ret;
	char szFullPath[_MAX_PATH], szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFileName[_MAX_FNAME], szExt[_MAX_EXT];
	std::string fullPath, dirPath, msg;
	switch (config.contentsTypeMode)
	{
	case contentsType::File: {
		cout << "������Ҫ�����ļ���λ��" << endl;
		cin >> szFullPath;
		ret = _splitpath_s(szFullPath, szDrive, szDrive ? _MAX_DRIVE : 0, szDir, szDir ? _MAX_DIR : 0, szFileName, szFileName ? _MAX_FNAME : 0, szExt, szExt ? _MAX_EXT : 0);
		dirPath += szDrive ? szDrive : "";
		dirPath += szDir ? szDir : "";
		fullPath = dirPath;
		switch (config.cryptoTypeMode)
		{
			fullPath += szFileName ? szFileName : "";
		case cryptoType::Encrypt: {
			fullPath += "cipher.txt";
		}break;
		case cryptoType::Decrypt: {
			fullPath += "invcipher";
			fullPath += szExt;
		}break;
		}

	}break;
	case contentsType::Message: {
		switch (config.cryptoTypeMode)
		{
		case cryptoType::Encrypt:
			cout << "������Ҫ������Ϣ:" << endl;
			cin >> msg;
			break;
		case cryptoType::Decrypt:
			cout << "������Ҫ������Ϣ:" << endl;
			cin >> msg;
			break;
		}
	}break;
	}
	
	switch (config.cryptoGraphicMode)
	{
	case cryptoGraphic::AES: {
		string keyStr;
		cout << "��������Կ��";
		cin >> keyStr;
		FileProc fp(szFullPath, fullPath);
		switch (config.cryptoTypeMode)
		{
		case cryptoType::Encrypt:
			encrypt(fp, keyStr, 16); break;
		case cryptoType::Decrypt:
			decrypt(fp, keyStr, 16); break;
		}
	}break;
	case cryptoGraphic::ECC: {
		ECC e;

		cout << "\n          ������ʵ����Բ���ߵļ��ܽ���" << endl;
		cout << "\n------------------------------------------------------------------------\n" << endl;
		switch (config.cryptoTypeMode)
		{
		case cryptoType::Encrypt:
			time_t t;
			srand((unsigned)time(&t));
			e.BuildParameters();
			e.PrintParameters();
			//������Կ����Կ�ļ������ļ���
			e.Ecc_saveKey(dirPath);

			printf("\n------------------------------------------------------------------------\n");
			e.Ecc_encipher(szFullPath, fullPath);//����
			break;
		case cryptoType::Decrypt:
			printf("\n------------------------------------------------------------------------\n");
			e.Ecc_loadKey(dirPath);
			e.Ecc_decipher(szFullPath, fullPath);//����

			break;
		}

	}break;
	case cryptoGraphic::ECDSA: {
		ECDSA ecdsa;
		ecdsa.printECDSA(sha256.ShaFile(szFullPath));
	}break;
	case cryptoGraphic::ElGamal: {
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

		std::unique_ptr<char[]> tempY(new char[800]());
		std::unique_ptr<char[]> tempR(new char[800]());
		std::unique_ptr<char[]> tempSHA(new char[800]());
		std::unique_ptr<char[]> tempA1(new char[800]());
		std::unique_ptr<char[]> tempT(new char[800]());

		try
		{
			if ((err = mp_init(&p)) != MP_OKAY) {
				throw("Error initializing the p. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&a)) != MP_OKAY) {
				throw("Error initializing the a. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&p_1)) != MP_OKAY) {
				throw("Error initializing the p_1. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&x)) != MP_OKAY) {
				throw("Error initializing the x. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&y)) != MP_OKAY) {
				throw("Error initializing the y. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&a)) != MP_OKAY) {
				throw("Error initializing the a. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&k)) != MP_OKAY) {
				throw("Error initializing the k. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&r)) != MP_OKAY) {
				throw("Error initializing the r. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&s)) != MP_OKAY) {
				throw("Error initializing the s. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&sha)) != MP_OKAY) {
				throw("Error initializing the sha. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&a1)) != MP_OKAY) {
				throw("Error initializing the a1. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&b1)) != MP_OKAY) {
				throw("Error initializing the b1. %s",
					mp_error_to_string(err));

			}
			if ((err = mp_init(&temp3)) != MP_OKAY) {
				throw("Error initializing the temp3. %s",
					mp_error_to_string(err));

			}

		}
		catch (const char* init_err)
		{
			cout << init_err << endl;
		}


		cout << "�������������λ����";
		cin >> lon;
		sts.GetPrime(&p, &a, lon);
		mp_sub_d(&p, 1, &p_1);
		do { mp_rand(&x, lon); } while (mp_cmp(&x, &p_1) != -1 && mp_cmp_d(&x, 1) != 1);//1<=x<p-1
		mp_exptmod(&a, &x, &p, &y);
		printf("y��:\n");
		mp_to_radix(&y, tempY.get(), SIZE_MAX, &written, 10);
		printf("%s\n", tempY.get());
		do {
			mp_rand(&k, lon / 26);
			mp_gcd(&k, &p_1, &r);//����
		} while (mp_cmp(&k, &p_1) != -1 || mp_cmp_d(&k, 1) != 1 || mp_cmp_d(&r, 1) != 0);//1<=k<p-1��k��p-1����
		mp_exptmod(&a, &k, &p, &r);
		printf("a**k mod p��:\n");
		mp_to_radix(&r, tempR.get(), SIZE_MAX, &written, 10);
		printf("%s\n", tempR.get());
		string str = sha256.ShaFile(szFullPath);
		mp_read_radix(&sha, str.c_str(), 10);
		printf("SHA��:\n");
		mp_to_radix(&sha, tempSHA.get(), SIZE_MAX, &written, 0x10);
		printf("%s\n", tempSHA.get());
		mp_mul(&x, &r, &s);
		mp_sub(&sha, &s, &s);
		ex_Eulid(&k, &p_1, &a1, &b1, &temp3);
		while (mp_cmp_d(&a1, 0) != 1)
			mp_add(&a1, &p_1, &a1);
		printf("k**-1��:\n");
		mp_to_radix(&a1, tempA1.get(), SIZE_MAX, &written, 10);
		printf("%s\n", tempA1.get());
		mp_mulmod(&k, &a1, &p_1, &temp3);
		printf("k*k**-1 mod p��:\n");
		mp_to_radix(&a1, tempT.get(), SIZE_MAX, &written, 10);
		printf("%s\n", tempT.get());
	}break;
	case cryptoGraphic::SHA256: {
		mp_int s;
		size_t written;
		mp_init(&s);
		string a = sha256.ShaFile(szFullPath);
		mp_read_radix(&s, a.c_str(), 10);
		char tempSHA[800] = { 0 };
		printf("SHA��:\n");
		mp_to_radix(&s, tempSHA, SIZE_MAX, &written, 0x10);
		printf("%s\n", tempSHA);

	}break;
	case cryptoGraphic::RC4: {


	}break;
		//case 7: {
		//	DES d;
		//	string keyStr;
		//	ifstream in;
		//	ofstream out;
		//	bitset<64> data;
		//	uint8_t plain[8];
		//	switch (enDoIndex)
		//	{
		//	case 1:
		//		//keyStr = GetRandList(8);
		//		keyStr = "12345678";
		//		cout << "��ԿΪ��" << keyStr << "(��ע�⸴�Ʊ���)" << endl;
		//		in.open(szFullPath, ios::binary);
		//		out.open(fullPath, ios::binary | ios::ate);
		//		if (in.is_open() && out.is_open()) {
		//			string buf((std::istreambuf_iterator<char>(in)),
		//				std::istreambuf_iterator<char>());
		//			buf=d.Encrypt(buf, keyStr);
		//			out.write(buf.c_str(),buf.length());
		//			in.close();
		//			out.close();
		//		}	
		//		break;
		//	case 2:
		//		/*cout << "��������Կ��";
		//		cin >> keyStr;*/
		//		keyStr = "12345678";
		//		if (keyStr.size() == 8) {
		//			in.open(szFullPath, ios::binary);
		//			out.open(fullPath, ios::binary | ios::ate);
		//			if (in.is_open()&&out.is_open()) {
		//				string buf((std::istreambuf_iterator<char>(in)),
		//					std::istreambuf_iterator<char>());
		//				buf = d.Decrypt(buf, keyStr);
		//				out.write(buf.c_str(), buf.length());
		//				in.close();
		//				out.close();
		//			}
		//			cout << "press any key to shutdown" << endl;
		//			std::cin.get();
		//		}
		//		else {
		//			cout << "��Կ��������";
		//			cout << "press any key to shutdown" << endl;
		//			std::cin.get();
		//		}
		//		break;
		//	}
		//}break;
	case cryptoGraphic::SM3: {
		SM3 sm3;
		FileProc fp(szFullPath, fullPath);
		uint8_t hash[32];
		sm3.SM3_HASH256(fp, hash);
		for (auto i = 0; i < sizeof(hash); i++) {
			printf("%x", hash[i]);
		}

	}break;
	case cryptoGraphic::SM4: {
		SM4 sm4;	
		string key;
		uint8_t* keyStr, plain[16] = { 0 }, cipher[16] = { 0, };
		cout << "��������Կ��";
		cin >> key;
		if (key.size() == 16) {
			keyStr = (uint8_t*)key.c_str();
			FileProc fp(szFullPath, fullPath);
			switch (config.cryptoTypeMode)
			{
			case cryptoType::Encrypt:
				while (fp.read((char*)(plain), sizeof(plain))) {
					sm4.SM4_Encrypt(keyStr, plain, cipher);
					fp.write((char*)cipher, sizeof(cipher));
				}break;
			case cryptoType::Decrypt:
				// ���� cipher.txt����д��ͼƬ flower1.jpg
				while (fp.read((char*)cipher, sizeof(cipher))) {
					sm4.SM4_Decrypt(keyStr,cipher, plain);
					fp.write((char*)plain, sizeof(plain));
				}break;
			}
		}
	}break;
	case cryptoGraphic::ZUC: {
		ZUC zuc;
		string key;
		uint8_t* keyStr, plain[16] = { 0 }, cipher[16] = { 0, };
		cout << "��������Կ��";
		cin >> key;
		if (key.size() == 16) {
			keyStr = (uint8_t*)key.c_str();
			FileProc fp(szFullPath, fullPath);
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::mt19937 g1(seed);
			uint32_t u32Random = g1();
			switch (config.cryptoTypeMode)
			{
			case cryptoType::Encrypt:
				while (fp.read((char*)(plain), sizeof(plain))) {
					//zuc.ZUC_Confidentiality(keyStr, u32Random, );
					fp.write((char*)cipher, sizeof(cipher));
				}break;
			case cryptoType::Decrypt:
				// ���� cipher.txt����д��ͼƬ flower1.jpg
				while (fp.read((char*)cipher, sizeof(cipher))) {
					//sm4.SM4_Decrypt(keyStr, cipher, plain);
					fp.write((char*)plain, sizeof(plain));
				}break;
			}
		}
	}break;
	default:
		break;
	}
}
