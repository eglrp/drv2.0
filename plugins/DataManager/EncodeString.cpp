#include "stdafx.h"
#include "EncodeString.h"


void ENCODESTRING::EncodeString(string& lpszText, string lpszKey,bool b)
{
	int nTextLen = 0;
	nTextLen = (lpszText).size();
	int nKeyLen = (lpszKey).size();
	int i = 0;
	int k = 0;
	int t = nTextLen;
	int cn = 0;
	for(int a = 0; a < nKeyLen; a++)	// ����Կ����ֵ������
		cn += lpszKey[a];

	for(; i < nTextLen; i++)
	{
		if(b)	// ����
		{
			lpszText[i] = lpszText[i] + t;
			lpszText[i] = lpszText[i] ^ lpszKey[k];
			lpszText[i] = lpszText[i] ^ cn;
		}
		else  //����
		{
			lpszText[i] = lpszText[i] ^ cn;
			lpszText[i] = lpszText[i] ^ lpszKey[k];
			lpszText[i] = lpszText[i] - t;
		}
		k++;
		t--;
		if(k >= nKeyLen)
			k = 0;
		if(t <= 0)
			t = nTextLen;
	}
}

