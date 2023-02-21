/*#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}*/

/*
/*#include <WiFi.h>

const char* ssid     = "iPhone"; // 此处修改为你的WIFI名称（SSID）
const char* password = "1234qwer"; // 此处修改为你的WIFI密码

void setup()
{

    Serial.begin(115200);

    delay(10);

    // 开始连接WIFI

    Serial.println();
    Serial.println();
    Serial.print("WIFI Mode: ");
    Serial.println(WiFi.getMode());  // 显示当前WIFI的模式
    Serial.print("Connecting to ");
    Serial.println(ssid);
     Serial.print("Hellow world");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) { // 等待连接WIFI 直到连接成功 退出循环
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected."); //  WIFI 已经连接
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  // 显示连接WIFI后的IP地址
    Serial.println(WiFi.RSSI());  // 显示连接WIFI后的IP地址
    Serial.print("WIFI Mode: ");
    Serial.println(WiFi.getMode());  // // 显示当前WIFI的模式
}

void loop(){
}*/
/*
#include <Arduino.h>
void setup() {
  sleep(1);                                         //暂停运行1秒
  Serial.begin(9600);                               //初始化串口通信
  Serial.println("Hello world from setup function");  //向串口发送一段字符串,提示该字符串来自setup这个函数
}

void loop() {
  sleep(1);                                         //暂停运行1秒
  Serial.println("Hello world from loop function");   //向串口发送一段字符串,提示该字符串来自loop这个函数
}*/


#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

 
const char* ssid = "iPhone";
const char* password =  "1234qwer";
//const char* ssid = "VM3400696";
//const char* password =  "tmypHvztf3sb";
 
void setup() {
 
  //Serial.begin(115200);
  Serial.begin(9600);
  delay(400);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("https://ec2-34-247-173-84.eu-west-1.compute.amazonaws.com/getDetails?id=15"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
  
    

    http.begin("https://ec2-34-247-173-84.eu-west-1.compute.amazonaws.com/addWeight");
    http.addHeader("Content-Type", "multipart/form-data; boundary=wL36Yn8afVp8Ag7AmP8qZ0SA4n1v9T");

    // Add the form data as payload
    String payload = "b'--wL36Yn8afVp8Ag7AmP8qZ0SA4n1v9T\r\nContent-Disposition: form-data; name=id;\r\nContent-Type: text/plain\r\n\r\n1\r\n--wL36Yn8afVp8Ag7AmP8qZ0SA4n1v9T\r\nContent-Disposition: form-data; name=weight;\r\nContent-Type: text/plain\r\n\r\n4.88\r\n--wL36Yn8afVp8Ag7AmP8qZ0SA4n1v9T\r\nContent-Disposition: form-data; name=date;\r\nContent-Type: text/plain\r\n\r\n2023-2-9\r\n--wL36Yn8afVp8Ag7AmP8qZ0SA4n1v9T--\r\n'";

    httpCode = http.POST(payload);                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }
 
  delay(5000);
 
}