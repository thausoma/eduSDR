#include <SoapySDR/Device.h>
#include <SoapySDR/Formats.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    // Инициализация устройства
    SoapySDRKwargs args = {};
    SoapySDRKwargs_set(&args, "driver", "plutosdr");
    SoapySDRKwargs_set(&args, "uri", "usb:");
    
    SoapySDRDevice *sdr = SoapySDRDevice_make(&args);
    SoapySDRKwargs_clear(&args);

    if (sdr == NULL)
    {
        printf("SoapySDRDevice_make fail: %s\n", SoapySDRDevice_lastError());
        return EXIT_FAILURE;
    }

    // Настройка RX
    if (SoapySDRDevice_setSampleRate(sdr, SOAPY_SDR_RX, 0, 1e6) != 0)
    {
        printf("setSampleRate rx fail: %s\n", SoapySDRDevice_lastError());
    }
    if (SoapySDRDevice_setFrequency(sdr, SOAPY_SDR_RX, 0, 800e6, NULL) != 0)
    {
        printf("setFrequency rx fail: %s\n", SoapySDRDevice_lastError());
    }
    if (SoapySDRDevice_setGain(sdr, SOAPY_SDR_RX, 0, 40.0) != 0)
    {
        printf("setGain rx fail: %s\n", SoapySDRDevice_lastError());
    }

    // Настройка потока
    size_t channels[] = {0};
    SoapySDRStream *rxStream = SoapySDRDevice_setupStream(
        sdr, SOAPY_SDR_RX, SOAPY_SDR_CS16, channels, 1, NULL);
    
    if (rxStream == NULL)
    {
        printf("setupStream rx fail: %s\n", SoapySDRDevice_lastError());
        SoapySDRDevice_unmake(sdr);
        return EXIT_FAILURE;
    }

    SoapySDRDevice_activateStream(sdr, rxStream, 0, 0, 0);

    // Получение MTU
    size_t rx_mtu = SoapySDRDevice_getStreamMTU(sdr, rxStream);
    printf("MTU RX: %lu\n", rx_mtu);

    int16_t rx_buffer[2 * rx_mtu];

    // Открытие файла для записи
    FILE *file = fopen("rxdata.pcm", "wb");
    if (!file)
    {
        printf("Не удалось открыть файл для записи!\n");
        return EXIT_FAILURE;
    }

    printf("Начинаем прием IQ-отсчетов...\n");

    // Прием данных
    size_t iteration_count = 100;
    for (size_t i = 0; i < iteration_count; i++)
    {
        void *rx_buffs[] = {rx_buffer};
        int flags;
        long long timeNs;

        int sr = SoapySDRDevice_readStream(
            sdr, rxStream, rx_buffs, rx_mtu, &flags, &timeNs, 1000000);

        if (sr > 0)
        {
            fwrite(rx_buffer, 2 * sr * sizeof(int16_t), 1, file);
        }
        else if (sr < 0)
        {
            printf("Ошибка чтения: %s\n", SoapySDRDevice_lastError());
        }
    }

    fclose(file);
    printf("Прием завершен. Файл rxdata.pcm сохранен.\n");

    // Очистка
    SoapySDRDevice_deactivateStream(sdr, rxStream, 0, 0);
    SoapySDRDevice_closeStream(sdr, rxStream);
    SoapySDRDevice_unmake(sdr);

    return EXIT_SUCCESS;
}