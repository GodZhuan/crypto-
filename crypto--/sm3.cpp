#include "sm3.h"
namespace crypto__ {
	void SM3::BiToW(uint32_t Bi[], uint32_t W[])
	{
		int i;
		uint32_t tmp;
		for (i = 0; i <= 15; i++)
		{
			W[i] = Bi[i];
		}
		for (i = 16; i <= 67; i++)
		{
			tmp = W[i - 16]
				^ W[i - 9]
				^ rotl32(W[i - 3], 15);
			W[i] = SM3_p1(tmp)
				^ (rotl32(W[i - 13], 7))
				^ W[i - 6];
		}
	}

	void SM3::WToW1(uint32_t W[], uint32_t W1[])
	{
		int i;
		for (i = 0; i <= 63; i++)
		{
			W1[i] = W[i] ^ W[i + 4];
		}
	}

	void SM3::CF(uint32_t W[], uint32_t W1[], uint32_t V[])
	{
		uint32_t SS1;
		uint32_t SS2;
		uint32_t TT1;
		uint32_t TT2;
		uint32_t A, B, C, D, E, F, G, H;
		uint32_t T = SM3_T1;
		uint32_t FF;
		uint32_t GG;
		int j;
		//reg init,set ABCDEFGH=V0
		A = V[0];
		B = V[1];
		C = V[2];
		D = V[3];
		E = V[4];
		F = V[5];
		G = V[6];
		H = V[7];
		for (j = 0; j <= 63; j++)
		{
			//SS1
			if (j == 0)
			{
				T = SM3_T1;
			}
			else if (j == 16)
			{
				T = rotl32(SM3_T2, 16);
			}
			else
			{
				T = rotl32(T, 1);
			}
			SS1 = rotl32((rotl32(A, 12) + E + T), 7);
			//SS2
			SS2 = SS1 ^ rotl32(A, 12);
			//TT1
			if (j <= 15)
			{
				FF = SM3_ff0(A, B, C);
			}
			else
			{
				FF = SM3_ff1(A, B, C);
			}
			TT1 = FF + D + SS2 + *W1;
			W1++;
			//TT2
			if (j <= 15)
			{
				GG = SM3_gg0(E, F, G);
			}
			else
			{
				GG = SM3_gg1(E, F, G);
			}
			TT2 = GG + H + SS1 + *W;
			W++;
			//D
			D = C;
			//C
			C = rotl32(B, 9);
			//B
			B = A;
			//A
			A = TT1;
			//H
			H = G;
			//G
			G = rotl32(F, 19);
			//F
			F = E;
			//E
			E = SM3_p0(TT2);
		}
		//update V
		V[0] = A ^ V[0];
		V[1] = B ^ V[1];
		V[2] = C ^ V[2];
		V[3] = D ^ V[3];
		V[4] = E ^ V[4];
		V[5] = F ^ V[5];
		V[6] = G ^ V[6];
		V[7] = H ^ V[7];
	}
	/******************************************************************************
	 Function: BigEndian
	 Description: U32 endian converse.GM/T 0004-2012 requires to use big-endian.
	 if CPU uses little-endian, BigEndian function is a necessary
	 call to change the little-endian format into big-endian format.
	 Calls:
	 Called By: SM3_compress, SM3_done
	 Input: src[uint8_tlen]
	 uint8_tlen
	 Output: des[uint8_tlen]
	 Return: null
	 Others: src and des could implies the same address
	*******************************************************************************/
	void SM3::BigEndian(uint8_t src[], uint32_t uint8_tlen, uint8_t des[])
	{
		uint8_t tmp = 0;
		uint32_t i = 0;
		for (i = 0; i < uint8_tlen / 4; i++)
		{
			tmp = des[4 * i];
			des[4 * i] = src[4 * i + 3];
			src[4 * i + 3] = tmp;
			tmp = des[4 * i + 1];
			des[4 * i + 1] = src[4 * i + 2];
			des[4 * i + 2] = tmp;
		}
	}
	/******************************************************************************
	 Function: SM3_init
	 Description: initiate SM3 state
	 Calls:
	 Called By: SM3_256
	 Input: SM3_STATE *md
	 Output: SM3_STATE *md
	 Return: null
	 Others:
	*******************************************************************************/
	void SM3::SM3_init(SM3_STATE* md)
	{
		md->curlen = md->length = 0;
		md->state[0] = SM3_IVA;
		md->state[1] = SM3_IVB;
		md->state[2] = SM3_IVC;
		md->state[3] = SM3_IVD;
		md->state[4] = SM3_IVE;
		md->state[5] = SM3_IVF;
		md->state[6] = SM3_IVG;
		md->state[7] = SM3_IVH;
	}
	/******************************************************************************
	 Function: SM3_compress
	 Description: compress a single block of message
	 Calls: BigEndian
	 BiToW
	 WToW1
	 CF
	 Called By: SM3_256
	 Input: SM3_STATE *md
	 Output: SM3_STATE *md
	 Return: null
	 Others:
	*******************************************************************************/
	void SM3::SM3_compress(SM3_STATE* md)
	{
		uint32_t W[68];
		uint32_t W1[64];
		//if CPU uses little-endian, BigEndian function is a necessary call
		BigEndian(md->buf, 64, md->buf);
		BiToW((uint32_t*)md->buf, W);
		WToW1(W, W1);
		CF(W, W1, md->state);
	}
	/******************************************************************************
	 Function: SM3_process
	 Description: compress the first (len/64) blocks of message
	 Calls: SM3_compress
	 Called By: SM3_256
	 Input: SM3_STATE *md
	 uint8_t buf[len] //the input message
	 int len //uint8_tlen of message
	 Output: SM3_STATE *md
	 Return: null
	 Others:
	*******************************************************************************/
	void SM3::SM3_process(SM3_STATE* md, uint8_t buf[], int len)
	{
		while (len--)
		{
			/* copy uint8_t */
			md->buf[md->curlen] = *buf++;
			md->curlen++;
			/* is 64 uint8_ts full? */
			if (md->curlen == 64)
			{
				SM3_compress(md);
				md->length += 512;
				md->curlen = 0;
			}
		}
	}
	/******************************************************************************
	 Function: SM3_done
	 Description: compress the rest message that the SM3_process has left behind
	 Calls: SM3_compress
	 Called By: SM3_256
	 Input: SM3_STATE *md
	 Output: uint8_t *hash
	 Return: null
	 Others:
	*******************************************************************************/
	void SM3::SM3_done(SM3_STATE* md, uint8_t hash[])
	{
		int i;
		uint8_t tmp = 0;
		/* increase the bit length of the message */
		md->length += md->curlen << 3;
		/* append the '1' bit */
		md->buf[md->curlen] = 0x80;
		md->curlen++;
		/* if the length is currently above 56 uint8_ts, appends zeros till
		it reaches 64 uint8_ts, compress the current block, creat a new
		block by appending zeros and length,and then compress it
		*/
		if (md->curlen > 56)
		{
			for (; md->curlen < 64;)
			{
				md->buf[md->curlen] = 0;
				md->curlen++;
			}
			SM3_compress(md);
			md->curlen = 0;
		}
		/* if the length is less than 56 uint8_ts, pad upto 56 uint8_ts of zeroes */
		for (; md->curlen < 56;)
		{
			md->buf[md->curlen] = 0;
			md->curlen++;
		}
		/* since all messages are under 2^32 bits we mark the top bits zero */
		for (i = 56; i < 60; i++)
		{
			md->buf[i] = 0;
		}
		/* append length */
		md->buf[63] = md->length & 0xff;
		md->buf[62] = (md->length >> 8) & 0xff;
		md->buf[61] = (md->length >> 16) & 0xff;
		md->buf[60] = (md->length >> 24) & 0xff;
		SM3_compress(md);
		/* copy output */
		memcpy(hash, md->state, SM3_len / 8);
		BigEndian(hash, SM3_len / 8, hash);//if CPU uses little-endian, BigEndian function is a necessary call
	}
	/******************************************************************************
	 Function: SM3_256
	 Description: calculate a hash value from a given message
	 Calls: SM3_init
	 SM3_process
	 SM3_done
	 Called By:
	 Input: uint8_t buf[len] //the input message
	 int len //uint8_tlen of the message
	 Output: uint8_t hash[32]
	 Return: null
	 Others:
	*******************************************************************************/
	void SM3::SM3_256(uint8_t buf[], int len, uint8_t hash[])
	{
		SM3_STATE md;
		SM3_init(&md);
		SM3_process(&md, buf, len);
		SM3_done(&md, hash);
	}
	/******************************************************************************
	 Function: SM3_SelfTest
	 Description: test whether the SM3 calculation is correct by comparing
	 the hash result with the standard result
	 Calls: SM3_256
	 Called By:
	 Input: null
	 Output: null
	 Return: 0 //the SM3 operation is correct
	 1 //the sm3 operation is wrong
	 Others:
	*******************************************************************************/
	int SM3::SM3_SelfTest()
	{
		uint32_t i = 0, a = 1, b = 1;
		uint8_t Msg1[3] = { 0x61,0x62,0x63 };
		int MsgLen1 = 3;
		uint8_t MsgHash1[32] = { 0 };
		uint8_t
			StdHash1[32] = { 0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,

			0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0 };
		uint8_t
			Msg2[64] = { 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

			0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

			0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

			0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64 };
		int MsgLen2 = 64;
		uint8_t MsgHash2[32] = { 0 };
		uint8_t
			StdHash2[32] = { 0xde,0xbe,0x9f,0xf9,0x22,0x75,0xb8,0xa1,0x38,0x60,0x48,0x89,0xc1,0x8e,0x5a,0x4d,

			0x6f,0xdb,0x70,0xe5,0x38,0x7e,0x57,0x65,0x29,0x3d,0xcb,0xa3,0x9c,0x0c,0x57,0x32 };
		SM3_256(Msg1, MsgLen1, MsgHash1);
		SM3_256(Msg2, MsgLen2, MsgHash2);
		a = memcmp(MsgHash1, StdHash1, SM3_len / 8);
		b = memcmp(MsgHash2, StdHash2, SM3_len / 8);
		if ((a == 0) && (b == 0))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	void SM3::SM3_HASH256(FileProc& fp, uint8_t hash[])
	{
		const size_t cnt = fp.inSize();
		uint8_t* buf = new uint8_t[cnt];
		SM3_256(buf, cnt, hash);
		delete []buf;
	}
}
