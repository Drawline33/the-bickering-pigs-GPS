// The original script for the arduino 2560 mega can be found at https://www.instructables.com/Interface-Arduino-Mega-With-GPS-Module-Neo-6M/ 
//

//Connect GPS with pin tx14 and rx15

//3.5inch tft screen drivers
#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
//gps drivers and decoder
#include <TinyGPS++.h>

//sd card includes
#include <SPI.h>             // f.k. for Arduino-1.5.2
#define USE_SDFAT
#include <SdFat.h>           // Use the SdFat library

#if SPI_DRIVER_SELECT != 2
#error edit SdFatConfig.h .  READ THE SKETCH INSTRUCTIONS
#endif

SoftSpiDriver<12, 11, 13> softSpi; //Bit-Bang on the Shield pins SDFat.h v2
SdFat SD;
#define SD_CS SdSpiConfig(10, DEDICATED_SPI, SD_SCK_MHZ(50), &softSpi)

MCUFRIEND_kbv tft; // create screen object
TinyGPSPlus gps; // create gps object
File myFile;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
String Latitude, Longitude, Altitude, day, month, year, hour, minute, second, Date, Time, Data;
int16_t ht = 16, top = 3, line, lines = 1, scroll;

void setup(){
  Serial.begin(57600);
  Serial.println("The GPS Received Signal:");
  while (!Serial) {
    ; 
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  Serial.println("Creating GPS_data.txt...");
  myFile = SD.open("GPS_data.txt", FILE_WRITE);
  if (myFile) {
    myFile.println( "Latitude, Longitude, Altitude, Date and Time \r\n");
    myFile.close();

  } else {
    Serial.println("error opening GPS_data.txt");
  }
  Serial3.begin(9600); // connect gps sensor
  tft.reset();
  uint16_t id = tft.readID();
  tft.begin(id);// connect serial
  tft.setRotation(45);     //Portrait
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
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
     obtain_data();
    //Latitude
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(),6);
     //Longitude
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(),6); 
    //Altitude
    Serial.print("Altitude: "); 
    Serial.println(gps.altitude.feet());
    //Speed
    Serial.print("Speed: "); 
    Serial.println(gps.speed.mph());
    // Number of satellites connected
    Serial.print("Number of Sats connected: "); 
    Serial.println(gps.satellites.value());  
      
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

void obtain_data()
{
  if (gps.location.isValid())
  {
    Latitude = gps.location.lat();
    Longitude = gps.location.lng();
    Altitude = gps.altitude.meters();
  }
  else
  {
    Serial.println("Location is not available");
  }
  
  if (gps.date.isValid())
  {
    month = gps.date.month();
    day = gps.date.day();
    year = gps.date.year();
    Date = day + "-" + month + "-" + year;
     Serial.println(Date);
  }
  else
  {
    Serial.print("Date Not Available");
  }
 
  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) ;
    hour = gps.time.hour();
    if (gps.time.minute() < 10);
    minute = gps.time.minute();
    if (gps.time.second() < 10) ;
    second = gps.time.second();
    Time = hour + ":" + minute + ":" + second;
  Serial.println(Time);
  }
  else
  {
    Serial.print("Time Not Available");
  }
  
  Data = Latitude + "," + Longitude + "," + Altitude + "," + Date + "," + Time;
  Serial.print("Save data: ");
  Serial.println(Data);
  myFile = SD.open("GPS_data.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("GPS logging to GPS_data.txt...");
    myFile.println(Data);
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening GPS_data.txt");
  } 
 Serial.println();
 delay(10000);
}
