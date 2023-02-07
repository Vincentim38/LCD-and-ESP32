#define BLYNK_TEMPLATE_ID "TMPLtHHF3prF"
#define BLYNK_DEVICE_NAME "Template pour ESP 32 avec Wifi"
#define BLYNK_AUTH_TOKEN "TPO34_bKWxJGqjD0VOH8cr-1EYSQSiI2"
#define BLYNK_PRINT Serial

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)

#include <Arduino.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BH1750.h>
#include <Adafruit_BMP280.h>
#include <U8g2lib.h>

Adafruit_BMP280 bmp;
BH1750 lightMeter;
BlynkTimer timer;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Livebox-1932";
char pass[] = "tim12345";

void myTimerEvent()
{
    Blynk.virtualWrite(V2, millis() / 1000);

    int lux = lightMeter.readLightLevel();
    Serial.print("Luminosité : ");
    Serial.print(lux);
    Serial.println(" lx");
    Blynk.virtualWrite(V5, lux);

    float tempe = bmp.readTemperature();
    Serial.print(F("Température = "));
    Serial.print(tempe);
    Serial.println(" deg C");
    Blynk.virtualWrite(V6, tempe);

    float pression = bmp.readPressure();
    float pressionmer = pression * 1.0859 + 0.03;
    Serial.print(F("Pression à 690m = "));
    Serial.print(pression / 100);
    Serial.println(" hPa");
    Serial.print(F("Pression niveau mer = "));
    Serial.print(pressionmer / 100);
    Serial.println(" hPa");
    Blynk.virtualWrite(V7, pression / 100);
    Blynk.virtualWrite(V9, pressionmer / 100);

    float alti = bmp.readAltitude(1013);
    Serial.print(F("Altitude approx = "));
    Serial.print(alti);
    Serial.println(" m");
    Blynk.virtualWrite(V8, alti);

    Serial.println("------------------");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.clearBuffer();

    u8g2.drawStr(0, 12, "Luminosite:");
    u8g2.setCursor(70, 12);
    u8g2.print(lux);
    u8g2.drawStr(100, 12, " lux");

    u8g2.drawStr(0, 24, "Temperature:");
    u8g2.setCursor(76, 24);
    u8g2.print(tempe);
    u8g2.drawStr(105, 24, " C");

    u8g2.drawStr(0, 36, "Pression:");
    u8g2.setCursor(60, 36);
    u8g2.print(pression / 100);
    u8g2.drawStr(105, 36, " hPa");

    u8g2.drawStr(0, 48, "Pres Mer:");
    u8g2.setCursor(62, 48);
    u8g2.print(pressionmer / 100);
    u8g2.drawStr(105, 48, " hPa");

    u8g2.sendBuffer();
}

void myTimerEvent2()
{
    // Restart the ESP32 every 24h
    ESP.restart();
}

void setup()
{
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    // Setup a function to be called every 10 seconds :
    timer.setInterval(10000, myTimerEvent);
    // Setup the reset function to be called every 24 hours (86400000 milliseconds):
    timer.setInterval(86400000, myTimerEvent2);

    // Initialize the I2C bus (BH1750 library doesn't do this automatically) :
    Wire.begin();
    lightMeter.begin();
    bmp.begin();
    u8g2.begin();

    /* Default settings for BMP280 */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop()
{
    Blynk.run();
    timer.run();

    // Avoid delay() function!
}
