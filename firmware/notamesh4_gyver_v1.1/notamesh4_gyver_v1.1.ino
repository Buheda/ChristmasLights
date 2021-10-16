/*
   notamesh - IR controlled 'synchronized' led lighting effects using FastLED across multiple Arduino controllers.

         By: Andrew Tuline
       Date: October, 2018
        URL: www.tuline.com
      Email: atuline@gmail.com
       Gist: https://gist.github.com/atuline
     GitHub: https://github.com/atuline
   Pastebin: http://pastebin.com/u/atuline
    Youtube: https://www.youtube.com/user/atuline/videos
*/

//  Переделка и дополнение
//  Декабрь 2018
//  Бикин Дмитрий
//  d.bikin@bk.ru
//  SysWork.ru
//  Как подключить пульт
//  https://mysku.ru/blog/aliexpress/68990.html

// Добавлено управление одной кнопкой, декабрь 2019
// AlexGyver
// https://AlexGyver.ru/
// https://www.youtube.com/c/alexgyvershow/
// 1.1 - исправлено невключение предыдущего режима

/////////////////////////////////////////////////////////////////////////////////
///                               Настройка
/////////////////////////////////////////////////////////////////////////////////

#define KOL_LED       50         // Сколько светодиодов в гирлянде при первом включении

//////////////////////////////////////// Параметры пульта
////
//Настройка действия кнопке осуществляется в файле command.h

#define USE_BTN 1   // использовать управление одной кнопкой
// клик: вкл-выкл
// 2х клик: след. режим
// 3х клик: пред. режим
// 4х клик: вкл/выкл блёстки
// удержание - яркость
#define BTN_PIN 5   // пин кнопки

#define BUZZER_ON        1           // Если равен 1 используются buzzer для оповещение (подтвержденеия) после установки режима
#define BUZZER_PIN       3           // номер пина для баззера

#define IR_ON         1           // Если равен 1  используется пульт
// если не равен 1 пульт не используется, работает в демо режиме
#define PIN_IR        8           // Номер пина, куда подключен датчик пульта,

#define IR_MAX_LEDS   50         // Максимальное количество светодиодов при работе с пультом должно быть больше
// или равно KOL_LED
// От размера этого числа зависит колличество используемой памяти


//////////////////////////////////////// Параметры гирлянды
////
#define COLOR_ORDER   RGB         //Очередность цвета светодиодов  RGB или GRB
#define CHIPSET       WS2812B     //Тип микросхемы светодиодов в гирлянде

#define LED_DT        6          // Номер пина куда подключена гирлянда
//#define LED_CK        11          // Номер пина для подключения тактов, применяется для светодиодов WS2801 или APA102
//ЕСЛИ НЕ ИСПОЛЬЗУЕТСЯ ЗАКОМЕНТИРОВАТЬ

#define POWER_V       5           // напряжение блока питания в Вольтах
#define POWER_I       3000        // Ток блока питания в милиАмперах


//////////////////////////////////////// Параметры программы
////
#define BLACKSTART    1           // Первый запуск делать с черного экрана 0- начинать с эффекта, 1- начинать с черного экрана

#define GLITER_ON     1           // Включить блеск 0 - блеск отключен, 1- блеск включен

#define CANDLE_KOL    3           // если больше 0 то будут включена эмитация свечей
// число задает через какое расстояние друг от друга располагать свечи
#define CANDLE_ON     0           // Если включена эмитация свечей то 1 = при запуске она будет включена
//                                  0 = при запуске она будет выключена

#define CANDLE_40     1           // Управление 40 эффектом (свечи)   1 = при запуске она будет включена по варианту efimova1969
//                                  иначе при запуске она будет включена по моему варианту

#define BACKGR_ON     1           // Включить заполнение черного цвета фоном

#define DEMO_MODE     0           // Режим при включении
// 0 - последняя запущенная программа
// 1 - демо режим с последовательным перебором эффектов
// 2 - демо режим с случайным перебором эффектов
// 3 - демо режим с последовательным перебором выбранных эффектов
// 4 - демо режим с случайным перебором выбранных эффектов

#define MY_MODE     0,1,2,4,5,9,10,11,14,16,17,18,19,21, 22, 28, 30, 32, 34, 36, 37, 38
// Выбранные эффекты пишутся номера через запятую,
// можно писать и одинаковые что актуально в демо 3
// Если закоментировать, то демо 3 и 4 отключаются

#define DEMO_TIME     30           // Время отображение эффекта в демо режиме в секундах

#define CHANGE_ON     1           // Включить плавную смену эффектов
#define CHANGE_SPARK  1           // Эффект смены 0 - без эффекта, 1 - искры (бенгальский огонь) 2 - яркий свет
                                  // 3 - метеорит,    4 - случайный эффект (из 1-3)
#define CHANGE_TIME   7           // Время в секундах, за которое произойдет плавная смена эффектов

#define DIRECT_TIME   20          // Через сколько секунд менять направление если 0 - не меняем
#define PALETTE_TIME  30          // Через сколько секунд менять палитру если 0 - не меняем

#define LOG_ON        1           // Включить лог  1 - включить лог

/// Параметры кнопок
//Настройка действия кнопке осуществляется в файле command.h

#define KEY_ON        0           // Если равен 1  используются кнопки подключенные к аналоговому входу
#define PIN_KEY       A3          // Номер пина, куда подключены кнопки

//Зачения смотреть в протоколе после строки Analog Key
#define KEY_0         10          // Значение соответствующее нажатой 0 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_1         70          // Значение соответствующее нажатой 1 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_2         140         // Значение соответствующее нажатой 2 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_3         280         // Значение соответствующее нажатой 3 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_4         360         // Значение соответствующее нажатой 4 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_5         520         // Значение соответствующее нажатой 5 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_6         700         // Значение соответствующее нажатой 6 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_7         900         // Значение соответствующее нажатой 7 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_DELTA     5           // погрешность значения кнопки, тоесть от -KEY_DELTA до +KEY_DELTA

//Тип пульта на который будет реагировать устройство, ставим 1 у того который используется
//это уменьшит используемую память
//Чтобы узнать имя пульта запустите в примерах IRremote / IRrecvDump
#define IR_RC5        0           // пульт RC5,
#define IR_RC6        0           // пульт RC6,          
#define IR_NEC        1           // пульт NEC,          
#define IR_SONY       0           // пульт SONY,          
#define IR_PANASONIC  0           // пульт PANASONIC,          
#define IR_JVC        0           // пульт JVC,
#define IR_SAMSUNG    0           // пульт SAMSUNG,
#define IR_WHYNTER    0           // пульт WHYNTER,
#define IR_AIWA       0           // пульт AIWA_RC_T501,
#define IR_LG         0           // пульт LG,
#define IR_SANYO      0           // пульт SANYO,
#define IR_MITSUBISHI 0           // пульт MITSUBISHI,
#define IR_DENON      0           // пульт DENON                     

#define IR_REPEAT     1           //Включить повтор, при долгом нажании на кнопку пульта будет повтор клавиши
