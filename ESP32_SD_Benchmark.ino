/* Uncomment one and only one below Dev Device pins header */
#include <PINS_ESP32-S3-CAM.h>
// #include <PINS_T-DECK.h>
// #include <PINS_JC1060P470.h>
// #include <PINS_ESP32-S3-Touch-LCD-2_8.h>
// #include <PINS_JC8012P4A1.h>

#ifndef DEV_DEVICE_PINS
#error "Please include one of the Dev Device pins header"
#endif

#ifndef SD_SUPPORTED
#error "No SD pins defined in this Dev Device"
#endif

#include <stdio.h>
#include <FS.h>
#include <SD.h>
#include <SD_MMC.h>
#include <FFat.h>
#include <LittleFS.h>

#define TEST_FILE_SIZE (4 * 1024 * 1024)

void testWriteFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  unsigned long start_time = millis();
  Serial.printf("Test write %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  int loop = TEST_FILE_SIZE / len;
  while (loop--)
  {
    if (!file.write(buf, len))
    {
      Serial.println("Write failed");
      return;
    }
  }
  file.flush();
  file.close();
  unsigned long time_used = millis() - start_time;
  Serial.printf("Write file used: %d ms, %f KB/s\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testRawWriteFile(const char *path, uint8_t *buf, int len)
{
  unsigned long start_time = millis();
  Serial.printf("Test write %s\n", path);

  FILE *file = fopen(path, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  int loop = TEST_FILE_SIZE / len;
  while (loop--)
  {
    if (!write(fileno(file), buf, len))
    {
      Serial.println("Write failed");
      return;
    }
  }
  fflush(file);
  fclose(file);
  unsigned long time_used = millis() - start_time;
  Serial.printf("Write file used: %d ms, %f KB/s\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testReadFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  unsigned long start_time = millis();
  Serial.printf("Test read %s\n", path);

  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  int loop = TEST_FILE_SIZE / len;
  while (loop--)
  {
    if (!file.read(buf, len))
    {
      Serial.println("Read failed");
      return;
    }
  }
  file.close();
  unsigned long time_used = millis() - start_time;
  Serial.printf("Read file used: %d ms, %f KB/s\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testRawReadFile(const char *path, uint8_t *buf, int len)
{
  unsigned long start_time = millis();
  Serial.printf("Test raw read %s\n", path);

  FILE *file = fopen(path, "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  int loop = TEST_FILE_SIZE / len;
  while (loop--)
  {
    if (!read(fileno(file), buf, len))
    {
      Serial.println("Read failed");
      return;
    }
  }
  fclose(file);
  unsigned long time_used = millis() - start_time;
  Serial.printf("Read file used: %d ms, %f KB/s\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testIO(fs::FS &fs)
{
  /* malloc will not reset all bytes to zero, so it is a random data */
  uint8_t *buf = (uint8_t *)malloc(64 * 1024);

  testWriteFile(fs, "/test_1k.bin", buf, 1024);
  testWriteFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testWriteFile(fs, "/test_4k.bin", buf, 4 * 1024);
  // testWriteFile(fs, "/test_8k.bin", buf, 8 * 1024);
  // testWriteFile(fs, "/test_16k.bin", buf, 16 * 1024);
  // testWriteFile(fs, "/test_32k.bin", buf, 32 * 1024);
  // testWriteFile(fs, "/test_64k.bin", buf, 64 * 1024);

  delay(10000);

  testReadFile(fs, "/test_1k.bin", buf, 1024);
  testReadFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testReadFile(fs, "/test_4k.bin", buf, 4 * 1024);
  // testReadFile(fs, "/test_8k.bin", buf, 8 * 1024);
  // testReadFile(fs, "/test_16k.bin", buf, 16 * 1024);
  // testReadFile(fs, "/test_32k.bin", buf, 32 * 1024);
  // testReadFile(fs, "/test_64k.bin", buf, 64 * 1024);
}

void testRawIO()
{
  /* malloc will not reset all bytes to zero, so it is a random data */
  uint8_t *buf = (uint8_t *)malloc(64 * 1024);

  testRawWriteFile("/root/test_1k.bin", buf, 1024);
  testRawWriteFile("/root/test_2k.bin", buf, 2 * 1024);
  testRawWriteFile("/root/test_4k.bin", buf, 4 * 1024);
  // testRawWriteFile("/root/test_8k.bin", buf, 8 * 1024);
  // testRawWriteFile("/root/test_16k.bin", buf, 16 * 1024);
  // testRawWriteFile("/root/test_32k.bin", buf, 32 * 1024);
  // testRawWriteFile("/root/test_64k.bin", buf, 64 * 1024);

  delay(10000);

  testRawReadFile("/root/test_1k.bin", buf, 1024);
  testRawReadFile("/root/test_2k.bin", buf, 2 * 1024);
  testRawReadFile("/root/test_4k.bin", buf, 4 * 1024);
  // testRawReadFile("/root/test_8k.bin", buf, 8 * 1024);
  // testRawReadFile("/root/test_16k.bin", buf, 16 * 1024);
  // testRawReadFile("/root/test_32k.bin", buf, 32 * 1024);
  // testRawReadFile("/root/test_64k.bin", buf, 64 * 1024);
}

void setup()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  delay(1000);
  Serial.println("ESP32 SD Benchmark");

#if defined(SPI_SHARED)
  Serial.println("test SD SPI Mode");
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  if (!SD.begin(SD_CS, SPI, 80000000, "/root"))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  testIO(SD);
  testRawIO();
#elif defined(SD_D1)
  Serial.println("test SD_MMC 4-bit Mode");
#if defined(SOC_SDMMC_USE_GPIO_MATRIX)
  SD_MMC.setPins(SD_SCK, SD_MOSI, SD_MISO, SD_D1, SD_D2, SD_CS);
#endif
  if (!SD_MMC.begin("/root", false /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_HIGHSPEED))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  testIO(SD_MMC);
  testRawIO();
#else
  Serial.println("test SD_MMC 1-bit Mode");
#if defined(SOC_SDMMC_USE_GPIO_MATRIX)
  SD_MMC.setPins(SD_SCK, SD_MOSI, SD_MISO);
#endif
  if (!SD_MMC.begin("/root", true /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_DEFAULT))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  testIO(SD_MMC);
  testRawIO();
#else
  if (!LittleFS.begin(true, "/root"))
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  else
  {
    Serial.println("test LittleFS");
    testIO(LittleFS);
    testRawIO();
  }

  if (!FFat.begin(true, "/root"))
  {
    Serial.println("FFat Mount Failed");
    return;
  }
  else
  {
    Serial.println("test FFat");
    testIO(FFat);
    testRawIO();
  }
#endif
}

void loop()
{
}
