#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial gprsSerial(7, 8);

const int button2Pin = 3;  // кнопка №2 - порт 3

const int LED_PIN =  12;    // порт 12, для светодиода
const int LED_PIN2 = 11;
const int LED_PIN3 = 4;

char cmdMsg[50];
char cmdMsgd[100];

int addr=0; // адрес 
int addr2=30;
int addr1=50; // адрес 
bool led_stat    = true;
int flag1 = 0;
int flag = 0; // флаг состояния
int regim = 0; // Переключение режимов
const byte GerkonPin = 10; //геркон
char data;

boolean LedState = 0; //флаг желтого диода
boolean LedStateRed = 0; //флаг красного диода

boolean GerkonState = 0; //флаг геркона
//Переменные для таймера
unsigned long last_on;
unsigned long last_on1=0;
unsigned long last_on3=0;

//таймеры
unsigned long previousMillis = 0;// обнуляем таймер диода
unsigned long intervalLed = 300; //время мигания зелёного диода
unsigned long previousMillisRed = 0;// обнуляем таймер диода
unsigned long intervalLedRed = 300; //время мигания зелёного диода

//unsigned long previousMillis1 = 0;
int ledState;
int Loop =1;

unsigned long xxx=5000;


void setup()
{   
  Serial.begin(19200);    //Скорость порта для связи Arduino с компьютером (Инициализируем последовательный интерфейс)
  gprsSerial.begin(19200);  //Скорость порта для связи Arduino с GSM модулем  (устанавливаем скорость передачи данных для последовательного порта, созданного библиотекой SoftwareSerial) 
  // Установим порты кнопок как входящие:
  pinMode(button2Pin, INPUT);
  // Установим порт светодиода как исходящий:
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  //Шлейф
  pinMode(10, INPUT);
  pinMode(GerkonPin, INPUT);
  
  last_on=0;
  // Настраиваем приём сообщений с других устройств
  // Между командами даём время на их обработку
  gprsSerial.print("AT+CMGF=1\r");
  delay(300);
  gprsSerial.print("AT+IFC=1, 1\r");
  delay(300);
  gprsSerial.print("AT+CPBS=\"SM\"\r");
  delay(300);
  gprsSerial.print("AT+CNMI=1,2,2,1,0\r");
  delay(300);
}

String currStr = "";
// Переменная принимает значение True, если текущая строка является сообщением
boolean isStringMessage = false;

