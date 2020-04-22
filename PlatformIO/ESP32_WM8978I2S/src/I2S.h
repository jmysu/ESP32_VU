#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"
#define SAMPLE_RATE (48000)
//TTGO tAudio-----------
#define PIN_I2S_BCLK 33
#define PIN_I2S_LRC  25
#define PIN_I2S_DOUT 26
#define PIN_I2S_DIN  27

// 44100Hz, 16bit, stereo
void I2S_Init();
void I2S_Write(char* data, int numData);
int I2S_Read(int16_t* data, int numData);
