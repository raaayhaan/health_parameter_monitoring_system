#include "DHT.h"
#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <SoftwareSerial.h>
#define REPORTING_PERIOD_MS     1000
#define DHTTYPE DHT11
#define DHTPIN 3     // Digital pin connected to the DHT sensor
SoftwareSerial espSerial(5, 6);
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
String str;
// String str1;


const int lm35_pin = A0;

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  //Serial.begin(9600);
  Serial.begin(115200);
  espSerial.begin(115200);
  Serial.print("Initializing pulse oximeter..");
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(3000);
  lcd.clear();
  Serial.println(F("DHTxx test!"));

  dht.begin();

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
     Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
  
  // Make sure to call update as fast as possible
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
  {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BPM : ");
    lcd.print(pox.getHeartRate());

    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(pox.getSpO2());
    lcd.print("%");
  }

 for(int i = 0; i < 500 ; i++)
 {
  pox.update();
  delay(1); 
 }   
  

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F(" Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("C "));
    Serial.print(f);
    Serial.print(F("F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("C "));
    Serial.print(hif);
    Serial.println(F("F"));
    str =String("&feild1="+String(h)+"&feild2="+String(t)+"&feild3="+String(pox.getHeartRate())+"&feild4="+String(pox.getSpO2()));
   // str= String(String("H= ")+String(h)+" "+String("T= ")+String(t)+" "+String("SpO2:")+String(pox.getSpO2())+" "+String("BPM: ")+String(pox.getHeartRate()));
    //str="&field1="+String(t)+"&field2="+String(h)+"&field3="+String(pox.getSpO2())+"&field4="+String(pox.getHeartRate()); 
    espSerial.println(str);
     //espSerial.println("&field1="+String(t)+"&field2="+String(h)+"&field3="+String(pox.getSpO2())+"&field4="+String(pox.getHeartRate()));
    //str="&field1="+String(t)+"&field2="+String(h)+"&field3="+String(pox.getSpO2())+"&field4="+String(pox.getHeartRate()); 

    lcd.clear();
    lcd.print("Humidity");
    lcd.print(h);
    lcd.print("%Temperature");
    lcd.print(t);
    tsLastReport = millis();
 
}
  // {
  //   int temp_adc_val;
  //   float temp_val;
  //   temp_adc_val = analogRead(lm35_pin);  /* Read Temperature */
  //   temp_val = (temp_adc_val * 4.88); /* Convert adc value to equivalent voltage */
  //   temp_val = (temp_val / 10); /* LM35 gives output of 10mv/°C */
  //   Serial.print("Temperature = ");
  //   Serial.print(temp_val);
  //   Serial.print(" Degree Celsius\n");
  //   delay(1000);
  // }

