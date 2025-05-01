#include <Adafruit_NeoPixel.h>
#include <DHT20.h>
#include <LiquidCrystal_I2C.h>;
#include <WiFi.h>               // For connecting to Wi-Fi
#include <AdafruitIO_WiFi.h>     // For Adafruit IO
#include <ESP32Servo.h>

/******************************* WIFI **************************************/
#define WIFI_SSID ""    
#define WIFI_PASS ""

/******************************* Adafruit IO *******************************/
#define IO_USERNAME ""
#define IO_KEY ""
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Declare feeds
AdafruitIO_Feed *NameFeed = io.feed("project-242.name");
AdafruitIO_Feed *DoorFeed = io.feed("project-242.door");
AdafruitIO_Feed *ButtonFeed = io.feed("project-242.on-off");
AdafruitIO_Feed *LedFeed = io.feed("project-242.light");
// AdafruitIO_Feed *temperatureFeed = io.feed("project-242.temperature");

LiquidCrystal_I2C lcd(0x21,16,2);
Adafruit_NeoPixel pixels5(4, P0_ADC, NEO_GRB + NEO_KHZ800);

Servo myservo;

bool ButtonFlag = false;
bool last_button = false;
String  Name = "Test";
int ButtonPin = BUTTON_A;
int buttonState = 0;
int buttonLastState = 0;
int state = 0;
bool LedState = false;

void TaskIdentity(void *pvParameters);
void TaskServor(void *pvParameters);
void TaskButton(void *pvParameters);
void TaskLed(void *pvParameters);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin();
  pixels5.begin();

  myservo.setPeriodHertz(100);    // standard 50 hz servo
	myservo.attach(P4, 500, 2400); // attaches the servo on pin P4 to the servo object

  // Connect to Adafruit IO
  Serial.print("Connecting to Adafruit IO...");
  io.connect();
  
  NameFeed->onMessage(handleMessageName);
  DoorFeed->onMessage(handleMessageButton);
  LedFeed->onMessage(handleMessageLed);

  // Wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to Adafruit IO");
  
  xTaskCreate(TaskIdentity, "Task Identity",2048, NULL, 2, NULL);
  xTaskCreate(TaskServor, "Task Servor",2048, NULL, 2, NULL);
  xTaskCreate(TaskButton, "Task button",2048, NULL, 2, NULL); 
  xTaskCreate(TaskLed, "Task led",2048, NULL, 2, NULL);


  Serial.printf("Basic Multi Threading Arduino Example\n");
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {
  io.run();
}

void handleMessageButton(AdafruitIO_Data *data) {
  Serial.print("Feed DoorFeed received: ");
  Serial.println(data->value());

  if (strcmp(data->value(), "OFF") == 0) {  
    ButtonFlag = false;
  } else {  
    ButtonFlag = true;
  }
  delay(1000);
}

 void handleMessageName(AdafruitIO_Data *data) {

  Serial.print("Feed name received: ");
  Serial.println(data->value());

  Name = data->value();

  delay(2000);
 }

 void handleMessageLed(AdafruitIO_Data *data) {

  Serial.print("Feed led received: ");
  Serial.println(data->value());

  if (strcmp(data->value(), "OFF") == 0) {  
    LedState = false;
  } else {  
    LedState = true;
  }
  delay(1000);
 }


void TaskIdentity(void *pvParameters){
  while(1){
    if (Name == "Unknown Person"){
      
        // ButtonFlag = false;
        // ButtonFeed->save("OFF"); 
        // Serial.printf("Button is OFF");       


        Serial.println("this picture is unknow");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error: Cannot");
        lcd.setCursor(0, 1);
        lcd.print("Detect face");
    }else if (Name != "Test"){

        // ButtonFlag = true;
        // ButtonFeed->save("ON");
        // Serial.printf("Button is ON");       




        Serial.println("this picture is someone");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome Home");
        lcd.setCursor(0, 1);
        lcd.print(Name);
    }
    
    delay(2000);
  }
}

void TaskLed(void *pvParameters){
  while(1){
    if (LedState == false){
        pixels5.setPixelColor(0, pixels5.Color(0,0,0));
        pixels5.setPixelColor(1, pixels5.Color(0,0,0));
        pixels5.setPixelColor(2, pixels5.Color(0,0,0));
        pixels5.setPixelColor(3, pixels5.Color(0,0,0));
        pixels5.show();

    }else if (LedState == true){
        pixels5.setPixelColor(0, pixels5.Color(255,255,255));
        pixels5.setPixelColor(1, pixels5.Color(255,255,255));
        pixels5.setPixelColor(2, pixels5.Color(255,255,255));
        pixels5.setPixelColor(3, pixels5.Color(255,255,255));
        pixels5.show();
    }
  }
}


void TaskServor(void *pvParameters){
  while(1){

    if (ButtonFlag != last_button) {  // Detect state change
      if (ButtonFlag) {  // If button is ON
        for (int pos = 0; pos <= 120; pos++) {  
          myservo.write(pos);  
          delay(5);
        }

      } else {  // If button is OFF
        for (int pos = 120; pos >= 0; pos--) {  
          myservo.write(pos);  
          delay(5);
        }
      }
      last_button = ButtonFlag; // Update last state
    }
    myservo.release();
    delay(1000);

  }
}
void TaskButton(void *pvParameters){
  
  pinMode(ButtonPin, INPUT);

  while (1){
    buttonState = digitalRead(ButtonPin);

    if (buttonState == HIGH && buttonLastState == LOW){
      if (state == 0){
        ButtonFeed->save("OFF");
        state = 1;
      }else if (state == 1){
        ButtonFeed->save("ON");
        state = 0;
      }
    }

    buttonLastState = buttonState;
    delay(1000);
  }
}