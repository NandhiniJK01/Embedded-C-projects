#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "wifi_module.h"  // Simulated Wi-Fi library
#include "sensor_module.h" // Simulated sensor library
#include "relay_control.h" // Simulated relay control for appliances
#include "lcd_display.h"   // Simulated LCD Display library
#include "security_module.h" // Simulated Security System

#define TEMP_THRESHOLD 30
#define GAS_THRESHOLD 200

void init_system();
void read_sensors();
void control_appliances();
void send_data_to_cloud();
void display_status_on_lcd();
void security_system_check();

uint16_t temperature = 0;
uint16_t gas_level = 0;
bool motion_detected = false;
bool light_status = false;
bool door_locked = true;

int main() {
    init_system();
    
    while (1) {
        read_sensors();
        control_appliances();
        security_system_check();
        send_data_to_cloud();
        display_status_on_lcd();
        sleep(2);  // Delay for 2 seconds
    }
    return 0;
}

void init_system() {
    printf("Initializing Smart Home System...\n");
    wifi_init();
    sensor_init();
    relay_init();
    lcd_init();
    security_init();
}

void read_sensors() {
    temperature = read_temperature_sensor();
    gas_level = read_gas_sensor();
    motion_detected = read_motion_sensor();
    printf("Sensors - Temp: %d°C, Gas: %d ppm, Motion: %s\n", temperature, gas_level, motion_detected ? "YES" : "NO");
}

void control_appliances() {
    if (temperature > TEMP_THRESHOLD) {
        relay_turn_on("Fan");
    } else {
        relay_turn_off("Fan");
    }
    
    if (gas_level > GAS_THRESHOLD) {
        relay_turn_on("Exhaust");
    } else {
        relay_turn_off("Exhaust");
    }
    
    if (motion_detected) {
        light_status = true;
        relay_turn_on("Light");
    } else {
        light_status = false;
        relay_turn_off("Light");
    }
}

void send_data_to_cloud() {
    char data[150];
    sprintf(data, "{\"temp\": %d, \"gas\": %d, \"motion\": %d, \"light\": %d, \"door_locked\": %d}", temperature, gas_level, motion_detected, light_status, door_locked);
    wifi_send_data("http://iot-cloud.com/api/data", data);
    printf("Data sent to cloud: %s\n", data);
}

void display_status_on_lcd() {
    char display_msg[100];
    sprintf(display_msg, "Temp: %dC\nGas: %d ppm\nLight: %s\nDoor: %s", temperature, gas_level, light_status ? "ON" : "OFF", door_locked ? "LOCKED" : "UNLOCKED");
    lcd_display_text(display_msg);
}

void security_system_check() {
    if (motion_detected && !door_locked) {
        printf("[ALERT] Intruder detected! Locking doors...\n");
        door_locked = true;
        relay_turn_on("Door Lock");
    }
}
