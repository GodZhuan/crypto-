#ifndef _DES_H_
#define _DES_H_

#include <string>

const unsigned BITS_PER_CHAR = 8;
const unsigned KEY_SZIE = 8;
const unsigned SUBKEY_NUM = 16;
const unsigned SUBKEY_LENGHT = 48;
const unsigned EXPAND_SIZE = 48;
const unsigned PC_2_SIZE = 48;
const unsigned PC_1_SIZE = 56;
const unsigned BIT_STR_SIZE = 64;
const unsigned HALF_BIT_STR_SIZE=BIT_STR_SIZE / 2;

class DES
{
private:
	//------------------------------------������Կ��Ҫ�ı�----------------------------------------
	//�û�ѡ���1
	//ѹ����λȥ��ÿ���ֽڵĵ�8λ��������żУ�죬�����ϵ�8λ�ɺ���
	static const unsigned int PC1_Table[PC_1_SIZE];
	//��ѭ�������
	static const unsigned int Move_Table[SUBKEY_NUM];
	//�û�ѡ���2
	//ѡ�����е�ĳЩλ������ٵ�48λ
	static const unsigned int PC2_Table[PC_2_SIZE];
	//����Կ
	volatile bool subKeys[SUBKEY_NUM][SUBKEY_LENGHT];
	//----------------------------------���ܻ������Ҫ�ı�--------------------------------------------
	//��ʼ�û���
	//���е���ֵ��ʾ����Ϊ���û������λ��
	static const unsigned int IP_Table[BIT_STR_SIZE];
	//��չ��
	//ͨ���ظ�ĳЩλ��32λ���Ұ벿�ְ��ձ���չ��48λ
	static const unsigned int Expand_Table[EXPAND_SIZE];
	//�����û���
	static const unsigned int Permute_Table[BIT_STR_SIZE / 2];
	//���û���
	static const unsigned int IP_1_Table[BIT_STR_SIZE];
	//�û���
	static const unsigned int SBox_Table[KEY_SZIE][4][16];
private:
	//����16������Կ
	bool CreateSubKey(const std::string& key);

	//����8�ֽ����ݿ�
	bool EncryptBlock(std::string& block);
	//����8�ֽ����ݿ�
	bool DecryptBlock(std::string& block);

	//----------------------------------ת������-----------------------------------------------
	bool PC1_Transform(const std::string& bitStr, std::string& PC1BitStr);
	void PC1_Transform(const bool bitStr[BIT_STR_SIZE], bool PC1bitStr[PC_1_SIZE]);

	void PC2_Transform(const bool PC1bitStr[PC_1_SIZE], volatile bool subKey[SUBKEY_LENGHT]);

	bool IP_Transform(bool bitStr[BIT_STR_SIZE]);

	void Expand_Transform(bool eBitStr[EXPAND_SIZE]);

	void SBox_Transform(bool eBitStr[EXPAND_SIZE]);

	void Permute_Transform(bool halfBitStr[HALF_BIT_STR_SIZE]);

	void IP_1_Transform(bool bitStr[BIT_STR_SIZE]);

	//------------------------------------��������------------------------------------------------
	void CharToBit(const std::string& str, bool* bitStr, int bits);
	void BitToChar(const bool* bitStr, std::string& str);

	void XOR(bool strFirst[EXPAND_SIZE], bool strSecond[EXPAND_SIZE], size_t num);

	bool LeftCycle(bool str[PC_1_SIZE], size_t step);
public:
	DES();
	~DES();

	std::string Encrypt(const std::string& plain, const std::string& key);
	std::string Decrypt(const std::string& cipher, const std::string& key);
};

#endif
