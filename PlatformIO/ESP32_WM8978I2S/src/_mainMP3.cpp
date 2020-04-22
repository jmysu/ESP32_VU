/*
 * https://github.com/schreibfaul1/ESP32-audioI2S
 * 
 * Modified by Jimmy.Su Apr-2020 with WuHan Cronaviruses
 * 
 */
#include "Arduino.h"

#include "Audio.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"
#include "WM8978.h"
#include "I2S.h"

//TTGO T-Audio SD pinout
#define _SD_MOSI 15
#define _SD_MISO 2
#define _SD_SCK  14
#define _SD_CS   13

//MP3 file on SD card
#define fileMP3 "/diva5.mp3"

// Digital I/O used
//#define SD_CS          5
//#define SPI_MOSI      23
//#define SPI_MISO      19
//#define SPI_SCK       18
//#define I2S_DOUT      25
//#define I2S_BCLK      27
//#define I2S_LRC       26

Audio audio;
//WiFiMulti wifiMulti;

//String ssid =     "xxxxxx";
//String password = "xxxxxx";

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
    audio.connecttoSD(fileMP3);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}

///////////////////////////////////////////////////////////////////////////////
void setup() {
    pinMode(_SD_CS, OUTPUT);      digitalWrite(_SD_CS, HIGH);
    SPI.begin(_SD_SCK, _SD_MISO, _SD_MOSI);
    SPI.setFrequency(1000000);
    Serial.begin(115200);
    SD.begin(_SD_CS);
    /*
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(ssid.c_str(), password.c_str());
    wifiMulti.run();
    if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect(true);
        wifiMulti.run();
      }
      
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(12); // 0...21
    */
    //audio.connecttoSD("/320k_test.mp3");
    //audio.connecttohost("www.wdr.de/wdrlive/media/einslive.m3u");
    //audio.connecttohost("dg-ais-eco-http-fra-eco-cdn.cast.addradio.de/hellwegradio/west/mp3/high");
    //audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
    //audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de");
    
    Serial.println("\nInit WM8978");
    wm8978Setup();
    WM8978_SPKvol_Set(32);      //0-63
    WM8978_HPvol_Set(32, 32);   //0-63
    Serial.println("Init I2S...");
    I2S_Init();
    Serial.println("Starting MP3...\n");
    audio.connecttoSD(fileMP3);
}

void loop()
{
    audio.loop();
}