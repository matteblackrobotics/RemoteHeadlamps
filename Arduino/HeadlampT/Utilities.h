#ifndef Utilities_h
#define Utilities_h


String fileName = " ------------------ Headlamp ------------------";
String author = " Matt Pfeiffer   Jan, 2022 ";
const int baud_rate1 = 9600;
const int baud_rate2 = 19200;
const int baud_rate3 = 115200;

int timer1;

void utilities_setup()
{
  Serial.begin(baud_rate1); delay(10);
  Serial.println(" "); Serial.println(" ");
  Serial.println(fileName);
  Serial.println(author);
  Serial.println(" ");
}


#endif
