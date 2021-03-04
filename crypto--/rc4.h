#ifndef _RC4_H_
#define _RC4_H_
using std::string;
class RC4
{
public:
	RC4();
	~RC4();
public:
	char T[256];    //����T
	int Key[256];   //������ɵ���Կ
	int S[256];
public:
	int* init_Key();
	void permute_S();
	string create_key_stream(string text, string KeyStream);
	void Rc4EncryptText(string text, string KeyStream, string PlainText, string CryptoText, string strPath);
};
#endif // !_RC4_H_

