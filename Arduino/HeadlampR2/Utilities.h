#ifndef Utilities_h
#define Utilities_h

String FileName = "HeadlampR2";
const int baud1 = 9600;
const long baud3 = 115200;

void utilitiesSetup()
{
    Serial.begin(baud3); delay(200);
    Serial.println(" "); 
    Serial.println(FileName); 
    Serial.println(" "); 
}
#endif
