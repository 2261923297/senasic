#pragma once

#define DATA_SIZE uint16_t

struct DATA_PKG {
	DATA_SIZE pkg_num;
	DATA_SIZE pkg_seq;
};

struct ComSt
{
	uint8_t addr;
	uint8_t func_code;
	uint8_t len;

	ComSt() {
		addr = 0;
		func_code = 0;
		len = 0;
	}
};

