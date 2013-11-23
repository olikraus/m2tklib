
#include "init.h"
#include "u8g.h"		/* u8g_Delay() */
#include "u8g_arm.h"

i2c_struct i2c;
uint8_t is_exmem_not_available = 0;

uint16_t calc_MapPosExternalMemoryAddress(uint16_t idx)
{
  /* return idx * sizeof(struct gps_map_pos_struct) + 256*1; */
  /* assumes sizeof(struct gps_map_pos_struct) < 128 */
  return idx * 128 + 256*1;
}

void write_MapPos(uint16_t idx, struct gps_map_pos_struct *map_pos)
{
  uint16_t memadr;
  uint8_t buf[2];
  
  if ( is_exmem_not_available != 0 )
    return;
  
  memadr = calc_MapPosExternalMemoryAddress(idx);
  
  buf[0] = memadr >> 8;
  buf[1] = memadr & 255;
  
  i2c_send_pre_data(&i2c, 0x050, 2, buf, sizeof(struct gps_map_pos_struct), (uint8_t *)map_pos);
  u8g_Delay(5);

  /*
  if ( i2c_send_2byte(&i2c, 0x050, memadr >> 8, memadr & 255, 0) != 0 )
  {
  }
  */
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

void write_Preferences(void)
{
  uint8_t buf[2] = { 0, 0 };
  if ( is_exmem_not_available != 0 )
    return;
  
  
  i2c_send_pre_data(&i2c, 0x050, 2, buf, sizeof(struct gps_tracker_preferences_struct),
    (uint8_t *)&(gps_tracker_variables.pref));

  u8g_Delay(5);
}

uint8_t read_Preferences(void)
{
  if ( i2c_send_2byte(&i2c, 0x050, 0, 0, 0) != 0 )
  {
    i2c_receive_data(&i2c, 0x050, sizeof(struct gps_tracker_preferences_struct), (uint8_t *)&(gps_tracker_variables.pref), 1);
    return 1;
  }
  return 0;
}

void check_ExternalMemory(void)
{
  is_exmem_not_available = 0;
  if ( i2c_send_2byte(&i2c, 0x050, 0, 0, 1) == 0 )
    is_exmem_not_available = 1;
}
