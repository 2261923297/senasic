#include "tools.h"
#include <stdint.h>
#include <qbytearray.h>
#include <iostream>
#include <vector>

 uint8_t IntIoCharHex(uint8_t ch) {
	if (15 >= ch && ch >= 10) {
		ch = ch - 10 + 'A';
	}
	else if (ch >= 0 && ch < 10) {
		ch += '0';
	}
	else {
		printf("ch = %d, cant change to hex\n", ch);
	}
	return ch;
}

 void changeSeq(const void* src, void* dest, int size) {
	const unsigned char
		* tmp_s = (const unsigned char*)src;
	unsigned char* tmp_d = (unsigned char*)dest;
	for (int i = 0, j = size - 1; i < size; i++, j--) {
		tmp_d[j] = tmp_s[i];
	}
}

 int getAddressFromFile(std::string& addr, const std::string& file) {
	FILE* pFile = fopen(file.c_str(), "rb");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", file.c_str());
		return -1;
	}
	fseek(pFile, SEEK_SET, 0);
	char buffer[256] = { 0 };

	fscanf(pFile, "%s\n", buffer);
	addr = buffer;
	return 0;
}

 void print_qbytearray(QByteArray qa) {
	__logger_debug << "ByteArray:";
	for (auto it : qa) { printf("0x%x, ", (unsigned char)it); }
	printf("\n");
}

 void CharToHex(const uint8_t& data, uint8_t buf[2]) {
	buf[1] = (uint8_t)(data & 0x0F);
	buf[0] = (uint8_t)((data & 0xF0) >> 4);

	buf[0] = IntIoCharHex(buf[0]);
	buf[1] = IntIoCharHex(buf[1]);

}

 void ChHexToInt(const uint8_t hex, uint8_t& rt) {
	if (hex >= '0' && hex <= '9') {
		rt = hex - '0';
	}
	else if (hex >= 'a' && hex <= 'f') {
		rt = hex - 'a' + 10;
	}
	else {
		rt = hex - 'A' + 10;
	}
}
 void StrHexToByte(char hex[2], uint8_t& byte) {
	byte = 0;
	uint8_t tmp;
	ChHexToInt(hex[0], tmp);
	byte |= tmp;
	byte <<= 4;
	ChHexToInt(hex[1], tmp);
	byte += tmp;
}

 uint16_t Crc16(uint8_t* buf, uint16_t len, uint16_t Poly, uint16_t init)
{
	uint16_t wCRCin = init;
	uint8_t  wChar = 0;
	uint8_t i;

	while (len--)
	{
		wChar = *buf++;
		wCRCin ^= (wChar << 8);

		for (i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ Poly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	//	printf("crc = 0x%x\n", wCRCin);
	return (wCRCin);
}





 void StrToHex(const std::string& chs, std::string& hex) {
	hex = "";
	unsigned char high, low;
	//	std::cout << "chs size = " << chs.size() << std::endl;
	for (int i = 0; i < chs.size(); i++) {
		high = 0xF0 & (unsigned char)chs[i];
		high >>= 4;
		high &= 0x0F;
		hex.push_back(IntIoCharHex(high));
		low = 0x0F & (unsigned char)chs[i];
		hex.push_back(IntIoCharHex(low));
	}
}

 uint8_t ChHexToByte(const uint8_t data[2]) {
	uint8_t rt = 0;
	uint8_t high = 0;
	uint8_t low = 0;
	ChHexToInt(data[0], high);
	ChHexToInt(data[1], low);
	rt |= high << 4;
	rt |= low;
//	printf("%c%c, %x\n", data[0], data[1], (unsigned char)rt);
	return rt;
}

 void StrHexToBytes(const std::vector<uint8_t>& strs
	, std::vector<uint8_t>& rt) {
	int i = 0;
	while (i < strs.size() - 1) {
		rt.push_back(ChHexToByte(strs.data() + i));
		i += 2;
	}
}
