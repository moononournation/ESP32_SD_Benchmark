/*
 * Connect the SD card to the following pins:
 *
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
 */
#include "FS.h"
#include "SD.h"
#include "SD_MMC.h"

#define TEST_FILE_SIZE (4 * 1024 * 1024)

void testWriteFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  unsigned long start_time = millis();
  Serial.printf("Test write %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  int loop = TEST_FILE_SIZE / len;
  while (loop--)
  {
    if (!file.write(buf, len)) {
      Serial.println("Write failed");
      return;
    }
  }
  file.flush();
  file.close();
  unsigned long time_used = millis() - start_time;
  Serial.printf("Write file used: %d ms, %f KB/s\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testReadFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  unsigned long start_time = millis();
  Serial.printf("Test read %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  int loop = TEST_FILE_SIZE / len;
  while (loop--)
  {
    if (!file.read(buf, len)) {
      Serial.println("Read failed");
      return;
    }
  }
  file.close();
  unsigned long time_used = millis() - start_time;
  Serial.printf("Read file used: %d ms, %f KB/s\n", time_used, (float)TEST_FILE_SIZE / time_used);
}

void testIO(fs::FS &fs)
{
  /* malloc will not reset all bytes to zero, so it is a random data */
  uint8_t *buf = (uint8_t*)malloc(64 * 1024);

  testWriteFile(fs, "/test_1k.bin", buf, 1024);
  testWriteFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testWriteFile(fs, "/test_4k.bin", buf, 4 * 1024);
  testWriteFile(fs, "/test_8k.bin", buf, 8 * 1024);
  testWriteFile(fs, "/test_16k.bin", buf, 16 * 1024);
  testWriteFile(fs, "/test_32k.bin", buf, 32 * 1024);
  testWriteFile(fs, "/test_64k.bin", buf, 64 * 1024);

  testReadFile(fs, "/test_1k.bin", buf, 1024);
  testReadFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testReadFile(fs, "/test_4k.bin", buf, 4 * 1024);
  testReadFile(fs, "/test_8k.bin", buf, 8 * 1024);
  testReadFile(fs, "/test_16k.bin", buf, 16 * 1024);
  testReadFile(fs, "/test_32k.bin", buf, 32 * 1024);
  testReadFile(fs, "/test_64k.bin", buf, 64 * 1024);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  /* test SD_MMC 4-bit Mode */
//  if (!SD_MMC.begin()) {
//    Serial.println("Card Mount Failed");
//    return;
//  }
//  testIO(SD_MMC);

  /* test SD_MMC 1-bit Mode */
  if (!SD_MMC.begin("/cdcard", true)) {
    Serial.println("Card Mount Failed");
    return;
  }
  testIO(SD_MMC);

  /* test SD SPI Mode at HSPI */
//  SPIClass spi = SPIClass(HSPI);
//  spi.begin(14 /* SCK */, 2 /* MISO */, 15 /* MOSI */, 13 /* SS */);
//  if (!SD.begin(13 /* SS */, spi, 80000000)) {
//    Serial.println("Card Mount Failed");
//    return;
//  }
//  testIO(SD);

  /* test SD SPI Mode at VSPI */
//  SPIClass spi = SPIClass(VSPI);
//  spi.begin(18 /* SCK */, 19 /* MISO */, 23 /* MOSI */, 5 /* SS */);
//  if (!SD.begin(5 /* SS */, spi, 80000000)) {
//    Serial.println("Card Mount Failed");
//    return;
//  }
//  testIO(SD);
}

void loop() {
}
