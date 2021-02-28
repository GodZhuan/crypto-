#include "des.h"
char DES::PC1_Table[PC_1_SIZE] = {                               //��Կ��һ���û�����
57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};
char DES::Move_Table[SUBKEY_NUM] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

char DES::PC2_Table[PC_2_SIZE] = {
14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

char DES::IP_Table[BIT_STR_SIZE] = {                                     //IP�û�����
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

char DES::Expand_Table[EXPAND_SIZE] = {                                  //��չ����
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1 };

char DES::Permute_Table[BIT_STR_SIZE / 2] = {                            //  P ��
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25 };

char DES::IP_1_Table[BIT_STR_SIZE] = {                                    //��IP�û�����
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25 };

char DES::SBox_Table[KEY_SZIE][4][16] = {                     //8��S��   ��ά����
// S1
{
14, 4,  13,     1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},
// S2
{
15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},
// S3
{
10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},
// S4
{
7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},
// S5
{
2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},
// S6
{
12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},
// S7
{
4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},
// S8
{
13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}
};

char subKey[SUBKEY_NUM][SUBKEY_LENGHT];

DES::DES(){}

DES::~DES(){}

std::string DES::Encrypt(const std::string& plain, const std::string& key)
{
	std::string result;
	if (plain.empty() || key.empty())return result;
	if (!CreateSubKey(key,subKey))return result;

	for (size_t i = 0; i < plain.size() / 8; ++i)
	{
		std::string block = plain.substr(i * 8, 8);
		EncryptBlock(block, subKey);
		result.append(block);
	}
	int remainder = plain.size() % 8;
	if (remainder)
	{
		std::string block = plain.substr(plain.size() - remainder, remainder);
		block.append(8 - remainder, '\0');
		EncryptBlock(block, subKey);
		result.append(block);
	}

	return result;
}

std::string DES::Decrypt(const std::string& cipher, const std::string& key)
{
	std::string result;
	if (cipher.empty() || key.empty())return result;
	else if (cipher.size()%8)return result;
	else if (!CreateSubKey(key, subKey))return result;

	for (size_t i = 0; i < cipher.size() / 8; ++i)
	{
		std::string block = cipher.substr(i * 8, 8);
		DecryptBlock(block, subKey);
		result.append(block);
	}

	return result;
}

bool DES::CreateSubKey(const std::string& key, char subKey[SUBKEY_NUM][SUBKEY_LENGHT])
{	
	std::string bitStr;
	bitStr.resize(BIT_STR_SIZE);// 64
	Char8ToBit64(key, bitStr);

	std::string PC1BitStr;
	PC1BitStr.resize(PC_1_SIZE);//56

	if (!PC1_Transform(bitStr, PC1BitStr))return false;

	for (int i = 0; i < SUBKEY_NUM; ++i)
	{
		LeftCycle(PC1BitStr, 0, PC_1_SIZE / 2, Move_Table[i]);
		LeftCycle(PC1BitStr, PC_1_SIZE / 2, PC_1_SIZE, Move_Table[i]);

		PC2_Transform(PC1BitStr, subKey[i]); // 48
	}
	return true;
}

bool DES::EncryptBlock(std::string& block, char subKey[SUBKEY_NUM][SUBKEY_LENGHT])
{
	if (block.size() != KEY_SZIE)return false;
	std::string bitStr;
	bitStr.resize(BIT_STR_SIZE);
	Char8ToBit64(block, bitStr);

	IP_Transform(bitStr);

	std::string halfBitStr;
	halfBitStr.resize(bitStr.size() / 2);
	std::string eBitStr;
	eBitStr.resize(EXPAND_SIZE);

	for (size_t i = 0; i < SUBKEY_NUM; ++i)
	{
		Expand_Transform(bitStr.substr(bitStr.size() / 2), eBitStr);

		std::string subKey = std::string(subKey[i], SUBKEY_LENGHT);

		XOR(eBitStr, subKey, SUBKEY_LENGHT);

		SBox_Transform(eBitStr, halfBitStr);

		Permute_Transform(halfBitStr);

		XOR(bitStr, halfBitStr, halfBitStr.size());

		if (i != SUBKEY_NUM - 1)
			LeftCycle(bitStr, 0, bitStr.size(), bitStr.size() / 2);
	}
	IP_1_Transform(bitStr);

	Bit64ToChar8(bitStr, block);

	return false;
}

bool DES::DecryptBlock(std::string& block, char subKey[SUBKEY_NUM][SUBKEY_LENGHT])
{
	if (block.size() != KEY_SZIE)
		return false;

	std::string bitStr;
	bitStr.resize(BIT_STR_SIZE);
	Char8ToBit64(block, bitStr);

	IP_Transform(bitStr);

	std::string halfBitStr;
	halfBitStr.resize(bitStr.size() / 2);
	std::string eBitStr;
	eBitStr.resize(EXPAND_SIZE);
	for (int i = SUBKEY_NUM - 1; i >= 0; --i)
	{
		Expand_Transform(bitStr.substr(bitStr.size() / 2), eBitStr);

		std::string SubKey = std::string(subKey[i], SUBKEY_LENGHT);

		XOR(eBitStr, SubKey, SUBKEY_LENGHT);

		SBox_Transform(eBitStr, halfBitStr);
		Permute_Transform(halfBitStr);

		XOR(bitStr, halfBitStr, halfBitStr.size());

		if (i != 0)
			LeftCycle(bitStr, 0, bitStr.size(), bitStr.size() / 2);
	}
	IP_1_Transform(bitStr);
	Bit64ToChar8(bitStr, block);

	return true;
}

bool DES::PC1_Transform(const std::string& bitStr, std::string& PC1BitStr)
{
	if (bitStr.size() != BIT_STR_SIZE)return false;

	std::string tmpStr;
	tmpStr.resize(PC_1_SIZE);
	for (size_t i = 0; i < PC_1_SIZE; ++i)
		tmpStr[i] = bitStr[PC1_Table[i]];

	PC1BitStr.swap(tmpStr);

	return true;
}

bool DES::PC2_Transform(const std::string& PC1BitStr, char subKey[SUBKEY_LENGHT])
{
	if (PC1BitStr.size() != PC_1_SIZE)return false;

	std::string tmpStr;
	tmpStr.resize(PC_2_SIZE);
	for (size_t i = 0; i < PC_2_SIZE; ++i)
		subKey[SUBKEY_LENGHT] = PC1BitStr[PC2_Table[i]];

	return true;
}

bool DES::IP_Transform(std::string& bitStr)
{
	if (bitStr.size() != BIT_STR_SIZE)return false;

	std::string tmpBitStr;
	tmpBitStr.resize(bitStr.size());
	for (size_t i = 0; i < bitStr.size(); ++i)
		tmpBitStr[i] = bitStr[IP_Table[i]];

	bitStr.swap(tmpBitStr);

	return true;
}

bool DES::Expand_Transform(const std::string& halfBitStr, std::string& eBitStr)
{
	if (halfBitStr.size() != BIT_STR_SIZE / 2 || eBitStr.size() != EXPAND_SIZE)return false;

	for (size_t i = 0; i < eBitStr.size(); ++i)
		eBitStr[i] = halfBitStr[Expand_Table[i]];

	return true;
}

bool DES::SBox_Transform(const std::string& eBitStr, std::string& halfBitStr)
{
	if (eBitStr.size() != EXPAND_SIZE || halfBitStr.size() != BIT_STR_SIZE / 2)return false;

	for (size_t i = 0; i < KEY_SZIE; ++i)
	{
		size_t j = i * 6;
		size_t row = (eBitStr[j] << 1) + eBitStr[j + eBitStr.size() / KEY_SZIE - 1];
		size_t column = (eBitStr[j + 1] << 3) + (eBitStr[j + 2] << 2) + (eBitStr[j + 3] << 1) + eBitStr[j + 4];

		int x = SBox_Table[i][row][column];


		halfBitStr[i * 4] = x >> 3;
		halfBitStr[i * 4 + 1] = (x >> 2) & 0x1;
		halfBitStr[i * 4 + 2] = (x >> 1) & 0x1;
		halfBitStr[i * 4 + 3] = x & 0x1;

	}
	return true;
}

bool DES::Permute_Transform(std::string& halfBitStr)
{
	if (halfBitStr.size() != BIT_STR_SIZE / 2)
		return false;

	std::string tmpStr;
	tmpStr.resize(halfBitStr.size());

	for (size_t i = 0; i < halfBitStr.size(); ++i)
		tmpStr[i] = halfBitStr[Permute_Table[i]];

	halfBitStr.swap(tmpStr);

	return true;
}

bool DES::IP_1_Transform(std::string& bitStr)
{
	if (bitStr.size() != BIT_STR_SIZE)
		return false;

	std::string tmpStr;
	tmpStr.resize(BIT_STR_SIZE);
	for (size_t i = 0; i < bitStr.size(); ++i)
		tmpStr[i] = bitStr[IP_1_Table[i]];

	bitStr.swap(tmpStr);

	return true;
}

bool DES::Char8ToBit64(const std::string& str, std::string& bitStr)
{
	if(str.size() != KEY_SZIE || bitStr.size() != BIT_STR_SIZE)
		return false;

	for (size_t i = 0; i < str.size(); ++i)
	{
		for (size_t j = 0; j < BITS_PER_CHAR; ++j)
			bitStr[i * BITS_PER_CHAR + j] = ((str[i] >> j) & 0x1);
	}
	return true;
}

bool DES::Bit64ToChar8(const std::string& bitStr, std::string& str)
{
	if (str.size() != KEY_SZIE || bitStr.size() != BIT_STR_SIZE)
		return false;

	for (size_t i = 0; i < KEY_SZIE; ++i)
	{
		for (size_t j = 0; j < BITS_PER_CHAR; ++j)
			str[i] |= bitStr[i * KEY_SZIE + j] << j;
	}
	return true;
}

bool DES::XOR(std::string& strFirst, std::string& strSecond, size_t num)
{
	if (strFirst.size()<num || strSecond.size()<num)
		return false;

	for (size_t i = 0; i < num; ++i)
		strFirst[i] ^= strSecond[i];
	
	return true;
}

bool DES::LeftCycle(std::string& str, size_t beginSection, size_t endSection, size_t step)
{
	if (endSection > str.size())
		return false;

	size_t tmpStep = step % (endSection - beginSection);
	std::string tmpStr = str.substr(beginSection + tmpStep, endSection - beginSection - tmpStep);

	tmpStr.append(str.substr(beginSection, tmpStep));

	//for (auto __begin = std::begin(Move_Table), __end = std::end(Move_Table); __begin != __end; ++__begin) {}
	for (size_t i = beginSection; i < endSection; ++i)
		str[i] = tmpStr[i - beginSection];

	return true;
}
