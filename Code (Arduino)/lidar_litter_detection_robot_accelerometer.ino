         /////////////////////////////////////////////  
        //      LIDAR Litter Detection Robot       //
       //             w/ Edge Impulse             //
      //             ---------------             //
     //              (Arduino Nano)             //           
    //             by Kutluhan Aktar           // 
   //                                         //
  /////////////////////////////////////////////

//
// Recognize and monitor litter with object detection via this self-driving robot. It also deploys a video stream and a fall detection system.
//
// For more information:
// https://www.theamplituhedron.com/projects/Autonomous_(LIDAR)_Litter_Detection_Robot_w_Edge_Impulse/
//
//
// Connections
// Arduino Nano :  
//                                DFRobot Serial 6-Axis Accelerometer
// D11 --------------------------- TX 
// D12 --------------------------- RX
//                                SSD1306 OLED 128x32
// A4  --------------------------- SDA
// A5  --------------------------- SCL    
//                                Buzzer
// D10 --------------------------- S

// Include the required libraries.
#include <DFRobot_WT61PC.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Initiate the software serial port. RX：11 TX：12
SoftwareSerial mySerial(11, 12);

// Define the Serial 6-Axis Accelerometer.
DFRobot_WT61PC accelerometer(&mySerial);

// Define the SSD1306 screen settings:
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define monochrome graphics:
static const unsigned char PROGMEM _speed [] = {
0x00, 0x7E, 0x00, 0x01, 0xFF, 0x80, 0x07, 0xFF, 0xE0, 0x0F, 0x99, 0xF0, 0x1E, 0x18, 0x78, 0x3C,
0x18, 0x3C, 0x3E, 0x00, 0x7C, 0x76, 0x00, 0xEE, 0x72, 0x00, 0x0E, 0x60, 0x01, 0x06, 0xE0, 0x07,
0x07, 0xE0, 0x1E, 0x07, 0xE0, 0x3E, 0x07, 0xF8, 0x26, 0x1F, 0xF8, 0x3C, 0x1F, 0xC0, 0x18, 0x03,
0xE0, 0x00, 0x07, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x73, 0x00, 0xCE, 0x3E, 0x00, 0x7C, 0x3C,
0x00, 0x7C, 0x1C, 0x00, 0x38, 0x08, 0x00, 0x10, 
};

// Define the buzzer pin.
#define buzzer 10

void setup(){
  mySerial.begin(9600);
  
  // Revise the data output frequency of the accelerometer: FREQUENCY_0_1HZ for 0.1Hz, FREQUENCY_0_5HZ for 0.5Hz, FREQUENCY_1HZ for 1Hz, FREQUENCY_2HZ for 2Hz, FREQUENCY_5HZ for 5Hz, FREQUENCY_10HZ for 10Hz, FREQUENCY_20HZ for 20Hz, FREQUENCY_50HZ for 50Hz, FREQUENCY_100HZ for 100Hz, FREQUENCY_125HZ for 125Hz, FREQUENCY_200HZ for 200Hz.
  accelerometer.modifyFrequency(FREQUENCY_10HZ);

  // Initialize the SSD1306 screen:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
}


void loop(){
  if(accelerometer.available()){
    // Print the acceleration information of the X, Y, and Z-axis.
    display.clearDisplay();
    display.drawBitmap(100, 6, _speed, 24, 24, SSD1306_WHITE);                                  
    display.setTextSize(1);
    display.setCursor(0,0);                              
    display.setTextColor(SSD1306_WHITE);      
    display.print("X: ");
    display.print(accelerometer.Acc.X);
    display.print(" Y: ");
    display.println(accelerometer.Acc.Y);
    display.print("\nZ: ");
    display.print(accelerometer.Acc.Z);
    display.display();

    // Activate the buzzer depending on the angle information of the X, Y, Z-axis.
    if(accelerometer.Angle.X < -50 || accelerometer.Angle.Y < -50 /*|| accelerometer.Angle.Z < -160*/){
      tone(buzzer, 500);
      delay(250);
    }else{
      noTone(buzzer);
    }
  }
}
