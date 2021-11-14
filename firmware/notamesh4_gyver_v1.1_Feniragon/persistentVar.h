//////////////////////////////////////// настройки хранения параметров
#define KEEP_SETTINGS 1         // хранить ВСЕ настройки в энергонезависимой памяти
#define KEEP_STATE 1            // сохранять в памяти состояние вкл/выкл системы (с пульта)
#define RESET_SETTINGS 0        // сброс настроек в EEPROM памяти (поставить 1, прошиться, поставить обратно 0, прошиться. Всё)
#define SETTINGS_LOG 0          // вывод всех настроек из EEPROM в порт при запуске

#define INITBYTE_POS   150
#define INITBYTE_VALUE 0x55     // If this is the value in INITBYTE_POS, then the Arduino has been initialized.

uint8_t START_MODE = 0;           // номер режима со старта
#define MODE 0                    // режим светомузыки при запуске

/////////////////////  Светомузыка ////////////////////////////////////////////////

// ----- сигнал
byte EMPTY_BRIGHT = 30;           // яркость "не горящих" светодиодов (0 - 255)

// ----- нижний порог шумов
uint16_t LOW_PASS = 100;          // нижний порог шумов режим VU, ручная настройка
uint16_t SPEKTR_LOW_PASS = 40;    // нижний порог шумов режим спектра, ручная настройка

// ----- режим шкала громкости
float SMOOTH = 0.3;               // коэффициент плавности анимации VU (по умолчанию 0.5)

// ----- режим цветомузыки
float SMOOTH_FREQ = 0.8;          // коэффициент плавности анимации частот (по умолчанию 0.8)
float MAX_COEF_FREQ = 1.2;        // коэффициент порога для "вспышки" цветомузыки (по умолчанию 1.5)

// ----- режим стробоскопа
uint16_t STROBE_PERIOD = 140;     // период вспышек, миллисекунды
byte STROBE_SMOOTH = 200;         // скорость нарастания/угасания вспышки (0 - 255)

// ----- настройки радуги
float RAINBOW_STEP = 5.00;        // шаг изменения цвета радуги

// ----- режим подсветки
byte LIGHT_COLOR = 0;             // начальный цвет подсветки
byte LIGHT_SAT = 255;             // начальная насыщенность подсветки
float RAINBOW_STEP_2 = 0.5;
byte COLOR_SPEED = 100;
int RAINBOW_PERIOD = 1;

// ----- режим бегущих частот
byte RUNNING_SPEED = 11;

// ----- режим анализатора спектра
byte HUE_START = 0;
byte HUE_STEP = 5;

unsigned long eeprom_timer;
byte THIS_MODE = MODE;
int8_t FREQ_STROBE_MODE, LIGHT_MODE;
boolean ONstate = true, eeprom_flag;

#if KEEP_SETTINGS
#include <EEPROMex.h>
#endif

/////////////////////  Гирлянда ////////////////////////////////////////////////
uint8_t LED_MODE = 0;             // номер текущего режима

uint16_t MESH_DELAY = 0;           // Timer for the notamesh
uint16_t THIS_DELAY = 0;           // Задержка delay
int MAX_BRIGHT = 255;              //Bright.

void readEEPROM() {
#if KEEP_SETTINGS
  //Byte = 1 byte, Int = 4 bytes, Float = 4 bytes
  START_MODE = EEPROM.readByte(0);
  THIS_MODE = EEPROM.readByte(1);
  FREQ_STROBE_MODE = EEPROM.readByte(2);
  LIGHT_MODE = EEPROM.readByte(3);
  RAINBOW_STEP = EEPROM.readInt(4);
  MAX_COEF_FREQ = EEPROM.readFloat(8);
  STROBE_PERIOD = EEPROM.readInt(12);
  LIGHT_SAT = EEPROM.readInt(16);
  RAINBOW_STEP_2 = EEPROM.readFloat(20);
  HUE_START = EEPROM.readInt(24);
  SMOOTH = EEPROM.readFloat(28);
  SMOOTH_FREQ = EEPROM.readFloat(32);
  STROBE_SMOOTH = EEPROM.readInt(36);
  LIGHT_COLOR = EEPROM.readInt(40);
  COLOR_SPEED = EEPROM.readInt(44);
  RAINBOW_PERIOD = EEPROM.readInt(48);
  RUNNING_SPEED = EEPROM.readInt(52);
  HUE_STEP = EEPROM.readInt(56);
  EMPTY_BRIGHT = EEPROM.readInt(60);

  LED_MODE = EEPROM.readByte(64);
  MESH_DELAY = EEPROM.readInt(68);
  THIS_DELAY = EEPROM.readInt(72);
  MAX_BRIGHT = EEPROM.readInt(76);


  if (KEEP_STATE) ONstate = EEPROM.readByte(100);
#endif
}

