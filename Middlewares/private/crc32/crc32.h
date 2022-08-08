
#ifndef _CRC32_h
#define _CRC32_h

#include "stdint.h"

extern const uint32_t gdwCrc32Table[256];
//������ʽ��ĺ���
extern void init_crc32_table(uint32_t* crc32Tbl);

//���ó�����ʽ���CRC32У�麯��
extern uint32_t crc32(uint8_t* pucBuff, uint32_t dwLen);

//CRC32���º���
extern void crc32_updata(uint32_t* pdwCrc32, uint8_t* pucBuff, uint32_t dwLen);

//���ñ�����ʽ���CRC32У�麯��
extern uint32_t crc32_tbl(uint32_t* pdwCrc32Tbl, uint8_t* pucBuff, uint32_t dwLen);

#endif
