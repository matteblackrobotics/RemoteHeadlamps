#ifndef utilities_H_
#define utilities_H_

String fileName = " ------------------ Headlamp11 ------------------";
String author = " Matt Pfeiffer   Jan, 2022 ";
const int baud_rate1 = 9600;
const int baud_rate2 = 19200;
const int baud_rate3 = 115200;

void utilities_setup()
{
  Serial.begin(baud_rate1); delay(10);
  Serial.println(" "); Serial.println(" ");
  Serial.println(fileName);
  Serial.println(author);
  Serial.println(" ");
}

#endif
