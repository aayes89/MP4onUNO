#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define W 160  // Efectivo después de rotation 1
#define H 80
#define HDR1 0xA5
#define HDR2 0x5A

uint8_t state = 0;
uint32_t px = 0;
uint8_t loByte;

void setup() {
  Serial.begin(1000000);
  //Serial.begin(921600);
  
  tft.initR(INITR_MINI160x80);  // Específico para tu 80x160 ST7735S
  //tft.initR(INITR_GREENTAB);  // Específico para tu 80x160 ST7735S
  tft.setRotation(3);           // Landscape (prueba 3 si queda al revés)
  tft.invertDisplay(false);      // Corrige colores invertidos en chinas
  
  // Offsets para eliminar mosaico/repeticiones (prueba estas combinaciones una por una)
  tft._colstart = 26;           // 26 es el más común; prueba 24 si ves desplazado
  tft._rowstart = 1;            // 1 o 0; con rotation 1, afecta el eje Y
  
  tft.fillScreen(ST77XX_BLACK); // Limpia para probar
}

void loop() {
  if (Serial.available() >= 2) {  // Optimizado para leer más rápido
    uint8_t b = Serial.read();
    if (state == 0 && b == HDR1) {
      state = 1;
      return;
    }
    if (state == 1) {
      if (b == HDR2) {
        px = 0;
        tft.setAddrWindow(0, 0, W-1, H-1);  // Ventana completa con offsets aplicados
        state = 2;
      } else {
        state = 0;
      }
      return;
    }
    if (state == 2) {
      loByte = b;
      if (Serial.available()) {
        uint8_t hi = Serial.read();
        tft.pushColor((hi << 8) | loByte);  // High byte primero (ajusta si colores mal)
        px++;
        if (px >= (uint32_t)W * H) {
          state = 0;
        }
      }
    }
  }
}
