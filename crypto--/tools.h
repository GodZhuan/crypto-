#ifndef _TOOLS_H_
#define _TOOLS_H_ 
#include "aes.h"
#include<time.h>
#include <random>
namespace crypto__ {
	//ѭ������nλ
	inline unsigned int rotl32(unsigned int buf, int n) {
		return (buf << n) | (buf >> (32 - n));
	}
	/**
	 *  ��һ��char�ַ�����ת��Ϊ������
	 *  �浽һ�� byte ������
	 */
	bool charToByte(byte out[16], std::string& s);

	/**
	 *  ��������128λ�ֳ�16�飬�浽һ�� byte ������
	 */
	void divideToByte(byte out[16], bitset<128>& data);


	/**
	 *  ��16�� byte �ϲ���������128λ
	 */
	bitset<128> mergeByte(byte in[16]);
	int myrng(unsigned char* dst, int len, void* dat);

	//����ָ���ֳ�������ַ���
	std::string GetRandList(int len);

	void ex_Eulid(mp_int* a, mp_int* b, mp_int* a1, mp_int* b1, mp_int* temp3);
	mp_err constmp_to_radix(const mp_int* a, const char* str, size_t maxlen, size_t* written, int radix);
}
#endif
