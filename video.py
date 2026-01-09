import cv2
import serial
import time
import numpy as np

PORT = "COM3"          # Ajusta
BAUD = 921600
W, H = 160, 80           # Usa 80x80 para más FPS; cambia a 160,80 para full

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)           # Espera reset Arduino

cap = cv2.VideoCapture("video.mp4")

def frame_to_rgb565_le(frame):
    frame = cv2.resize(frame, (W, H), interpolation=cv2.INTER_AREA)
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    r = (frame[:, :, 0] >> 3).astype(np.uint16)
    g = (frame[:, :, 1] >> 2).astype(np.uint16)
    b = (frame[:, :, 2] >> 3).astype(np.uint16)
    rgb565 = (r << 11) | (g << 5) | b
    flat = rgb565.flatten()
    out = np.empty(flat.size * 2, dtype=np.uint8)
    out[0::2] = flat & 0xFF          # Low byte primero (little-endian)
    out[1::2] = flat >> 8
    return out.tobytes()             # Más eficiente que bytes()

print("Enviando frames... (Ctrl+C para parar)")

frame_count = 0
start_time = time.time()

try:
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        payload = frame_to_rgb565_le(frame)
        ser.write(bytes([0xA5, 0x5A]))  # Header
        ser.write(payload)              # Frame completo

        frame_count += 1

        # Opcional: limitar FPS para no saturar (quita o ajusta)
        time.sleep(0.08)  # ~6-7 fps teóricos

    elapsed = time.time() - start_time
    print(f"Frames enviados: {frame_count} | Tiempo: {elapsed:.1f}s | FPS: {frame_count / elapsed:.1f}")

except KeyboardInterrupt:
    print("Parado por usuario")

finally:
    cap.release()
    ser.close()
