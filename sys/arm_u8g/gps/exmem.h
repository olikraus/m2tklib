
#ifndef _EXMEM_H
#define _EXMEM_H

extern uint8_t is_exmem_not_available;

void check_ExternalMemory(void);
void write_MapPos(uint16_t idx, struct gps_map_pos_struct *map_pos);
uint8_t read_MapPos(uint16_t idx, struct gps_map_pos_struct *map_pos);

void write_Preferences(void);
uint8_t read_Preferences(void);

#endif