#ifndef _ECDSA_H_
#define _ECDSA_H_
#include<string>
#include "tommath.h"
#include "ecc.h"
#include "sts.h"
namespace crypto__ {
	static const char* rP = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
	static const char* rGX = "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
	static const char* rGY = "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
	static const char* rn = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
	class ECDSA
	{
	private:
		mp_err err;
		ECC ecc;
		STS sts;
		int lon;
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
	public:
		ECDSA();
		~ECDSA();
		void printECDSA(std::string& sh);
	};
}
#endif // !_ECDSA_H_

