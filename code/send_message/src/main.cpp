#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "HX711.h"

// WiFi name & password 
const char* ssid = "iPhone";
const char* password =  "1234qwer";
//const char* ssid = "VM3400696";
//const char* password =  "tmypHvztf3sb";

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 32;
const int LOADCELL_SCK_PIN = 33;
HX711 scale;

//motor
int freq = 50;      // 频率(20ms周期)
int channel = 8;    // 通道(高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。)
int resolution = 8; // 分辨率
const int led = 14;

int calculatePWM(int degree)
{ //0-180度
 //20ms周期，高电平0.5-2.5ms，对应0-180度角度
  const float deadZone = 6.4;//对应0.5ms（0.5ms/(20ms/256）)
  const float max = 32;//对应2.5ms
  if (degree < 0)
    degree = 0;
  if (degree > 180)
    degree = 180;
  return (int)(((max - deadZone) / 180) * degree + deadZone);
}

void openlid(int second){
      //closed
      int d = 190;
      ledcWrite(channel, calculatePWM(d)); // 输出PWM
      Serial.printf("openning value=%d,calcu=%d\n", d, calculatePWM(d));
      delay(1000);
      //open
      d = 90;
      ledcWrite(channel, calculatePWM(d)); // 输出PWM
      Serial.printf("opened value=%d,calcu=%d\n", d, calculatePWM(d));
      delay(second);
      Serial.printf("delay=%d\n", second);
      //close again
      d = 190;
      ledcWrite(channel, calculatePWM(d)); // 输出PWM
      Serial.printf("closing value=%d,calcu=%d\n", d, calculatePWM(d));
}

int weight = 4;


void setup() {
 
  Serial.begin(9600);
  ledcSetup(channel, freq, resolution); // 设置通道
  ledcAttachPin(led, channel);          // 将通道与对应的引脚连接

  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
						// by the SCALE parameter (not set yet)

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
						// by the SCALE parameter set with set_scale

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");  

}

void sendWeight(){
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
}
 
void loop() {

  Serial.print("one reading:\t");
  Serial.print(scale.get_units()*3, 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10)*3, 1);
 // int weight =scale.get_units(10)*3
  if(scale.get_units(10)*3 > 80){
  //if(true){
    int ss = 22;
    Serial.println("overweight"+ ss);
    openlid(4000);
    //sendWeight();
  }

 
  scale.power_down();			        // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
 
}