void updateEEPROM() {
#if KEEP_SETTINGS
  EEPROM.updateByte(0, START_MODE);
  EEPROM.updateByte(1, THIS_MODE);
  EEPROM.updateByte(2, FREQ_STROBE_MODE);
  EEPROM.updateByte(3, LIGHT_MODE);
  EEPROM.updateInt(4, RAINBOW_STEP);
  EEPROM.updateFloat(8, MAX_COEF_FREQ);
  EEPROM.updateInt(12, STROBE_PERIOD);
  EEPROM.updateInt(16, LIGHT_SAT);
  EEPROM.updateFloat(20, RAINBOW_STEP_2);
  EEPROM.updateInt(24, HUE_START);
  EEPROM.updateFloat(28, SMOOTH);
  EEPROM.updateFloat(32, SMOOTH_FREQ);
  EEPROM.updateInt(36, STROBE_SMOOTH);
  EEPROM.updateInt(40, LIGHT_COLOR);
  EEPROM.updateInt(44, COLOR_SPEED);
  EEPROM.updateInt(48, RAINBOW_PERIOD);
  EEPROM.updateInt(52, RUNNING_SPEED);
  EEPROM.updateInt(56, HUE_STEP);
  EEPROM.updateInt(60, EMPTY_BRIGHT);

  EEPROM.updateByte(64, LED_MODE);
  EEPROM.updateInt(68, MESH_DELAY);
  EEPROM.updateInt(72, THIS_DELAY);
  EEPROM.updateInt(76, MAX_BRIGHT);

  if (KEEP_STATE) EEPROM.updateByte(100, ONstate);
#endif
}

void printEEPROMSettings() {
#if (SETTINGS_LOG == 1 && LOG_ON)
  Serial.print(F("START_MODE = ")); Serial.println(START_MODE);
  Serial.print(F("THIS_MODE = ")); Serial.println(THIS_MODE);
  Serial.print(F("FREQ_STROBE_MODE = ")); Serial.println(FREQ_STROBE_MODE);
  Serial.print(F("LIGHT_MODE = ")); Serial.println(LIGHT_MODE);
  Serial.print(F("RAINBOW_STEP = ")); Serial.println(RAINBOW_STEP);
  Serial.print(F("MAX_COEF_FREQ = ")); Serial.println(MAX_COEF_FREQ);
  Serial.print(F("STROBE_PERIOD = ")); Serial.println(STROBE_PERIOD);
  Serial.print(F("LIGHT_SAT = ")); Serial.println(LIGHT_SAT);
  Serial.print(F("RAINBOW_STEP_2 = ")); Serial.println(RAINBOW_STEP_2);
  Serial.print(F("HUE_START = ")); Serial.println(HUE_START);
  Serial.print(F("SMOOTH = ")); Serial.println(SMOOTH);
  Serial.print(F("SMOOTH_FREQ = ")); Serial.println(SMOOTH_FREQ);
  Serial.print(F("STROBE_SMOOTH = ")); Serial.println(STROBE_SMOOTH);
  Serial.print(F("LIGHT_COLOR = ")); Serial.println(LIGHT_COLOR);
  Serial.print(F("COLOR_SPEED = ")); Serial.println(COLOR_SPEED);
  Serial.print(F("RAINBOW_PERIOD = ")); Serial.println(RAINBOW_PERIOD);
  Serial.print(F("RUNNING_SPEED = ")); Serial.println(RUNNING_SPEED);
  Serial.print(F("HUE_STEP = ")); Serial.println(HUE_STEP);
  Serial.print(F("EMPTY_BRIGHT = ")); Serial.println(EMPTY_BRIGHT);
  Serial.print(F("LED_MODE = ")); Serial.println(LED_MODE);
  Serial.print(F("MESH_DELAY = ")); Serial.println(MESH_DELAY);
  Serial.print(F("THIS_DELAY = ")); Serial.println(THIS_DELAY);
  Serial.print(F("MAX_BRIGHT = ")); Serial.println(MAX_BRIGHT);

  Serial.print(F("ONstate = ")); Serial.println(ONstate);
#endif
}

void updateEEPROM_MAX_BRIGHT() {
#if KEEP_SETTINGS
  EEPROM.updateInt(76, MAX_BRIGHT);
#endif
}

void updateEEPROM_THIS_DELAY() {
#if KEEP_SETTINGS
  EEPROM.updateInt(72, THIS_DELAY);
#endif
}

void updateEEPROM_MESH_DELAY() {
#if KEEP_SETTINGS
  EEPROM.updateInt(68, MESH_DELAY);
#endif
}

void updateEEPROM_START_MODE() {
#if KEEP_SETTINGS
  EEPROM.updateByte(0, START_MODE);
#endif
}

void updateEEPROM_RESET() {
#if KEEP_SETTINGS
  EEPROM.updateByte(INITBYTE_POS, 0);
#endif
}

void updateEEPROM_SET() {
#if KEEP_SETTINGS
  EEPROM.updateByte(INITBYTE_POS, INITBYTE_VALUE);
#endif
}

bool readEEPROM_ISINIT() {
#if KEEP_SETTINGS
  return EEPROM.readByte(INITBYTE_POS) == INITBYTE_VALUE;
#else
  return false;
#endif
}

void updateEEPROM_LOW_PASS() {
#if KEEP_SETTINGS
  EEPROM.updateInt(80, LOW_PASS);
#endif
}

void updateEEPROM_SPEKTR_LOW_PASS() {
#if KEEP_SETTINGS
  EEPROM.updateInt(84, SPEKTR_LOW_PASS);
#endif
}

int readEEPROM_LOW_PASS() {
#if KEEP_SETTINGS
  LOW_PASS = EEPROM.readByte(80);
#endif
  return LOW_PASS;
}

int readEEPROM_SPEKTR_LOW_PASS() {
#if KEEP_SETTINGS
  SPEKTR_LOW_PASS = EEPROM.readByte(84);
#endif
  return SPEKTR_LOW_PASS;
}

void eepromTick() {
#if KEEP_SETTINGS
  if (eeprom_flag)
    if (millis() - eeprom_timer > 30000) {  // 30 секунд после последнего нажатия с пульта
      eeprom_flag = false;
      eeprom_timer = millis();
      updateEEPROM();
    }
#endif
}
