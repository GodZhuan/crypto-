#include"tools.h"
namespace crypto__ {
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

	//����ָ���ֳ�������ַ���
	std::string GetRandList(int len)
	{
		char strRandomList[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '!', '@', '#', '$', '%', '&', '*', '_' };
		std::string pwd = "";
		std::mt19937 e(static_cast<unsigned int>(time(NULL)));
		std::uniform_int_distribution<unsigned> u(0, 69);
		for (int i = 0; i < len; i++)
		{
			pwd += strRandomList[u(e)];//���ȡstrRandomList ����ֵ
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
	mp_err constmp_to_radix(const mp_int* a, const char* str, size_t maxlen, size_t* written, int radix) {
		return mp_to_radix(a, const_cast<char*>(str), maxlen, written, radix);
	}
}