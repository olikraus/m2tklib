
#include "init.h"
#include "u8g_arm.h"

i2c_struct i2c;


uint16_t calc_MapPosExternalMemoryAddress(uint16_t idx)
{
  return idx * sizeof(struct gps_map_pos_struct) + 256*1;
}

void write_MapPos(uint16_t idx, struct gps_map_pos_struct *map_pos)
{
}

uint8_t read_MapPos(uint16_t idx, struct gps_map_pos_struct *map_pos)
{
  uint16_t memadr;
  memadr = calc_MapPosExternalMemoryAddress(idx);
  
  if ( i2c_send_2byte(&i2c, 0x050, memadr >> 8, memadr & 255, 0) != 0 )
  {
    i2c_receive_data(&i2c, 0x050, sizeof(struct gps_map_pos_struct), (uint8_t *)map_pos, 1);
    return 1;
  }
  return 0;
}