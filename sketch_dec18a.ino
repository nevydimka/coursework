#include <Wire.h>             // Подключаем бибилиотеку для работы с I2C устройствами
#include <DS3231.h>           // Подключаем библиотеку для работы с RTC DS3231
#include <Servo.h>

DS3231 clock;                 // Связываем объект clock с библиотекой DS3231
RTCDateTime DateTime;         // Определяем сущность структуры RTCDateTime (описанной в библиотеке DS3231) для хранения считанных с часов даты и времени
//RTCAlarmTime Alarm1; 
//int alarmLED = 10; // Определяем сущность структуры RTCAlarmTime (описанной в библиотеке DS3231) для хранения считанных с первого будильника настроек даты и времени
int intervals[]= {1086};
Servo servo; 
void setup()
{
  Serial.begin(9600);               // Инициализируем вывод данных на монитор серийного порта, со скоростью 9600 бод
  clock.begin();                    // Инициализируем работу с объектом библиотеки DS3231

  clock.setDateTime(__DATE__, __TIME__);                  // Устанавливаем время на часах, основываясь на времени компиляции скетча
  clock.setAlarm1(0, 0, 0, 1, DS3231_MATCH_S);           // Устанавливаем первый будильник на срабатывание в 10 сек. каждой минуты. Режим DS3231_MATCH_S сообщает о том, что ориентироваться надо на секунды.
 servo.attach(10); //привязываем привод к порту 10
}

bool checkInterval(int intervals[], int size, int interval){
  for(int i = 0; i < size;){
    if(intervals[i]==interval){
      return true;
    }
    i=i+1;
  }
  return false;
}

void turnOn()
{
servo.write(10); //ставимо вал під 110

delay(2000); //чекаємо 5 секунд
}
void turnOff()
{
servo.write(110); //ставимо вал під 180

delay(2000); //чекаємо 5 секунд
}

void loop()
{
  DateTime = clock.getDateTime();                                                 // Считываем c часов текущие значения даты и времени в сущность DateTime
                                                                                                            // Перевод строки
  if (clock.isAlarm1())                                                                    // Если поступил сигнал о сработке будильника
  {

  Serial.println(clock.dateFormat("d.m.Y H:i:s - l   ", DateTime));               // Определяем формат вывода даты и выводим на монитор серийного порта
  Serial.println("Alarm: " + String(clock.dateFormat("__ __:__:s", clock.getAlarm1())));     // Выводим настройки будильника                                
  Serial.println();
    
   if (checkInterval(intervals, 1, DateTime.hour * 60 + DateTime.minute)) 
   {
      Serial.println("Servo");
      turnOn();
      turnOff();
   }
  } 
  delay(1000);                                                                    // Задержка в 1 секунду
}
