#ifndef __CRC8_16_H
#define __CRC8_16_H
#include "sys.h"

 

typedef struct {
		u8 date[8];
		u8 time[8];
    u8 p1[7];
//    u8 p2[7];
//    u8 p3[7];
//    u8 T1[6];
//		u8 id[4];
}Sensor_HandleTypeDef;


extern Sensor_HandleTypeDef   A_Sensor_Data;


u16 Get_Crc16(u8 *puchMsg,u16 usDataLen);
u8 Get_Crc8(Sensor_HandleTypeDef *ptr,u16 len);
u8 Get_Crc18(u8 *ptr,u16 len);
#endif




























