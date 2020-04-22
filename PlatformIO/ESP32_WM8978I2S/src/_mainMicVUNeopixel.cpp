/*------------------------------------------------------------------------------
  Adapted Code refrenced by squix78.
  https://blog.squix.org/2019/08/esp32-esp-eye-browser-based-spectrum-analyzer.html
  

  Modified for TTGO T-Audio w/ Neopixels VU
  by Jimmy.Su Apr-20-2020
------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <math.h>

#include <arduinoFFT.h> 
#include <driver/i2s.h>
#include "I2S.h"
#include "WM8978.h"

const i2s_port_t I2S_PORT = I2S_NUM_0;
const int BLOCK_SIZE = 128;
int32_t samples[BLOCK_SIZE];

#include <Adafruit_NeoPixel.h>
// NEOPIXEL SETUP
#define PIN            22
#define NUMPIXELS      19

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const uint32_t NEO_BG = pixels.Color(0, 0, 150);
const uint32_t NEO_FG = pixels.Color(50, 250, 50);


void NeoSpinner(int pos) {
  for (int i=pos+1; i<=NUMPIXELS; i++) {
    pixels.setPixelColor(i, NEO_BG);
    }
  for (int i=0; i<pos+1; i++) {
    int iV = map(pos,0,200,0,100);
    pixels.setPixelColor(i%NUMPIXELS, pixels.Color(iV, 250, 50)); // rotate clockwise
    }
  //pixels.setBrightness(1+(pos>>1));  
  pixels.show();  
}

/*
const double signalFrequency = 1000;
const double samplingFrequency = 48000;
const uint8_t amplitude = 150;
double vReal[BLOCK_SIZE];
double vImag[BLOCK_SIZE];
String labels[] = {"125", "250", "500", "1K", "2K", "4K", "8K", "16K"};
arduinoFFT FFT = arduinoFFT(); // Create FFT object 
int bands[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setupMic() {
  Serial.println("Configuring I2S...");
  esp_err_t err;

  // The I2S config as per the example
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
      .sample_rate = samplingFrequency,                        
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // could only get it to work with 32bits
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // although the SEL config should be left, it seems to transmit on right
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,     // Interrupt level 1
      .dma_buf_count = 4,                           // number of buffers
      .dma_buf_len = BLOCK_SIZE                     // samples per buffer
  };

  // The pin config as per the setup
    i2s_pin_config_t pin_config = {
        .bck_io_num = 26,  // IIS_SCLK
        .ws_io_num = 32,   // IIS_LCLK
        .data_out_num = -1,// IIS_DSIN
        .data_in_num = 33  // IIS_DOUT
    };

  // Configuring the I2S driver and pins.
  // This function must be called before any I2S driver read/write operations.
  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed installing driver: %d\n", err);
    while (true);
  }
  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed setting pin: %d\n", err);
    while (true);
  }
  Serial.println("I2S driver installed.");
}
*/


///////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(100);
  Serial.println("Setting up I2S");
  I2S_Init();
  wm8978Setup();
  Serial.println("I2S setup completed, VU started!");

  pixels.begin();
  pixels.setBrightness(1);
  pixels.show();
}

int iVU; //sound pressure 0~1024
int iPos, iPosOld;
void loop() {
  // put your main code here, to run repeatedly:

  // Read multiple samples at once and calculate the sound pressure
  int num_bytes_read = i2s_read_bytes(I2S_PORT, 
                                      (char *)samples, 
                                      BLOCK_SIZE,     // the doc says bytes, but its elements.
                                      portMAX_DELAY); // no timeout

  int samples_read = num_bytes_read / 8;

  if (samples_read > 0) {
    float mean = 0;
    int32_t inorm = 0;
    int32_t imax=-1000000;
    int32_t imin= 1000000;

    for (int i = 0; i < samples_read; ++i) {
      int32_t d16 = samples[i] >> 16; //Adjust for I2S_COMM_FORMAT_I2S_MSB 16bit from 32bit sample
      mean += d16;
      }      
    mean = mean / samples_read;
    for (int i=0; i<samples_read;i++) {  
      int32_t d16 = samples[i] >> 16;
      inorm = d16-mean; //DC removal
      imax = max(imax, inorm);
      imin = min(imin, inorm);
      //Serial.printf("%5d ", inorm);
      }
    iVU = (imax - imin);  
    //iPos = map(iVU,0,100,0,18);
    iPos = iVU >> 5;
    Serial.printf("\tMean/VU:%5.0f %5d %3d\n", mean, iVU, iPos);
  }

  if ( abs(iPos-iPosOld)>2 ) {
    iPosOld = iPos;
    NeoSpinner(iPos);
    delay(iPos*10);
    }

/* //for FFT processing
  for (uint16_t i = 0; i < BLOCK_SIZE; i++) {
    //vReal[i] = samples[i] << 8;
    vReal[i] = samples[i];
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
    }

  FFT.Windowing(vReal, BLOCK_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, BLOCK_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, BLOCK_SIZE);
  for (int i = 0; i < 8; i++) {
    bands[i] = 0;
    }
  
  for (int i = 2; i < (BLOCK_SIZE/2); i++){ // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
    if (vReal[i] > 2000) { // Add a crude noise filter, 10 x amplitude or more
      if (i<=2 )             bands[0] = max(bands[0], (int)(vReal[i]/amplitude)); // 125Hz
      if (i >3   && i<=5 )   bands[1] = max(bands[1], (int)(vReal[i]/amplitude)); // 250Hz
      if (i >5   && i<=7 )   bands[2] = max(bands[2], (int)(vReal[i]/amplitude)); // 500Hz
      if (i >7   && i<=15 )  bands[3] = max(bands[3], (int)(vReal[i]/amplitude)); // 1000Hz
      if (i >15  && i<=30 )  bands[4] = max(bands[4], (int)(vReal[i]/amplitude)); // 2000Hz
      if (i >30  && i<=53 )  bands[5] = max(bands[5], (int)(vReal[i]/amplitude)); // 4000Hz
      if (i >53  && i<=200 ) bands[6] = max(bands[6], (int)(vReal[i]/amplitude)); // 8000Hz
      if (i >200           ) bands[7] = max(bands[7], (int)(vReal[i]/amplitude)); // 16000Hz
        }

    //for (byte band = 0; band <= 6; band++) display.drawHorizontalLine(18*band,64-peak[band],14);
    }
  for (int i = 0; i < 8; i++) {
    Serial.printf("% 10d ", bands[i]);
    }
  Serial.println();
  */

}