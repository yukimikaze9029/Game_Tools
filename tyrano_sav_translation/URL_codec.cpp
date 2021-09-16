#include "URL_codec.h"

inline BYTE URL_codec::toHex(const BYTE& x)
{
	return x > 9 ? x - 10 + 'A' : x + '0';
}

inline BYTE URL_codec::fromHex(const BYTE& x)
{
	return isdigit(x) ? x - '0' : x - 'A' + 10;
}

inline string URL_codec::UnicodeToUTF8(BYTE* c_unicode)
{
	string res;
	if (c_unicode[0] != '0' || c_unicode[1] >= '8')
	{
		BYTE ch1, ch2, ch3;
		ch1 = 224 | fromHex(c_unicode[0]);
		ch2 = 128 | (fromHex(c_unicode[1]) << 2) | (fromHex(c_unicode[2]) >> 2);
		ch3 = 128 | ((fromHex(c_unicode[2]) & 3) << 4) | fromHex(c_unicode[3]);
		res += (char)ch1;
		res += (char)ch2;
		res += (char)ch3;
	}
	else if (c_unicode[1] != '0' || c_unicode[2] >= '8')
	{
		BYTE ch1, ch2;
		ch1 = 192 | (fromHex(c_unicode[1]) << 2) | (fromHex(c_unicode[2]) >> 2);
		ch2 = 128 | ((fromHex(c_unicode[2]) & 3) << 4) | fromHex(c_unicode[3]);
		res += (char)ch1;
		res += (char)ch2;
	}
	else
	{
		BYTE ch1;
		ch1 = fromHex(c_unicode[2]) << 4 | fromHex(c_unicode[3]);
		res += ch1;
	}
	return res;
}

string URL_codec::strToUnicode(const BYTE& x1, const BYTE& x2, const BYTE& x3)
{
	string res("%u");
	BYTE ch1, ch2, ch3, ch4;
	ch1 = toHex(x1 % 16);
	ch2 = toHex((x2 % 64) >> 2);
	ch3 = toHex((x2 % 4) << 2 | (x3 % 64) >> 4);
	ch4 = toHex(x3 % 16);
	res += ch1;
	res += ch2;
	res += ch3;
	res += ch4;
	return res;
}

int URL_codec::scanChar(int x)
{
	int res = 0;
	if ((x >= '0' && x <= '9') || (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || x == '.' || x == '_' || x == '/' || x == '*' || x == '-' || x == '+')
	{
		// sav = txt
		res = 0;
	}
	else if ((x >= 0x20 && x <= 0xff) || x == 0xffffffa4)//(x == '{' || x == '}' || x == ':' || x == ',' || x == '\"' || x == '[' || x == ']' || x == '<' || x == '>' || x == ' ' || x == '=' || x == '\'' || x == '\\' || x == '%' || x == ';' || x == '(' || x == ')' || x == '&')
	{
		// sav = %xx
		res = 1;
	}
	else if(x == '\n')
	{
		// ignore
		res = 3;
	}
	else
	{
		// sav = %uxxxx
		res = 2;
	}
	return res;
}

string URL_codec::URL_Decode_UTF8(string& sOrg)
{
	string sOut;
	for (size_t i = 0; i < sOrg.size(); i++)
	{
		BYTE ch = 0;
		if (sOrg[i] == '%')
		{
			if (sOrg[i + 1] == 'u')
			{
				string temp;
				BYTE c_unicode[4] = { (BYTE)sOrg[i + 2], (BYTE)sOrg[i + 3], (BYTE)sOrg[i + 4], (BYTE)sOrg[i + 5] };
				temp = UnicodeToUTF8(c_unicode);
				i += 5;
				sOut += temp;
				continue;
			}
			else
			{
				ch = fromHex(sOrg[i + 1]) << 4;
				ch |= fromHex(sOrg[i + 2]);
				i += 2;
			}
		}
		//else if (sOrg[i] == '+')
		//{
		//	ch = '+';
		//}
		else
		{
			ch = sOrg[i];
		}
		sOut += (char)ch;
		if (ch == ',')
		{
			sOut += '\n';
		}
	}
	return sOut;
}

string URL_codec::URL_Encode_UTF8(string& sOrg)
{
	string sOut;
	for (size_t i = 0; i < sOrg.size(); i++)
	{
		BYTE buf[7];
		memset(buf, 0, 7);
		int n = scanChar(sOrg[i]);
		if (n == 0)
		{
			buf[0] = sOrg[i];
		}
		else if (n == 1)
		{
			buf[0] = '%';
			buf[1] = toHex((BYTE)sOrg[i] >> 4);
			buf[2] = toHex((BYTE)sOrg[i] % 16);
		}
		else if(n == 2)
		{
			string temp;
			temp = strToUnicode((BYTE)sOrg[i], (BYTE)sOrg[i + 1], (BYTE)sOrg[i + 2]);
			i += 2;
			sOut += temp;
			continue;
		}
		else
			continue;
		sOut += (char*)buf;
	}
	return sOut;
}

URL_codec::URL_codec()
{
}

URL_codec::~URL_codec()
{
}

string URL_codec::URL_Decode(string& str)
{
	return URL_Decode_UTF8(str);
}

string URL_codec::URL_Encode(string& str)
{
	return URL_Encode_UTF8(str);
}
