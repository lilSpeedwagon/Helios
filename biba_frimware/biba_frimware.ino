#include <SoftwareSerial.h>

const char RELAY0 = 4;
const char RELAY1 = 5;
const char RELAY2 = 6;
const char RELAY3 = 7;
//13 pin это диод на плате

const char* HSERIAL = "H0002";
const char* ssid = "Quartal1";
const char* pass = "SSQ2018sf";
//const char* ssid = "AndroidAP";
//const char* pass = "ldih3432";
const char* port = "8888";

String OK = "OK";
String ARROW = "> ";
String DONE = "DONE";
SoftwareSerial mySerial(2, 3);

String ret(String str) {
  return str + (char)13 + (char)10;
}

void sendESP(char id, String msg) {
  String message = "AT+CIPSEND=";
  mySerial.println(message + String((int)id) + ',' + String(msg.length()));
  while (!readStrESP().equals(ARROW));
  mySerial.println(msg);
}

String readStrCOM() {
  String serialString = "";
  while (Serial.available()) {
    serialString += (char)(Serial.read());
    if (serialString[serialString.length() - 1] == '\n')
      break;
    delay(1);
  }
  return serialString;
}

String readStrESP() {
  String serialString = "";
  while (mySerial.available()) {
    serialString += (char)(mySerial.read());
    if (serialString[serialString.length() - 1] == '\n')
      break;
    delay(1);
  }
  if (!serialString.equals("")) {
    Serial.print(serialString);
  }
  return serialString;
}


String readCOM() {
  String serialString = "";
  while (Serial.available()) {
    serialString += readStrCOM();
    delay(1);
  }
  return serialString;
}

String readESP() {
  String serialString = "";
  while (mySerial.available()) {
    serialString += readStrESP();
    delay(1);
  }
  return serialString;
}

void setup() {
  OK = ret(OK);
  pinMode(RELAY0, OUTPUT); //инициализация пинов реле
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  digitalWrite(RELAY0, LOW);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  delay(1000);
  //Запуск последовательных портов
  mySerial.begin(115200);
  Serial.begin(9600);

  mySerial.println("AT+UART_DEF=9600,8,1,0,0");
  readESP();

  mySerial.end();
  mySerial.begin(9600);
  
  Serial.println("Serial port initialized");
  delay(1000);
  readESP();

  mySerial.println("AT+RST");               //Перезагрузка модуля WiFi
  delay(1000);
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CWAUTOCONN=0");      //Автоматическое подключение выключено
  while (true){
    String inCome = readStrESP();
    if(inCome.equals(OK)||inCome.equals("ERROR"));   
    break;
    }

  mySerial.println("AT+CWMODE_DEF=3");      //Режим станции и клиента
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CIPMUX=1");          //Multiconnection mode
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CWJAP_DEF=\"" + (String)ssid + "\",\"" + (String)pass + "\""); //Connecting to wifi
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CWSAP_DEF=\"" + (String)HSERIAL + "\",\"SAN987873\",5,3"); //Starting AP
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CWDHCP_DEF=2,1");             //Запуск DHCP
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CIPSERVER=1," + (String)port); //Запуск сервера на порту 8888
  while (!readStrESP().equals(OK));

  mySerial.println("AT+CIPSTA_CUR?"); //Выводит в консоль айпи, шлюз и маску
  while (!readStrESP().equals(OK));
}

void loop() {

  if (Serial.available())
    mySerial.println(readStrCOM());

  if (mySerial.available()) {
    String inStr = readStrESP();

    if (inStr.startsWith("+IPD,")) {
      String buff = "";
      int i = 0;
      for (; 1; i++) {
        if (inStr[i + 5] != ',')
          buff += inStr[i + 5];
        else break;
      }
      int inAdress = buff.toInt();
      Serial.print("id:");
      Serial.println(inAdress);
      buff = "";
      i++;
      for (; 1; i++) {
        if (inStr[i + 5] != ':')
          buff += inStr[i + 5];
        else break;
      }
      int msgSize = buff.toInt();
      Serial.print("size of message:");
      Serial.println(msgSize);
      buff = "";
      i++;
      int checkSize = inStr[i + 5];
      Serial.print("size of check:");
      Serial.println(checkSize);
      i++;
      for (; 1; i++) {
        if ((buff.length() < checkSize) && (inStr[i + 5] != 10) && (inStr[i + 5] != 13) && (i + 5 <= inStr.length() - 1)) {
          buff += inStr[i + 5];
          //Serial.println(i+5);
        }
        else break;
      }
      Serial.print("message:" );
      Serial.println(buff);

      if (buff.equals("OFF")) {
        digitalWrite(RELAY0, HIGH);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, HIGH);
        delay(5);
        digitalWrite(RELAY0, LOW);
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, LOW);
        digitalWrite(RELAY3, LOW);
        delay(5);
        digitalWrite(RELAY0, HIGH);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, HIGH);
        delay(5);
        digitalWrite(RELAY0, LOW);
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, LOW);
        digitalWrite(RELAY3, LOW);
        delay(5);
        digitalWrite(RELAY0, HIGH);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, HIGH);
        delay(5);
        sendESP(inAdress, DONE);
      }
      else if (buff.equals("ON")) {
        digitalWrite(RELAY0, LOW);
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, LOW);
        digitalWrite(RELAY3, LOW);
        delay(5);
        digitalWrite(RELAY0, HIGH);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, HIGH);
        delay(5);
        digitalWrite(RELAY0, LOW);
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, LOW);
        digitalWrite(RELAY3, LOW);
        delay(5);
        digitalWrite(RELAY0, HIGH);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, HIGH);
        delay(5);
        digitalWrite(RELAY0, LOW);
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, LOW);
        digitalWrite(RELAY3, LOW);
        
        sendESP(inAdress, DONE);
      }
      else if (buff.equals("OFF0")) {
        digitalWrite(RELAY0, HIGH);
      }
      else if (buff.equals("OFF1")) {
        digitalWrite(RELAY1, HIGH);
      }
      else if (buff.equals("OFF2")) {
        digitalWrite(RELAY2, HIGH);
      }
      else if (buff.equals("OFF3")) {
        digitalWrite(RELAY3, HIGH);
      }
      else if (buff.equals("ON0")) {
        digitalWrite(RELAY0, LOW);
      }
      else if (buff.equals("ON1")) {
        digitalWrite(RELAY1, LOW);
      }
      else if (buff.equals("ON2")) {
        digitalWrite(RELAY2, LOW);
      }
      else if (buff.equals("ON3")) {
        digitalWrite(RELAY3, LOW);
      }
      else if (buff.equals("HI"))
        sendESP(inAdress, HSERIAL);
      else if (buff.equals("ASK"));
    }
  }
}
