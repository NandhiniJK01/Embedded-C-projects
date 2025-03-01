#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "sensor_module.h" // Simulated sensor library
#include "relay_control.h" // Simulated relay control for actuators
#include "uart_comm.h"    // Simulated UART Communication
#include "wifi_module.h"  // Simulated Wi-Fi Module
#include "sms_alert.h"   // Simulated SMS Alert System
#include "email_alert.h" // Simulated Email Alert System

#define TEMP_THRESHOLD 70
#define GAS_THRESHOLD 300
#define SMOKE_THRESHOLD 400

void init_system();
void read_sensors();
void control_safety_measures();
void send_alerts();
void send_sms_alert();
void send_email_alert();

uint16_t temperature = 0;
uint16_t gas_level = 0;
uint16_t smoke_level = 0;
bool alarm_triggered = false;

int main() {
    init_system();
    
    while (1) {
        read_sensors();
        control_safety_measures();
        send_alerts();
        sleep(2);  // Delay for 2 seconds
    }
    return 0;
}

void init_system() {
    printf("Initializing Industrial Safety System...\n");
    sensor_init();
    relay_init();
    uart_init();
    wifi_init();
    sms_alert_init();
    email_alert_init();
}

void read_sensors() {
    temperature = read_temperature_sensor();
    gas_level = read_gas_sensor();
    smoke_level = read_smoke_sensor();
    printf("Sensors - Temp: %d°C, Gas: %d ppm, Smoke: %d ppm\n", temperature, gas_level, smoke_level);
}

void control_safety_measures() {
    alarm_triggered = false;
    if (temperature > TEMP_THRESHOLD) {
        relay_turn_on("Cooling Fan");
    } else {
        relay_turn_off("Cooling Fan");
    }
    
    if (gas_level > GAS_THRESHOLD || smoke_level > SMOKE_THRESHOLD) {
        relay_turn_on("Exhaust Fan");
        relay_turn_on("Water Sprinkler");
        relay_turn_on("Emergency Alarm");
        alarm_triggered = true;
        send_sms_alert();
        send_email_alert();
    } else {
        relay_turn_off("Exhaust Fan");
        relay_turn_off("Water Sprinkler");
        relay_turn_off("Emergency Alarm");
    }
}

void send_alerts() {
    char log_data[150];
    sprintf(log_data, "Temp: %d, Gas: %d, Smoke: %d, Alarm: %d", temperature, gas_level, smoke_level, alarm_triggered);
    uart_send(log_data);
    wifi_send_data("http://industrial-alerts.com/api/logs", log_data);
    printf("Data sent to central monitoring system: %s\n", log_data);
}

void send_sms_alert() {
    if (alarm_triggered) {
        char sms_msg[100];
        sprintf(sms_msg, "ALERT! Temp: %dC, Gas: %d ppm, Smoke: %d ppm. Take Action!", temperature, gas_level, smoke_level);
        sms_send("+1234567890", sms_msg);
        printf("SMS Alert Sent: %s\n", sms_msg);
    }
}

void send_email_alert() {
    if (alarm_triggered) {
        char email_msg[200];
        sprintf(email_msg, "URGENT: Industrial Safety Alert!\nTemperature: %dC\nGas Level: %d ppm\nSmoke Level: %d ppm\nImmediate action required!", temperature, gas_level, smoke_level);
        email_send("admin@industry.com", "Industrial Safety Alert", email_msg);
        printf("Email Alert Sent\n");
    }
}
