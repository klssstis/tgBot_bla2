#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

void wifiGetUP(){
  Serial.println("wifiGetUP");
  WiFi.mode(WIFI_STA);//
  WiFi.begin("XXXXXXXX", "XXXXXXXXXX");
  int waitWF = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    waitWF += 1;
    if(waitWF > 50)
      break;
    Serial.print(".");
  }  
}
void wifiGetDOWN()
{
    Serial.println("wifiGetDOWN");
    WiFi.disconnect();
}
void TelegramPrint(String message) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  if (https.begin(*client, "https://api.telegram.org/botXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXX/")) {
    https.addHeader("Content-Type", "application/json");
    //741655528
    https.POST("{\"method\":\"sendMessage\",\"chat_id\":1037182071,\"text\":\"" + message + "\"}");
    https.end();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  delay(2000);
  wifiGetUP();      
  TelegramPrint("start setup()");
  delay(2000);
  wifiGetDOWN(); 
  delay(2000);  
  if (!bmp.begin()) {
    Serial.println("Failed to find BMP180 chip");
    delay(2000);
    wifiGetUP();      
    TelegramPrint("Failed to find BMP180 chip");
    delay(2000);
    wifiGetDOWN(); 
    delay(2000);      
    while (1) {
      delay(10000);
    }
  }
  Serial.println("BMP180 Found!");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    delay(2000);
    wifiGetUP();      
    TelegramPrint("Failed to find MPU6050 chip");
    delay(2000);
    wifiGetDOWN(); 
    delay(2000);     
    while (1) {
      delay(10000);
    }
  }
  Serial.println("MPU6050 Found!");


mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
  
}

void loop()
{


    delay(10000);
    wifiGetUP();
    delay(5000);
    
    double P, Alt;
    P = bmp.readPressure();
    TelegramPrint(String("Температура (*C): ")+String(bmp.readTemperature()));
    TelegramPrint(String("Давление (мм рт. ст.): ")+String(P * 0.00750063755419211));
    TelegramPrint(String("Давление (ПА): ")+String(P));    
    TelegramPrint(String("Высота над уровнем моря (м): ")+String(bmp.readAltitude()));        
    delay(5000);

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  
    TelegramPrint(String("Acceleration X: ")+String(a.acceleration.x)+String(", Y: ")+String(a.acceleration.y)+String(", Z: ")+String(a.acceleration.z)+String(" m/s^2"));
    TelegramPrint(String("Rotation X: ")+String(g.gyro.x)+String(", Y: ")+String(g.gyro.y)+String(", Z: ")+String(g.gyro.z)+String(" rad/s"));
    TelegramPrint(String("Temperature: ")+String(temp.temperature)+String(" degC"));

  
    delay(5000);
    
    wifiGetDOWN();  
    delay(5000);
}