void loop()
{   
  GerkonState = digitalRead(GerkonPin); // назначаем переменную геркону
  int button2State;  // переменные для сохранения состояния кнопок
  button2State = digitalRead(button2Pin);
      
  if(button2State == LOW && flag == 0) 
  {   
  last_on = millis();   
  regim ++;
  flag = 1;

  if(regim > 1) // Если номер режима превышает требуемого
  { // то отсчет начинается с нуля 
  regim = 0;
  }
 }

 if(button2State == HIGH && flag == 1)
  {
  flag = 0;   
  }

//---------------------------------------------------------------------------------------------------------
char data = Serial.read();
  switch (data) {
  case '7': 
  { 
  int cmd = nextByte();
   
  if(cmd == 126)
   {
     char charIn = 0;
     byte i = 0;
     while (charIn != 126) // wait for header byte again (Дождитесь снова заголовка)
     {
       charIn = nextByte();
       //Serial.println (charIn);
       
       if(charIn != 126)
       {
         cmdMsg[i] = charIn;
         i += 1;
       // Serial.println (cmdMsg);
         //Serial.println (i);
       }
     }
     Serial.println("Phone number saved:");
     //Serial.println (cmdMsg);
     //Serial.println (cmdMsg); //Serial.println is important for C# (took me a while to figure that out)
       //that will shoot back whatever you sent in without the '~'s
     cmdMsg[i] = NULL;
   //Serial.println (i);
   }
   EEPROM.put(addr, cmdMsg);
    break;
  }
  case '8':
  {
  int cmdd = nextByted();
   
  if(cmdd == 126)
   {
     char charInd = 0;
     byte id = 0;
     while (charInd != 126) // wait for header byte again (Дождитесь снова заголовка)
     {
       charInd = nextByte();
       //Serial.println (charIn);
       
       if(charInd != 126)
       {
         cmdMsgd[id] = charInd;
         id += 1;
        //Serial.println (cmdMsg);
         //Serial.println (i);
       }
     }
     Serial.println("Text SMS savend:");
     Serial.println (cmdMsgd); //Serial.println is important for C# (took me a while to figure that out)
       //that will shoot back whatever you sent in without the '~'s
     cmdMsgd[id] = NULL;
   //Serial.println (i);
   }
   EEPROM.put(addr1, cmdMsgd);
   break; 
  }
  case '9':
  {
      last_on = millis();   
      regim ++;
      flag = 1;
      break; 
  }
  case '0':
  {
     regim = 0;
     break; 
  }
  case '6':
  {
     xxx=Serial.parseInt();
     EEPROM_ulong_write(addr2, xxx);
     Serial.println ("Time plat saved, mc");
     Serial.println (xxx);     
     break;
  }
   }
//---------------------------------------------------------------------------------------------------------
// for (int addr=0; addr<1024; addr++) { // для всех ячеек памяти (1024Б=1кБ)
//    Serial.print(addr); // выводим адрес в послед. порт 
//    Serial.print("\t"); // табуляция
//    EEPROM.get(addr, cmdMsg); // считываем массив символов по адресу addr
//    Serial.println(cmdMsg); // выводим массив в порт
//  }  
//---------------------------------------------------------------------------------------------------------  
//  char data = Serial.read();
//
//  switch (data) {
//    case '1':  
//    last_on = millis();   
//    regim ++;
//    flag = 1;
//    break;
//    case '2':
//    regim = 0;
//    break;
//    }
 //-------------------------------------------------------------------------- 
//if (gprsSerial.available())              // if date is comming from softwareserial port ==> data is comming from gprs shield
//  {
//    while(gprsSerial.available())          // reading data into char array 
//    {
//      buffer[count++]=gprsSerial.read();   // writing data into array
//      if(count == 64)break;
//  }
//    Serial.write(buffer,count);   // if no data transmission ends, write buffer to hardware serial port
//    clearBufferArray();           // call clearBufferArray function to clear the storaged data from the array
//    count = 0;                    // set counter of while loop to zero
//  }
//    if (Serial.available())         // if data is available on hardwareserial port ==> data is comming from PC or notebook
//    gprsSerial.write(Serial.read());    // write it to the GPRS shield
//Нулевой режим-----------------------------------------------------------------  
  if (regim == 0)  // сравниваем, нажаты ли кнопки                                                     
  {
    digitalWrite(LED_PIN, LOW);  // вывод состояния светодиода 1  
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
  }
//Первый режим----------------------------------------------------------------  
 if(regim == 1){
  data = Serial.read();
  if(millis()-last_on<=10){
  Serial.println("Start plant time");
  delay(30);
  }
  //-----------------------------------------------------------------------------
  last_on1 = millis(); 
  digitalWrite(LED_PIN, LedState);
 //----------------------------------------------------------
 //Проверка на целостность шлейфа
 if (GerkonState == 0){ //если "0" то
    digitalWrite(LED_PIN2, HIGH);     
      last_on3 = millis();               
 }else{
    digitalWrite(LED_PIN2, LedStateRed);     //мигает постоянно
 }
//------------------------------------------------------------   
  unsigned long currentMillis = millis();// частота мигания
  unsigned long currentMillisRed = millis();
//------------------------------------------------------------
//мигание для красного светодиода
if (currentMillisRed - previousMillisRed > intervalLedRed) {
    // сохраняем время последнего переключения
      previousMillisRed = currentMillisRed;
      // если светодиод не горит, то зажигаем, и наоборот
    if (LedStateRed == LOW)
      LedStateRed = HIGH;
    else
      LedStateRed = LOW;
     }
//-------------------------------------------------------------  
// Serial.print("Millis ");
// Serial.print(millis());
//  Serial.print(" last_on ");
//  Serial.print(last_on);
//  Serial.print(" Minus ");
//  Serial.println(millis() - last_on);
//Взвод сигналки на 10 секунд

 if(millis()-last_on<=EEPROM_ulong_read(addr2)){
//  
//  Serial.print("Millis ");
//  Serial.print(millis());
//  Serial.print(" last_on ");
//  Serial.print(last_on);
//  Serial.print(" Minus ");
//  Serial.println(millis() - last_on);
  //Serial.print("GerkonState ");
  //Serial.print(GerkonState);
  
//Участок отвечающий за мегание светодиода 
  if (currentMillis - previousMillis > intervalLed) {
    // сохраняем время последнего переключения
      previousMillis = currentMillis;
      // если светодиод не горит, то зажигаем, и наоборот
    if (LedState == LOW)
      LedState = HIGH;
    else
      LedState = LOW;
     }         
   }else{
//-------------------------------------------------------------- 
  if(millis()-last_on<=EEPROM_ulong_read(addr2)+2){ 
  Serial.println("Alarm start");
  delay(30);
  }
//-----------------------------------------------------------------  
    LedState = LOW;
    if (GerkonState == 0){
  digitalWrite(LED_PIN2, HIGH);   
    }else{ 
  regim++;  
    }
   }
  }
//--------------------------------------------------------------- 
  if(regim==2){
    
//  Serial.print("Millis ");
//  Serial.print(millis());
//  Serial.print(" last_on ");
//  Serial.print(last_on3);
//  Serial.print(" Minus ");
//  Serial.println(millis() - last_on3);
  
  if(millis()-last_on3<=30000){ 
    digitalWrite(LED_PIN3, HIGH);
   }else{
    digitalWrite(LED_PIN3, LOW);
   }
 // Serial.print("Millis ");
 // Serial.print(millis());
  //Serial.print(" last_on ");
  //Serial.print(last_on1);
  //Serial.print(" Minus ");
  //Serial.println(millis() - last_on);
  //Serial.print("GerkonState ");
  //Serial.print(GerkonState);
  //---------------------------------------------------------------------
  if(millis()-last_on1<=10){
  Serial.println("Allert");
  delay(30);
  } 
//------------------------------------------------------------------------  
  if(millis()-last_on1<=100){  
     sendTextMessage();   
   //delay(200); 
   }
    
   digitalWrite(LED_PIN, LOW);
   digitalWrite(LED_PIN2, LedStateRed);
   unsigned long currentMillisRed = millis(); 
   //мигание для красного светодиода
  if (currentMillisRed - previousMillisRed > intervalLedRed) {
    // сохраняем время последнего переключения
      previousMillisRed = currentMillisRed;
      // если светодиод не горит, то зажигаем, и наоборот      
    if (LedStateRed == LOW)
      LedStateRed = HIGH;
    else
      LedStateRed = LOW;
     }
  }
//---------------------------------------------------------------------------------  
char currSymb = gprsSerial.read();
String numm = currStr.substring(currStr.indexOf("\""),currStr.indexOf(",")); // выделяем номер и запоминаем в переменную num
//Serial.print (currSymb);
//Serial.println (numm);
    if ('\r' == currSymb) {
        if (isStringMessage) { 
            //если текущая строка - SMS-сообщение,
            //отреагируем на него соответствующим образом
            Serial.println (currStr);
            if (!currStr.compareTo("On")) {
                //digitalWrite(greenPin, HIGH);
                regim=1;
            } else if (!currStr.compareTo("Off")) {
                regim=0;
            } 
            isStringMessage = false;
        } else {
            if (currStr.startsWith("+CMT")) {
              Serial.println (numm);
              Serial.println ("Received a message:");
              //Serial.println (numm1);
                //если текущая строка начинается с "+CMT",
                //то следующая строка является сообщением
//                //если строка начинается с CMT то далее следует номер
//                  String numm = currStr.substring(currStr.indexOf("+"),currStr.indexOf(":")); // выделяем номер и запоминаем в переменную num                  
//                  Serial.print (numm);
                if (!numm.compareTo ("\"+7xxxxxxxxxx\""))  {
                  //Serial.println ("1");
                  isStringMessage = true;    
                }                 
            }
        }
        currStr = "";
    } else if ('\n' != currSymb) {
        currStr += String(currSymb);
    }
//--------------------------------------------------------------------------------------  
}

