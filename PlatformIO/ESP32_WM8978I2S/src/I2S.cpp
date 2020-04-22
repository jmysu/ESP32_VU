#include "I2S.h"

void I2S_Init() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 3,
    .dma_buf_len = 512,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = PIN_I2S_BCLK,
    .ws_io_num = PIN_I2S_LRC,
    .data_out_num = PIN_I2S_DOUT,
    .data_in_num = PIN_I2S_DIN
  };
  i2s_driver_install((i2s_port_t)0, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)0, &pin_config);
  i2s_set_clk((i2s_port_t)0, SAMPLE_RATE, (i2s_bits_per_sample_t)16, (i2s_channel_t)2);
  
  //output clock for I2S0 to CLK_OUT1
  //The ESP32 MCLK (I2S master clock) output can be put out through a CLK_OUT pin only.
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
  REG_WRITE(PIN_CTRL, 0xFFFFFFF0);
}

void I2S_Write(char* data, int numData) {
    i2s_write_bytes((i2s_port_t)0, (const char *)data, numData, portMAX_DELAY);
}

int I2S_Read(int16_t* data, int numData) {
    return i2s_read_bytes((i2s_port_t)0, (int16_t *)data, numData, portMAX_DELAY); 
}