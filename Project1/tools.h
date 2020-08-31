#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "aes.h"
/**
 *  ��һ��char�ַ�����ת��Ϊ������
 *  �浽һ�� byte ������
 */
bool charToByte(byte out[16], std::string& s)
{
	if (s[15] == NULL)return false;
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 8; ++j)
			out[i][j] = ((s[i] >> j) & 1);
	return true;
}

/**
 *  ��������128λ�ֳ�16�飬�浽һ�� byte ������
 */
void divideToByte(byte out[16], bitset<128>& data)
{
	bitset<128> temp;
	for (int i = 0; i < 16; ++i)
	{
		temp = (data << 8 * i) >> 120;
		out[i] = temp.to_ulong();
	}
}


/**
 *  ��16�� byte �ϲ���������128λ
 */
bitset<128> mergeByte(byte in[16])
{
	bitset<128> res;
	res.reset();  // ��0
	bitset<128> temp;
	for (int i = 0; i < 16; ++i)
	{
		temp = in[i].to_ulong();
		temp <<= 8 * (15 - i);
		res |= temp;
	}
	return res;
}
int myrng(unsigned char* dst, int len, void* dat)
{
	int x;
	for (x = 0; x < len; x++) dst[x] = rand() & 0xFF;
	return len;
}
#endif
