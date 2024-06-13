#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
} WavHeader;

int main() {
    char filePath[] = "F:\\WAV_project\\example.wav";

    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    WavHeader header;
    fread(&header, sizeof(WavHeader), 1, file);

    uint32_t dataSize = header.subchunk2Size;
    uint32_t numSamples = dataSize / (header.bitsPerSample / 8);

    int16_t *audioData = (int16_t *)malloc(dataSize);
    if (audioData == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    fread(audioData, dataSize, 1, file);

    fclose(file);

    printf("WAV File Header Information:\n");
    printf(" - Sample Rate       : %u Hz\n", header.sampleRate);
    printf(" - Channels          : %u\n", header.numChannels);
    printf(" - Bits Per Sample   : %u bits\n", header.bitsPerSample);

    uint32_t durationInSeconds = header.subchunk2Size / header.byteRate;
    printf("Duration of the WAV file: %u seconds\n", durationInSeconds);

    printf("\nFirst 10 audio samples:\n");
    for (int i = 0; i < 10 && i < numSamples; ++i) {
        printf("%d ", audioData[i]);
    }
    printf("\n");

    free(audioData);

    return 0;
}
