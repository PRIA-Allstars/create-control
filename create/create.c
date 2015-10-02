#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringSerial.h>

#include "create.h"
#include "create_io_codes.h"

int create_fd = 0;

int create_connect(){
	create_fd = serialOpen("/dev/ttyAMA0", 57600);
	if(create_fd == -1){
		return 0;
	}
	serialPutchar(create_fd, IO_START);
	usleep(10000);
	serialPutchar(create_fd, IO_SAFE);
	usleep(10000);
	serialFlush(create_fd);
	return 1;
}

void create_disconnect(){
	serialClose(create_fd);
}

void set_create_mode(char mode){
	if(mode == CREATE_SAVE_MODE){
		serialPutchar(create_fd, IO_SAFE);
	} else {
		serialPutchar(create_fd, IO_FULL);
	}
	usleep(10000);
}

void create_drive_direct(int16_t left_speed, int16_t right_speed){
	serialPutchar(create_fd, IO_DRIVE_DIRECT);
	usleep(10000);
	uint8_t right_speed_low = (uint8_t)right_speed;
	uint8_t right_speed_high = (uint8_t)(right_speed >> 8);
	uint8_t left_speed_low = (uint8_t)left_speed;
	uint8_t left_speed_high = (uint8_t)(left_speed >> 8);
	serialPutchar(create_fd, right_speed_high);
	usleep(10000);
	serialPutchar(create_fd, right_speed_low);
	usleep(10000);
	serialPutchar(create_fd, left_speed_high);
	usleep(10000);
	serialPutchar(create_fd, left_speed_low);
	usleep(10000);
}

void create_drive(int16_t speed, int16_t radius){
	serialPutchar(create_fd, IO_DRIVE);
	usleep(10000);
	uint8_t speed_high = (uint8_t)(speed >> 8);
	uint8_t speed_low = (uint8_t) speed;
	uint8_t radius_high = (uint8_t)(radius >> 8);
	uint8_t radius_low = (uint8_t) radius;
	serialPutchar(create_fd, speed_high);
	usleep(10000);
	serialPutchar(create_fd, speed_low);
	usleep(10000);
	serialPutchar(create_fd, radius_high);
	usleep(10000);
	serialPutchar(create_fd, radius_low);
	usleep(10000);
}

void create_turn(int16_t speed, int16_t angle) {
	create_drive_direct(0,0);
	//usleep(1000000);
	if(angle < 0) speed *= -1;
	create_drive_direct(-1*speed,speed);
	create_wait_angle(angle);
	get_create_right_cliff();
	create_drive_direct(0,0);
}

uint8_t get_create_left_bump(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_BUMPS_AND_WHEEL_DROPS);
	return (serialGetchar(create_fd) >> 1) & 1 ;
}

uint8_t get_create_right_bump(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_BUMPS_AND_WHEEL_DROPS);
	return serialGetchar(create_fd) & 1;
}

uint8_t get_create_left_wheel_drop() {
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_BUMPS_AND_WHEEL_DROPS);
	return (serialGetchar(create_fd) & ( 1 << 3)) >> 3;
}

uint8_t get_create_right_wheel_drop() {
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_BUMPS_AND_WHEEL_DROPS);
	return (serialGetchar(create_fd) & ( 1 << 2)) >> 2;
}


uint8_t get_create_caster_wheel_drop() {
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_BUMPS_AND_WHEEL_DROPS);
	return (serialGetchar(create_fd) & ( 1 << 4)) >> 4;
}

uint8_t get_create_wall(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_WALL);
	return serialGetchar(create_fd);
}

uint8_t get_create_left_cliff(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_LEFT);
	return serialGetchar(create_fd);;
}

uint8_t get_create_right_cliff(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_RIGHT);
	return serialGetchar(create_fd);
}

uint8_t get_create_left_front_cliff(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_FRONT_LEFT);
	return serialGetchar(create_fd);
}

uint8_t get_create_right_front_cliff(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_FRONT_RIGHT);
	return serialGetchar(create_fd);
}

uint16_t get_create_left_cliff_analog(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_LEFT_SIGNAL);
	uint16_t high = serialGetchar(create_fd) << 8;
	uint8_t low = serialGetchar(create_fd);
	return high | (uint16_t)low;
}

uint16_t get_create_right_cliff_analog(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_RIGHT_SIGNAL);
	uint16_t high = serialGetchar(create_fd) << 8;
	uint8_t low = serialGetchar(create_fd);
	return high | (uint16_t)low;
}

uint16_t get_create_left_front_cliff_analog(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_LEFT_FRONT_SIGNAL);
	uint16_t high = serialGetchar(create_fd) << 8;
	uint8_t low = serialGetchar(create_fd);
	return high | (uint16_t)low;
}

uint16_t get_create_right_front_cliff_analog(){
	serialPutchar(create_fd, IO_SENSORS);
	usleep(10000);
	serialPutchar(create_fd, IO_CLIFF_RIGHT_FRONT_SIGNAL);
	uint16_t high = serialGetchar(create_fd) << 8;
	uint8_t low = serialGetchar(create_fd);
	return high | (uint16_t)low;
}

void create_wait_angle(int16_t angle) {
	serialPutchar(create_fd, IO_WAIT_ANGLE);
	usleep(10000);
	serialPutchar(create_fd, (uint8_t)(angle >> 8));
	usleep(10000);
	serialPutchar(create_fd, (uint8_t) angle);
	usleep(10000);
}

int16_t get_create_angle(){
	serialPutchar(create_fd, IO_SENSORS);
        usleep(10000);
        serialPutchar(create_fd, IO_ANGLE);
        int16_t high = serialGetchar(create_fd) << 8;
        uint8_t low = serialGetchar(create_fd);
        return high | (int16_t)low;
}
