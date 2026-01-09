# MP4onUNO

Reproduce (a muy baja tasa de frames) archivos MP4 en una pantalla ICSST7735 (4-wire) de 160×80 píxeles usando solo un Arduino Uno y comunicación serial a alta velocidad.<br>
<b>Resultado realista actual:</b> ~0.8–1.5 FPS (dependiendo del computador y puerto serie)<br>
<b>¿Qué hace exactamente?</b>
Envía frames pre-procesados en formato RGB565 little-endian desde la PC hacia el Arduino, que los dibuja directamente en la pantalla sin apenas procesamiento.<br>
PC (Python + OpenCV) ──► USB serial (921600 baud) ──► Arduino Uno ──► ST7735 160×80

# Requisitos
<b>Hardware</b>
- Arduino Uno (o compatible)
- Pantalla ST7735 160×80 (tipo "mini 160x80" con controlador ST7735S)

<b>Conexiones típicas:</b>
- TFT_CS   → D10
- TFT_DC   → D9
- TFT_RST  → D8
- TFT_MOSI → D11
- TFT_SCK  → D13
- VCC → 3.3V o 5V (según tu módulo)
- GND → GND

<b>Software – PC</b>
- Python 3.8+
- Librerías: pip install opencv-python pyserial numpy
- Puerto serie rápido (COMx o /dev/ttyACMx con 921600 baud)

<b>Software – Arduino</b>
- Librerías: Adafruit_GFX + Adafruit_ST7735
- Usa la variante initR(INITR_MINI160x80) (muy importante)

# Estado actual del proyecto  

- Funciona .................................... ✓
- Colores correctos (tras ajustes) ............ ✓
- Sincronización frame (header 0xA5 0x5A) ..... ✓
- Tasa realista en Uno @921600 ................ ~1 fps
- Soporta cualquier resolución tras resize .... ✓

# ¿Qué falta / se podría mejorar?

- Alcanzar 4–7 fps reales (muy difícil en Uno, posible en ESP32-S3 o Teensy 4.1)
- Implementar compresión simple RLE o delta-frames
- Soporte para audio (prácticamente imposible en este hardware)
- Mejor manejo de sincronización y reintentos en caso de pérdida de bytes
- Interfaz para seleccionar/pausar video desde Arduino (botones)
- Versión optimizada para ESP32 con SPI más rápido y doble buffering
- Conversor automático de video a secuencia RGB565 (script batch)

# Disclaimer
- Este proyecto es principalmente una prueba de concepto y un ejercicio educativo.
- La tasa de ~1 fps hace que solo sea práctico para efectos visuales lentos, animaciones tipo GIF muy lentas o demostraciones "lo logré".
- No esperes reproducir películas fluidas con un Arduino Uno + pantalla de 2€.
- ¡Cualquier mejora/pull request es bienvenida! 🚀

# Capturas

<b>Script python ejecutando un video</b>

<img width="431" height="103" alt="imagen" src="https://github.com/user-attachments/assets/f55e99e9-85c5-469c-97dc-6e6b62c92235" />

<b>Arduino UNO reproduciendo el video</b>

https://github.com/user-attachments/assets/728fbc71-9592-4a13-857c-fcc09eea2d7d



