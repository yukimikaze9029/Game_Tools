#pragma once
#include "util/util.h"

class URL_codec
{
private:
	BYTE toHex(const BYTE& x);
	BYTE fromHex(const BYTE& x);
	string UnicodeToUTF8(BYTE*);
	string strToUnicode(const BYTE& x1, const BYTE& x2, const BYTE& x3);
	int scanChar(int);

	// URL ���� UTF-8
	string URL_Decode_UTF8(string&);
	// URL ���� UTF-8
	string URL_Encode_UTF8(string&);
public:
	URL_codec();
	~URL_codec();
	// URL ����
	string URL_Decode(string&);
	// URL ����
	string URL_Encode(string&);
};