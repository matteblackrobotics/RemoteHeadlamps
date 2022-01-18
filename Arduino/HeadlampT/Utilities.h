#ifndef Utilities_h
#define Utilities_h

// serialport: /dev/cu.usbserial-14320

String fileName = "------------------ Headlamp ------------------";
String author = " Matt Pfeiffer   Jan, 2022 ";
const int baud1 = 9600;
const int baud2 = 19200;
const long baud3 = 115200;

int delay1 = 0;

void utilitiesSetup()
{
  Serial.begin(baud3); delay(10);
  Serial.println(" "); Serial.println(" ");
  Serial.println(fileName);
  Serial.println(author);
  Serial.println(" ");
}


#endif
