import numpy as np
import matplotlib.pyplot as plt

name = "build/rxdata.pcm"

raw_data = np.fromfile(name, dtype=np.int16)
real = raw_data[0::2]
imag = raw_data[1::2]
count = np.arange(len(real))

plt.figure(figsize=(15, 6))
plt.plot(count, real, color='blue', label='I (Real)', alpha=0.7)
plt.plot(count, imag, color='red', label='Q (Imag)', alpha=0.7)
plt.title("Принятые IQ-отсчеты")
plt.xlabel("Номер сэмпла")
plt.ylabel("Амплитуда")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()