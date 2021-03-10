#ifndef _ECC_H_
#define _ECC_H_
/*��1���û�Aѡ��һ���ʺϼ��ܵ���Բ����Ep(a,b)(��:y2=x3+ax+b)����ȡ��Բ������һ�㣬��Ϊ����G��
����2���û�Aѡ��һ��˽����Կk�������ɹ�����ԿK=kG��
����3���û�A��Ep(a,b)�͵�K��G�����û�B��
����4���û�B�ӵ���Ϣ�� ��������������ı��뵽Ep(a,b)��һ��M��������һ���������r��r<n����
����5���û�B�����C1=M+rK��C2=rG��
����6���û�B��C1��C2�����û�A��
����7���û�A�ӵ���Ϣ�󣬼���C1-kC2��������ǵ�M����Ϊ
		  C1-kC2=M+rK-k(rG)=M+rK-r(kG)=M
�������ٶԵ�M���н���Ϳ��Եõ����ġ�

  ��������ѧ�У�����һ��Fp�ϵ���Բ���ߣ����õ�����������
	   T=(p,a,b,G,n,h)��
������p ��a ��b ����ȷ��һ����Բ���ߣ�GΪ���㣬nΪ��G�Ľף�h ����Բ���������е�ĸ���m��n������������֣�

�����⼸������ȡֵ��ѡ��ֱ��Ӱ���˼��ܵİ�ȫ�ԡ�����ֵһ��Ҫ���������¼���������

����1��p ��ȻԽ��Խ��ȫ����Խ�󣬼����ٶȻ������200λ���ҿ�������һ�㰲ȫҪ��
����2��p��n��h��
����3��pt��1 (mod n)��1��t<20��
����4��4a3+27b2��0 (mod p)��
����5��n Ϊ������
����6��h��4��
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "tommath.h"
#include <time.h>
using namespace std;


constexpr auto BIT_LEN = 800 ;
constexpr auto KEY_LONG = 256;  //˽Կ���س�;
constexpr auto P_LONG = 200;    //������P���س�;
constexpr auto EN_LONG = 40;    //һ��ȡ�����ֽ���(x,20)(y,20);

class ECC
{
private:
	mp_int GX;//����G��x����
	mp_int GY;//����G��y����
	mp_int K;//˽����Կ
	mp_int A;//���߲���A
	mp_int B;//���߲���B
	mp_int QX;//��ԿQ��x����
	mp_int QY;//��ԿQ��y����
	mp_int P;//Fp�е�p(������P)
	mp_err err;

	string temp;
	string tempA;
	string tempB;
	string tempGX;
	string tempGY;
	string tempK;
	string tempQX;
	string tempQY;
public:
	ECC();
	~ECC();
	//������Բ���ɲ���
	void BuildParameters(void);
	//��ӡ��Բ���߲��������浽�ļ���
	void PrintParameters(void);
	std::string Encrypt(const std::string& plain, const std::string& key);
	std::string Decrypt(const std::string& cipher, const std::string& key);

	//�õ�lon���س�����
	int GetPrime(mp_int* m, int lon);
	//�õ�B��G��X����G��Y����
	void Get_B_X_Y(mp_int* x1, mp_int* y1, mp_int* b, mp_int* a, mp_int* p);
	//���
	bool Ecc_points_mul(mp_int* qx, mp_int* qy, mp_int* px, mp_int* py, mp_int* d, mp_int* a, mp_int* p);
	//���
	int Two_points_add(mp_int* x1, mp_int* y1, mp_int* x2, mp_int* y2, mp_int* x3, mp_int* y3, mp_int* a, bool zero, mp_int* p);
	//�����ƴ洢����
	int chmistore(mp_int* a, FILE* fp);
	//�Ѷ�ȡ���ַ�����mp_int����
	int putin(mp_int* a, char* ch, int chlong);
	//ECC����
	void Ecc_encipher(char* inPath,string outPath);
	//ECC��Կ����
	void Ecc_saveKey(string outPath);
	//ECC��Կ��ȡ
	void Ecc_loadKey(string inPath);
	//ECC����
	void Ecc_decipher(char* inPath, string outPath);
	//ʵ�ֽ�mp_int��a�еı��ش���ԭΪ�ַ����������ַ���ch��
	int chdraw(mp_int* a, char* ch);
	//ȡ����
	int miwendraw(mp_int* a, char* ch, int chlong);

};
#endif // !_ECC_H_

