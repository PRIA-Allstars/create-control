#include <stdint.h>

#define CREATE_FULL_MODE 1
#define CREATE_SAVE_MODE 2

int create_connect();
void create_disconnect();

void set_create_mode(char mode);

void create_drive_direct(int16_t left_speed, int16_t right_speed);
void create_drive(int16_t speed, int16_t radius);
void create_turn(int16_t speed, int16_t angle);

void create_wait_angle(int16_t angle);

uint8_t get_create_left_bump();
uint8_t get_create_right_bump();
uint8_t get_create_left_wheel_drop();
uint8_t get_create_right_wheel_drop();
uint8_t get_create_caster_wheel_drop();
uint8_t get_create_wall();

uint8_t get_create_left_cliff();
uint8_t get_create_right_cliff();
uint8_t get_create_left_front_cliff();
uint8_t get_create_right_front_cliff();

uint16_t get_create_left_cliff_analog();
uint16_t get_create_right_cliff_analog();
uint16_t get_create_left_front_cliff_analog();
uint16_t get_create_right_front_cliff_analog();

int16_t get_create_angle();
