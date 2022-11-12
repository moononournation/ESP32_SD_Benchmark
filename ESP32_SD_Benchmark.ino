/*******************
 * For ESP32 only, connect the SD card to the following pins:
 * SD Card | ESP32
 *    D2       12
 *    D3       13
 *    CMD      15
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      14
 *    VSS      GND
 *    D0       2  (add 1K pull up after flashing)
 *    D1       4
 * 
 * For ESP32-S3 the pins are configurable
*/
#include <stdio.h>
#include <FS.h>
#include <SD.h>
#include <SD_MMC.h>

// esp32s3usbotg
#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32)
#define SDMMC_D2 12  // SDMMC Data2
#define SDMMC_D3 13  // SDMMC Data3 / SPI CS
#define SDMMC_CMD 15 // SDMMC CMD   / SPI MOSI
#define SDMMC_CLK 14 // SDMMC CLK   / SPI SCK
#define SDMMC_D0 2   // SDMMC Data0 / SPI MISO
#define SDMMC_D1 4   // SDMMC Data1
#elif defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
#define SDMMC_D2 33  // SDMMC Data2
#define SDMMC_D3 34  // SDMMC Data3 / SPI CS
#define SDMMC_CMD 35 // SDMMC CMD   / SPI MOSI
#define SDMMC_CLK 36 // SDMMC CLK   / SPI SCK
#define SDMMC_D0 37  // SDMMC Data0 / SPI MISO
#define SDMMC_D1 38  // SDMMC Data1
#endif

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
  testWriteFile(fs, "/test_8k.bin", buf, 8 * 1024);
  testWriteFile(fs, "/test_16k.bin", buf, 16 * 1024);
  testWriteFile(fs, "/test_32k.bin", buf, 32 * 1024);
  testWriteFile(fs, "/test_64k.bin", buf, 64 * 1024);

  delay(10000);

  testReadFile(fs, "/test_1k.bin", buf, 1024);
  testReadFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testReadFile(fs, "/test_4k.bin", buf, 4 * 1024);
  testReadFile(fs, "/test_8k.bin", buf, 8 * 1024);
  testReadFile(fs, "/test_16k.bin", buf, 16 * 1024);
  testReadFile(fs, "/test_32k.bin", buf, 32 * 1024);
  testReadFile(fs, "/test_64k.bin", buf, 64 * 1024);
}

void testRawIO()
{
  /* malloc will not reset all bytes to zero, so it is a random data */
  uint8_t *buf = (uint8_t *)malloc(64 * 1024);

  testRawWriteFile("/root/test_1k.bin", buf, 1024);
  testRawWriteFile("/root/test_2k.bin", buf, 2 * 1024);
  testRawWriteFile("/root/test_4k.bin", buf, 4 * 1024);
  testRawWriteFile("/root/test_8k.bin", buf, 8 * 1024);
  testRawWriteFile("/root/test_16k.bin", buf, 16 * 1024);
  testRawWriteFile("/root/test_32k.bin", buf, 32 * 1024);
  testRawWriteFile("/root/test_64k.bin", buf, 64 * 1024);

  delay(10000);

  testRawReadFile("/root/test_1k.bin", buf, 1024);
  testRawReadFile("/root/test_2k.bin", buf, 2 * 1024);
  testRawReadFile("/root/test_4k.bin", buf, 4 * 1024);
  testRawReadFile("/root/test_8k.bin", buf, 8 * 1024);
  testRawReadFile("/root/test_16k.bin", buf, 16 * 1024);
  testRawReadFile("/root/test_32k.bin", buf, 32 * 1024);
  testRawReadFile("/root/test_64k.bin", buf, 64 * 1024);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while (!Serial);

  /* test SD_MMC 4-bit Mode */
#if defined(SOC_SDMMC_USE_GPIO_MATRIX)
  SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0, SDMMC_D1, SDMMC_D2, SDMMC_D3);
#endif
  if (!SD_MMC.begin("/root"))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  testIO(SD_MMC);
  testRawIO();

  /* test SD_MMC 1-bit Mode */
  // pinMode(SDMMC_D3 /* CS */, OUTPUT);
  // digitalWrite(SDMMC_D3 /* CS */, HIGH);
  // #if defined(SOC_SDMMC_USE_GPIO_MATRIX)
  // SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0);
  // #endif
  // if (!SD_MMC.begin("/root", true))
  // {
  //   Serial.println("Card Mount Failed");
  //   return;
  // }
  // testIO(SD_MMC);
  // testRawIO();

  /* test SD SPI Mode at HSPI */
  // SPIClass spi = SPIClass(HSPI);
  // spi.begin(SDMMC_CLK, SDMMC_D0 /* MISO */, SDMMC_CMD /* MOSI */, SDMMC_D3 /* SS */);
  // if (!SD.begin(SDMMC_D3 /* SS */, spi, 80000000, "/root"))
  // {
  //   Serial.println("Card Mount Failed");
  //   return;
  // }
  // testIO(SD);
  // testRawIO();

  /* test SD SPI Mode at VSPI */
  // #define SD_CS 4
  // SPIClass spi = SPIClass(VSPI);
  // spi.begin(18 /* SCK */, 19 /* MISO */, 23 /* MOSI */, SD_CS /* SS */);
  // if (!SD.begin(SD_CS /* SS */, spi, 80000000, "/root"))
  // {
  //   Serial.printf("Card Mount Failed, SD_CS: %d\n", SD_CS);
  //   return;
  // }
  // testIO(SD);
  // testRawIO();
}

void loop()
{
}
