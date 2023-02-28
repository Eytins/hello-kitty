#include <WiFi.h>

const char* ssid = "iPhone"; //wifi nane
const char* password = "1234qwer"; // wifi password

//-----------网络时间获取-----------//
const char *ntpServer = "pool.ntp.org"; //网络时间服务器
//时区设置函数，东八区（UTC/GMT+8:00）写成8*3600
const long gmtOffset_sec = 0;//8 * 3600;    
const int daylightOffset_sec = 0;   //夏令时填写3600，否则填0

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%F %T %A"); // 格式化输出
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected!");

    // 从网络时间服务器上获取并设置时间
    // 获取成功后芯片会使用RTC时钟保持时间的更新
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();

    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);
    // Serial.println("WiFi disconnected!");
}

void loop()
{
    delay(1000);
    printLocalTime();
}
