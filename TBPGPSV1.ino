//Connect with pin 14 and 15
#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <TinyGPS++.h>
MCUFRIEND_kbv tft;
TinyGPSPlus gps; // create gps object
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int16_t ht = 16, top = 3, line, lines = 1, scroll;

void setup(){
  Serial.begin(57600);
  tft.reset();
  uint16_t id = tft.readID();
  tft.begin(id);// connect serial
  tft.setRotation(45);     //Portrait
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  Serial.println("The GPS Received Signal:");
  Serial3.begin(9600); // connect gps sensor
  tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
  tft.setCursor(60, 0);
  tft.print("The Bickering Pigs GPS V01.0.1");
 
  tft.setCursor(30, 0);
  tft.setCursor(0,50);
  tft.println("LAT: ");
  tft.setCursor(0,70);
  tft.println("LON: ");
  tft.setCursor(0,90);
  tft.println("Altitude: ");
  tft.setCursor(0,110);
  tft.println("SPEED: ");
  tft.setCursor(0,130);
  tft.println("SAT: ");
 
}
 
void loop(){
    while(Serial3.available()){ // check for gps data
    if(gps.encode(Serial3.read()))// encode gps data
    { 
    
    //Latitude
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(),6);
    
    //Longitude  
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(),6);
   
    //Alitude  
    Serial.print("Altitude: "); 
    Serial.println(gps.altitude.feet());
    
    //Speed (MPH)  
    Serial.print("Speed: "); 
    Serial.println(gps.speed.mph());
    
    // Number of satellites connected  
    Serial.print("Number of Sats connected: "); 
    Serial.println(gps.satellites.value());
    
    
      
      //Latitude
    tft.setCursor(110, (scroll + top) * ht);
    if (++scroll >= lines) scroll = 0;
    tft.vertScroll(top * ht, lines * ht, (scroll) * ht);
    tft.setTextColor(WHITE,BLACK);
    tft.setCursor(140,50);
    tft.println(gps.location.lat(),6);
    //Longitude
     
    tft.setCursor(140,70);
    tft.println(gps.location.lng(),6);
    
    //Altitude
    
    tft.setCursor(140,90);
    tft.println(gps.altitude.feet());
    
    //Speed
    tft.setCursor(140,110);
    tft.println(gps.speed.mph());

    // Number of satellites connected
    tft.setCursor(140,130);
    tft.println(gps.satellites.value());
    tft.setTextColor(WHITE,BLACK);
    delay(2000);
    line++;
   }
  }
}
