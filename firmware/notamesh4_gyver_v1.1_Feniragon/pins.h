#define BUZZER_ON     1           // Если равен 1 используются buzzer для оповещение (подтвержденеия) после установки режима
#if BUZZER_ON
#define BUZZER_PIN      3           // номер пина для баззера
#endif

#define IR_ON         1           // Если равен 1  используется пульт
// если не равен 1 пульт не используется, работает в демо режиме
#if IR_ON
#define PIN_IR          8           // Номер пина, куда подключен датчик пульта,

//Тип пульта на который будет реагировать устройство, ставим 1 у того который используется
//это уменьшит используемую память
//Чтобы узнать имя пульта запустите в примерах IRremote / IRrecvDump
#define IR_RC5            0           // пульт RC5,
#define IR_RC6            0           // пульт RC6,          
#define IR_NEC            1           // пульт NEC,          
#define IR_SONY           0           // пульт SONY,          
#define IR_PANASONIC      0           // пульт PANASONIC,          
#define IR_JVC            0           // пульт JVC,
#define IR_SAMSUNG        0           // пульт SAMSUNG,
#define IR_WHYNTER        0           // пульт WHYNTER,
#define IR_AIWA           0           // пульт AIWA_RC_T501,
#define IR_LG             0           // пульт LG,
#define IR_SANYO          0           // пульт SANYO,
#define IR_MITSUBISHI     0           // пульт MITSUBISHI,
#define IR_DENON          0           // пульт DENON                     

#define IR_REPEAT         1           //Включить повтор, при долгом нажании на кнопку пульта будет повтор клавиши
#endif

//#define LED_CK      11          // Номер пина для подключения тактов, применяется для светодиодов WS2801 или APA102
//ЕСЛИ НЕ ИСПОЛЬЗУЕТСЯ ЗАКОМЕНТИРОВАТЬ

/// Параметры кнопок
//Настройка действия кнопке осуществляется в файле command.h

#define KEY_ON        0           // Если равен 1 использутся клавиатурные кнопки, подключенные к аналоговому входу
#if KEY_ON
#define PIN_KEY         A4          // Номер пина, куда подключены кнопки клавиатуры

//Зачения смотреть в протоколе после строки Analog Key
#define KEY_0             10          // Значение соответствующее нажатой 0 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_1             70          // Значение соответствующее нажатой 1 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_2             140         // Значение соответствующее нажатой 2 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_3             280         // Значение соответствующее нажатой 3 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_4             360         // Значение соответствующее нажатой 4 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_5             520         // Значение соответствующее нажатой 5 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_6             700         // Значение соответствующее нажатой 6 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_7             900         // Значение соответствующее нажатой 7 кнопке
// если значение меньше KEY_DELTA, то кнопка не используется
#define KEY_DELTA         5           // погрешность значения кнопки, тоесть от -KEY_DELTA до +KEY_DELTA
#endif

#define USE_BTN       0           // использовать управление одной кнопкой
// клик: вкл-выкл
// 2х клик: след. режим
// 3х клик: пред. режим
// 4х клик: вкл/выкл блёстки
// удержание - яркость
#if USE_BTN
#define BTN_PIN         5           // кнопка переключения режимов (PIN --- КНОПКА --- GND)
#endif

#if defined(__AVR_ATmega32U4__)     // Пины для Arduino Pro Micro (смотри схему для Pro Micro на странице проекта!!!)
#define MLED_PIN        17          // пин светодиода режимов на ProMicro, т.к. обычный не выведен.
#define MLED_ON LOW
#define LED_DT          9           // пин DI светодиодной ленты на ProMicro, т.к. обычный не выведен.
#else                               // Пины для других плат Arduino (по умолчанию)
#define MLED_PIN        13          // пин светодиода режимов
#define MLED_ON HIGH
#define LED_DT          6           // Номер пина куда подключена гирлянда
#endif

#define POT_GND         A0          // пин земля для потенциометра

// ----- пины подключения jack 3.5
#define SOUND_R         A2          // аналоговый пин вход аудио, правый канал
#define SOUND_L         A1          // аналоговый пин вход аудио, левый канал
#define SOUND_R_FREQ    A3          // аналоговый пин вход аудио для режима с частотами (через кондер)
