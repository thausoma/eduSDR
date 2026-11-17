import numpy as np
import matplotlib.pyplot as plt
import os

file_path = "build/rxdata.pcm"

if not os.path.exists(file_path):
    print(f"Ошибка: Файл {file_path} не найден")
    exit(1)

raw_data = np.fromfile(file_path, dtype=np.int16)

real = raw_data[0::2]
imag = raw_data[1::2]
count = np.arange(len(real))

print(f"Загружено сэмплов: {len(real)}")

plt.figure(figsize=(16, 6))

MAX_SAMPLES_TO_PLOT = 200_000_000

if len(real) > MAX_SAMPLES_TO_PLOT:
    plot_real = real[:MAX_SAMPLES_TO_PLOT]
    plot_imag = imag[:MAX_SAMPLES_TO_PLOT]
    plot_count = count[:MAX_SAMPLES_TO_PLOT]
else:
    plot_real = real
    plot_imag = imag
    plot_count = count

plt.plot(plot_count, plot_real, color='blue', label='I (Real)', alpha=0.8, linewidth=0.5)
plt.plot(plot_count, plot_imag, color='red', label='Q (Imag)', alpha=0.8, linewidth=0.5)

plt.title("Принятый сигнал")
plt.xlabel("Номер сэмпла")
plt.ylabel("Амплитуда")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()