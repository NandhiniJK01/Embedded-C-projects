#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "audio_sensor.h"  // Microphone input
#include "dsp.h"           // Digital Signal Processing
#include "effects.h"       // Voice effects (Pitch shift, Robot effect, Reverb, Auto-tune)
#include "noise_filter.h"  // Noise cancellation module
#include "bluetooth.h"     // Bluetooth output
#include "controls.h"      // User control interface
#include "recording.h"     // Audio recording feature

#define SAMPLE_RATE 44100  // 44.1 kHz audio sample rate
#define BUFFER_SIZE 1024   // Audio buffer size

void init_voice_changer();
void capture_audio();
void apply_voice_effects();
void apply_noise_filter();
void output_audio();
void update_display();
void handle_user_input();
void record_audio();

float audio_buffer[BUFFER_SIZE];
float effect_settings[4] = {1.0, 0.5, 0.8, 0.6}; // Pitch shift, Robot effect, Echo level, Reverb level
bool bluetooth_enabled = false;
bool recording_enabled = false;

int main() {
    init_voice_changer();
    
    while (1) {
        capture_audio();
        apply_noise_filter();
        apply_voice_effects();
        output_audio();
        update_display();
        handle_user_input();
        record_audio();
    }
    return 0;
}

void init_voice_changer() {
    printf("Initializing Advanced Real-time Voice Changer...\n");
    audio_sensor_init();
    dsp_init();
    effects_init();
    noise_filter_init();
    bluetooth_init();
    controls_init();
    recording_init();
}

void capture_audio() {
    read_audio_samples(audio_buffer, BUFFER_SIZE);
    printf("Captured audio samples\n");
}

void apply_noise_filter() {
    filter_noise(audio_buffer, BUFFER_SIZE);
    printf("Applied noise reduction\n");
}

void apply_voice_effects() {
    apply_pitch_shift(audio_buffer, BUFFER_SIZE, effect_settings[0]);
    apply_robot_effect(audio_buffer, BUFFER_SIZE, effect_settings[1]);
    apply_echo(audio_buffer, BUFFER_SIZE, effect_settings[2]);
    apply_reverb(audio_buffer, BUFFER_SIZE, effect_settings[3]);
    apply_auto_tune(audio_buffer, BUFFER_SIZE);
    printf("Applied voice effects: Pitch=%.2f, Robot=%.2f, Echo=%.2f, Reverb=%.2f\n", 
           effect_settings[0], effect_settings[1], effect_settings[2], effect_settings[3]);
}

void output_audio() {
    if (bluetooth_enabled) {
        transmit_audio_bluetooth(audio_buffer, BUFFER_SIZE);
        printf("Streaming modified voice via Bluetooth\n");
    } else {
        output_speaker(audio_buffer, BUFFER_SIZE);
        printf("Playing modified voice through speaker\n");
    }
}

void update_display() {
    show_effect_settings(effect_settings);
    printf("Updated display with effect settings\n");
}

void handle_user_input() {
    get_user_effect_settings(effect_settings);
    bluetooth_enabled = check_bluetooth_status();
    recording_enabled = check_recording_status();
    printf("User updated effect settings, Bluetooth: %s, Recording: %s\n", 
           bluetooth_enabled ? "Enabled" : "Disabled", 
           recording_enabled ? "Enabled" : "Disabled");
}

void record_audio() {
    if (recording_enabled) {
        save_audio_to_sd(audio_buffer, BUFFER_SIZE);
        printf("Recording and saving modified voice\n");
    }
}
