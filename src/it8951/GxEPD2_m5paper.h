#ifndef _GxEPD2_m5paper_H_
#define _GxEPD2_m5paper_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_m5paper : public GxEPD2_EPD
{
  public:
    static const uint16_t WIDTH = 540;
    static const uint16_t HEIGHT = 960;
    static const GxEPD2::Panel panel = GxEPD2::M5Paper;
    static const bool hasColor = false;
    static const bool hasPartialUpdate = true;
    static const bool hasFastPartialUpdate = true;
    // TODO: Get actual measurements, these values are copied from the IT60 sample
    static const uint16_t reset_to_ready_time = 1800; // ms, e.g. 1721883us
    static const uint16_t power_on_time = 10; // ms, e.g. 3879us
    static const uint16_t power_off_time = 200; // ms, e.g. 109875us
    static const uint16_t full_refresh_time = 600; // ms, e.g. 573921us
    static const uint16_t partial_refresh_time = 300; // ms, e.g. 246948us
    static const uint16_t refresh_cmd_time = 10; // ms, e.g. 6460us
    static const uint16_t refresh_par_time = 2; // ms, e.g. 1921us
    static const uint16_t default_wait_time = 1; // ms, default busy check, needed?
    static const uint16_t diag_min_time = 3; // ms, e.g. > refresh_par_time
    static const uint16_t set_vcom_time = 40; // ms, e.g. 37833us
    // constructor
    GxEPD2_m5paper(int8_t cs, int8_t dc, int8_t rst, int8_t busy); // TODO: Remove unnecessary pin definitions - the M5Paper has fixed pins
    // methods (virtual)
    void init(uint32_t serial_diag_bitrate = 0); // serial_diag_bitrate = 0 : disabled
    void init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration = 20, bool pulldown_rst_mode = false);
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0x33); // init controller memory and screen (default white)
    void writeScreenBuffer(uint8_t value = 0x33); // init controller memory (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, without screen refresh; x and w should be multiple of 8
    void writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, with screen refresh; x and w should be multiple of 8
    void drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void refresh(bool partial_update_mode = false); // screen refresh from controller memory to full screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen
    void powerOff(); // turns off generation of panel driving voltages, avoids screen fading over time
    void hibernate(); // turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
  private:
    struct IT8951DevInfoStruct
    {
      uint16_t usPanelW;
      uint16_t usPanelH;
      uint16_t usImgBufAddrL;
      uint16_t usImgBufAddrH;
      uint16_t usFWVersion[8];   //16 Bytes String
      uint16_t usLUTVersion[8];   //16 Bytes String
    };
    IT8951DevInfoStruct IT8951DevInfo;
    SPISettings _spi_settings;
    SPISettings _spi_settings_for_read;
  private:
    void _writeScreenBuffer(uint8_t value);
    void _refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode);
    void _send8pixel(uint8_t data);
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _PowerOn();
    void _PowerOff();
    void _InitDisplay();
    void _Init_Full();
    void _Init_Part();
    // IT8951
    void _waitWhileBusy2(const char* comment = 0, uint16_t busy_time = 5000);
    uint16_t _transfer16(uint16_t value);
    void _writeCommand16(uint16_t c);
    void _writeData16(uint16_t d);
    void _writeData16(const uint16_t* d, uint32_t n);
    uint16_t _readData16();
    void _readData16(uint16_t* d, uint32_t n);
    void _writeCommandData16(uint16_t c, const uint16_t* d, uint16_t n);
    void _IT8951SystemRun();
    void _IT8951StandBy();
    void _IT8951Sleep();
    uint16_t _IT8951ReadReg(uint16_t usRegAddr);
    void _IT8951WriteReg(uint16_t usRegAddr, uint16_t usValue);
    uint16_t _IT8951GetVCOM(void);
    void _IT8951SetVCOM(uint16_t vcom);
}