void sendTextMessage() { 
    //Serial.println ("OK1");  
    // Устанавливает текстовый режим для SMS-сообщений
    gprsSerial.print("AT+CMGF=1\r");
    delay(100); // даём время на усваивание команды
    // Устанавливаем адресата: телефонный номер в международном формате
//    gprsSerial.println("AT + CMGS = \"+7xxxxxxxxxx\"");
//    delay(100);
    //---------------------------------------------------------------------------------------------  
//  int cmd = nextByte();
//   
//  if(cmd == 126)
//   {
//     char charIn = 0;
//     byte i = 0;
//     while (charIn != 126) // wait for header byte again (Дождитесь снова заголовка)
//     {
//       charIn = nextByte();
//       //Serial.println (charIn);
//       
//       if(charIn != 126)
//       {
//         cmdMsg[i] = charIn;
//         i += 1;
//        //Serial.println (cmdMsg);
//         //Serial.println (i);
//       }
//     }
//     //Serial.println (cmdMsg); //Serial.println is important for C# (took me a while to figure that out)
//       //that will shoot back whatever you sent in without the '~'s
//     cmdMsg[i] = NULL;
//   //Serial.println (i);
//   }
    //-----------------------------------------------------------------------------------------------
    // Пишем текст сообщения
      EEPROM.get(addr, cmdMsg);
    //Serial.println (cmdMsg);
      gprsSerial.println(cmdMsg);
      Serial.println("Sending SMS...");
      delay(100);

//----------------------------------------------------------------------------------------------
//  int cmdd = nextByted();
//   
//  if(cmdd == 126)
//   {
//     char charInd = 0;
//     byte id = 0;
//     while (charInd != 126) // wait for header byte again (Дождитесь снова заголовка)
//     {
//       charInd = nextByte();
//       //Serial.println (charIn);
//       
//       if(charInd != 126)
//       {
//         cmdMsgd[id] = charInd;
//         id += 1;
//        //Serial.println (cmdMsg);
//         //Serial.println (i);
//       }
//     }
//     Serial.println("Text SMS:");
//     Serial.println (cmdMsgd); //Serial.println is important for C# (took me a while to figure that out)
//       //that will shoot back whatever you sent in without the '~'s
//     cmdMsgd[id] = NULL;
//   //Serial.println (i);
//   } 
//---------------------------------------------------------------------------------------------
      EEPROM.get(addr1, cmdMsgd);
      gprsSerial.println(cmdMsgd);
      delay(100);
    // Отправляем Ctrl+Z, обозначая, что сообщение готово
    gprsSerial.println((char)26);    
}

byte nextByte() {
   while(1) {
     if(Serial.available() > 0) {
         byte b =  Serial.read();
       return b;
      }
   }
}

   byte nextByted() {
   while(1) {
     if(Serial.available() > 0) {
         byte bd =  Serial.read();
       return bd;
      }
   }
}

// чтение
unsigned long EEPROM_ulong_read(int addr) {    
  byte raw[4];
  for(byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr+i);
  unsigned long &num = (unsigned long&)raw;
  //Serial.println(num);
  return num;
}

// запись
void EEPROM_ulong_write(int addr, unsigned long num) {
  byte raw[4];
  (unsigned long&)raw = num;
  for(byte i = 0; i < 4; i++) EEPROM.write(addr+i, raw[i]);       
  }   