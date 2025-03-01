#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "audio_input.h"   // Microphone input processing
#include "audio_output.h"  // Speaker output processing
#include "dsp_filter.h"    // Digital filters for ANC
#include "anc_algorithm.h" // Adaptive ANC processing
#include "user_controls.h" // User interface and tuning

#define SAMPLE_RATE 48000  // 48 kHz sample rate
#define BUFFER_SIZE 1024   // Audio buffer size

void init_anc_system();
void capture_noise_reference();
void capture_primary_audio();
void process_anc();
void output_anc_audio();
void adjust_anc_parameters();

float noise_reference_buffer[BUFFER_SIZE];
float primary_audio_buffer[BUFFER_SIZE];
float processed_audio_buffer[BUFFER_SIZE];
bool anc_enabled = true;

int main() {
    init_anc_system();
    
    while (1) {
        capture_noise_reference();
        capture_primary_audio();
        process_anc();
        output_anc_audio();
        adjust_anc_parameters();
    }
    return 0;
}

void init_anc_system() {
    printf("Initializing Adaptive ANC System...\n");
    audio_input_init();
    audio_output_init();
    dsp_filter_init();
    anc_algorithm_init();
    user_controls_init();
}

void capture_noise_reference() {
    read_noise_reference(noise_reference_buffer, BUFFER_SIZE);
    printf("Captured noise reference signal\n");
}

void capture_primary_audio() {
    read_primary_audio(primary_audio_buffer, BUFFER_SIZE);
    printf("Captured primary audio signal\n");
}

void process_anc() {
    if (anc_enabled) {
        apply_adaptive_anc(noise_reference_buffer, primary_audio_buffer, processed_audio_buffer, BUFFER_SIZE);
        printf("Applied adaptive ANC processing\n");
    }
}

void output_anc_audio() {
    output_speaker(processed_audio_buffer, BUFFER_SIZE);
    printf("Playing ANC processed audio\n");
}

void adjust_anc_parameters() {
    anc_enabled = check_anc_status();
    printf("User toggled ANC: %s\n", anc_enabled ? "Enabled" : "Disabled");
}
