#ifndef _AES_H_
#define _AES_H_

#include <bitset> 
using std::bitset;
#include"tommath.h"
namespace crypto__ {
	typedef bitset<8> byte;
	typedef bitset<32> word;

	const unsigned char Nr = 10; // AES-128��Ҫ 10 �ּ��� 
	const unsigned char Nk = 4; // Nk ��ʾ������Կ�� word ���� 


	class AES
	{
	public:
		//��Կ��չ�㷨
		void KeyExpansion(byte key[4 * Nk], word w[4 * (Nr + 1)]);

		//����
		void encrypt(byte in[4 * 4], word w[4 * (Nr + 1)]);

		//����
		void decrypt(byte in[4 * 4], word w[4 * (Nr + 1)]);
		AES();
		~AES();
	private:

		byte GFMul(byte a, byte b);
		void AddRoundKey(byte mtx[4 * 4], word k[4]);
		//----------------------------------ת������-----------------------------------------------

		//�ֽ��滻�任
		void SubBytes(byte mtx[4 * 4]);
		void InvSubBytes(byte mtx[4 * 4]);

		//����λ�任
		void ShiftRows(byte mtx[4 * 4]);
		void InvShiftRows(byte mtx[4 * 4]);

		//�л�ϱ任
		void MixColumns(byte mtx[4 * 4]);
		void InvMixColumns(byte mtx[4 * 4]);

	private:
		//----------------------------------��������------------------------------------------------

		//��4�� byte ת��Ϊһ�� word
		word Word(byte& k1, byte& k2, byte& k3, byte& k4);

		//���ֽ� ѭ������һλ��
		//����[a0, a1, a2, a3]���[a1, a2, a3, a0]
		word RotWord(word& rw);

		//������word�е�ÿһ���ֽڽ���S-�б任	 
		word SubWord(word& sw);
	};
}
#endif