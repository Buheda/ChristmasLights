//////////////////////////////////////// настройки хранения параметров
#define KEEP_SETTINGS 1         // хранить ВСЕ настройки в энергонезависимой памяти
#define KEEP_STATE 1            // сохранять в памяти состояние вкл/выкл системы (с пульта)
#define RESET_SETTINGS 0        // сброс настроек в EEPROM памяти (поставить 1, прошиться, поставить обратно 0, прошиться. Всё)
#define SETTINGS_LOG 0          // вывод всех настроек из EEPROM в порт при запуске

#define INITBYTE_POS   150
#define INITBYTE_VALUE 0x55     // If this is the value in INITBYTE_POS, then the Arduino has been initialized.

#define MODE 0                    // режим светомузыки при запуске

/////////////////////  Светомузыка ////////////////////////////////////////////////

// ----- сигнал
byte psEMPTY_BRIGHT = 30;           // яркость "не горящих" светодиодов (0 - 255)

// ----- нижний порог шумов
uint16_t LOW_PASS = 100;          // нижний порог шумов режим VU, ручная настройка
uint16_t psSPEKTR_LOW_PASS = 40;    // нижний порог шумов режим спектра, ручная настройка

// ----- режим шкала громкости
float psSMOOTH = 0.3;               // коэффициент плавности анимации VU (по умолчанию 0.5)

// ----- режим цветомузыки
float psSMOOTH_FREQ = 0.8;          // коэффициент плавности анимации частот (по умолчанию 0.8)
float psMAX_COEF_FREQ = 1.2;        // коэффициент порога для "вспышки" цветомузыки (по умолчанию 1.5)

// ----- режим стробоскопа
uint16_t psSTROBE_PERIOD = 140;     // период вспышек, миллисекунды
uint8_t psSTROBE_SMOOTH = 200;         // скорость нарастания/угасания вспышки (0 - 255)

// ----- настройки радуги
float psRAINBOW_STEP = 5.00;        // шаг изменения цвета радуги

// ----- режим подсветки
byte psLIGHT_COLOR = 0;             // начальный цвет подсветки
byte psLIGHT_SAT = 255;             // начальная насыщенность подсветки
float psRAINBOW_STEP_2 = 0.5;
byte psCOLOR_SPEED = 100;
int psRAINBOW_PERIOD = 1;

// ----- режим бегущих частот
byte psRUNNING_SPEED = 11;

// ----- режим анализатора спектра
byte psHUE_START = 0;
byte psHUE_STEP = 5;

unsigned long eeprom_timer;
uint8_t psMUSIC_MODE = MODE;
uint8_t psFREQ_STROBE_MODE, psLIGHT_MODE;
boolean ONstate = true, eeprom_flag;

#if KEEP_SETTINGS == 1
//#include <EEPROMex.h>
#include <EEPROM.h>

#endif

/////////////////////  Гирлянда ////////////////////////////////////////////////
uint8_t psLED_MODE = 0;           // номер режима со старта

uint16_t psMESH_DELAY = 0;           // Timer for the notamesh
uint16_t psTHIS_DELAY = 0;           // Задержка delay
int psMAX_BRIGHT = 255;              //Bright.

void readEEPROM() {
#if KEEP_SETTINGS == 1
  EEPROM.get(1, psMUSIC_MODE);
  EEPROM.get(2, psFREQ_STROBE_MODE);
  EEPROM.get(3, psLIGHT_MODE);
  EEPROM.get(4, psRAINBOW_STEP);
  EEPROM.get(5, psMAX_COEF_FREQ);
  EEPROM.get(12, psSTROBE_PERIOD);
  EEPROM.get(16, psLIGHT_SAT);
  EEPROM.get(20, psRAINBOW_STEP_2);
  EEPROM.get(24, psHUE_START);
  EEPROM.get(28, psSMOOTH);
  EEPROM.get(32, psSMOOTH_FREQ);
  EEPROM.get(36, psSTROBE_SMOOTH);
  EEPROM.get(40, psLIGHT_COLOR);
  EEPROM.get(44, psCOLOR_SPEED);
  EEPROM.get(48, psRAINBOW_PERIOD);
  EEPROM.get(52, psRUNNING_SPEED);
  EEPROM.get(56, psHUE_STEP);
  EEPROM.get(60, psEMPTY_BRIGHT);
  EEPROM.get(64, psLED_MODE);
  EEPROM.get(68, psMESH_DELAY);
  EEPROM.get(72, psTHIS_DELAY);
  EEPROM.get(76, psMAX_BRIGHT);

  if (KEEP_STATE == 1) EEPROM.get(100, ONstate);
  //Byte = 1 byte, Int = 4 bytes, Float = 4 bytes
  /* psMUSIC_MODE = EEPROM.readByte(1);
    psFREQ_STROBE_MODE = EEPROM.readByte(2);
    psLIGHT_MODE = EEPROM.readByte(3);
    psRAINBOW_STEP = EEPROM.readInt(4);
    psMAX_COEF_FREQ = EEPROM.readFloat(8);
    psSTROBE_PERIOD = EEPROM.readInt(12);
    psLIGHT_SAT = EEPROM.readInt(16);
    psRAINBOW_STEP_2 = EEPROM.readFloat(20);
    psHUE_START = EEPROM.readInt(24);
    psSMOOTH = EEPROM.readFloat(28);
    psSMOOTH_FREQ = EEPROM.readFloat(32);
    psSTROBE_SMOOTH = EEPROM.readInt(36);
    psLIGHT_COLOR = EEPROM.readInt(40);
    psCOLOR_SPEED = EEPROM.readInt(44);
    psRAINBOW_PERIOD = EEPROM.readInt(48);
    psRUNNING_SPEED = EEPROM.readInt(52);
    psHUE_STEP = EEPROM.readInt(56);
    psEMPTY_BRIGHT = EEPROM.readInt(60);

    psLED_MODE = EEPROM.readByte(64);
    psMESH_DELAY = EEPROM.readInt(68);
    psTHIS_DELAY = EEPROM.readInt(72);
    psMAX_BRIGHT = EEPROM.readInt(76);

    isChristmasLight = (psLED_MODE == COLOR_MUSIC_MODE);
    if (KEEP_STATE == 1) ONstate = EEPROM.readByte(100);*/
  isChristmasLight = (psLED_MODE != COLOR_MUSIC_MODE);

#endif
}

void updateEEPROM() {
#if KEEP_SETTINGS == 1
  EEPROM.put(1, psMUSIC_MODE);
  EEPROM.put(2, psFREQ_STROBE_MODE);
  EEPROM.put(3, psLIGHT_MODE);
  EEPROM.put(4, psRAINBOW_STEP);
  EEPROM.put(8, psMAX_COEF_FREQ);
  EEPROM.put(12, psSTROBE_PERIOD);
  EEPROM.put(16, psLIGHT_SAT);
  EEPROM.put(20, psRAINBOW_STEP_2);
  EEPROM.put(24, psHUE_START);
  EEPROM.put(28, psSMOOTH);
  EEPROM.put(32, psSMOOTH_FREQ);
  EEPROM.put(36, psSTROBE_SMOOTH);
  EEPROM.put(40, psLIGHT_COLOR);
  EEPROM.put(44, psCOLOR_SPEED);
  EEPROM.put(48, psRAINBOW_PERIOD);
  EEPROM.put(52, psRUNNING_SPEED);
  EEPROM.put(56, psHUE_STEP);
  EEPROM.put(60, psEMPTY_BRIGHT);

  EEPROM.put(64, psLED_MODE);
  EEPROM.put(68, psMESH_DELAY);
  EEPROM.put(72, psTHIS_DELAY);
  EEPROM.put(76, psMAX_BRIGHT);

  if (KEEP_STATE == 1) EEPROM.put(100, ONstate);
  /*EEPROM.updateByte(1, psMUSIC_MODE);
    EEPROM.updateByte(2, psFREQ_STROBE_MODE);
    EEPROM.updateByte(3, psLIGHT_MODE);
    EEPROM.updateInt(4, psRAINBOW_STEP);
    EEPROM.updateFloat(8, MAX_COEF_FREQ);
    EEPROM.updateInt(12, psSTROBE_PERIOD);
    EEPROM.updateInt(16, psLIGHT_SAT);
    EEPROM.updateFloat(20, psRAINBOW_STEP_2);
    EEPROM.updateInt(24, psHUE_START);
    EEPROM.updateFloat(28, psSMOOTH);
    EEPROM.updateFloat(32, psSMOOTH_FREQ);
    EEPROM.updateInt(36, psSTROBE_SMOOTH);
    EEPROM.updateInt(40, psLIGHT_COLOR);
    EEPROM.updateInt(44, psCOLOR_SPEED);
    EEPROM.updateInt(48, psRAINBOW_PERIOD);
    EEPROM.updateInt(52, psRUNNING_SPEED);
    EEPROM.updateInt(56, psHUE_STEP);
    EEPROM.updateInt(60, psEMPTY_BRIGHT);

    EEPROM.updateByte(64, psLED_MODE);
    EEPROM.updateInt(68, psMESH_DELAY);
    EEPROM.updateInt(72, psTHIS_DELAY);
    EEPROM.updateInt(76, psMAX_BRIGHT);

    if (KEEP_STATE == 1) EEPROM.updateByte(100, ONstate);*/
#endif
}

void printEEPROMSettings() {
#if (SETTINGS_LOG == 1 )
  Serial.print(F("psLED_MODE = ")); Serial.println(psLED_MODE);
  Serial.print(F("psMUSIC_MODE = ")); Serial.println(psMUSIC_MODE);
  Serial.print(F("psFREQ_STROBE_MODE = ")); Serial.println(psFREQ_STROBE_MODE);
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
  Serial.print(F("psLED_MODE = ")); Serial.println(psLED_MODE);
  Serial.print(F("psMESH_DELAY = ")); Serial.println(psMESH_DELAY);
  Serial.print(F("psTHIS_DELAY = ")); Serial.println(psTHIS_DELAY);
  Serial.print(F("MAX_BRIGHT = ")); Serial.println(MAX_BRIGHT);

  Serial.print(F("ONstate = ")); Serial.println(ONstate);
#endif
}

void updateEEPROM_MAX_BRIGHT() {
#if KEEP_SETTINGS == 1
  EEPROM.put(76, psMAX_BRIGHT);
  //EEPROM.updateInt(76, psMAX_BRIGHT);
#endif
}

void updateEEPROM_THIS_DELAY() {
#if KEEP_SETTINGS == 1
  EEPROM.put(72, psTHIS_DELAY);
  //EEPROM.updateInt(72, psTHIS_DELAY);
#endif
}

void updateEEPROM_MESH_DELAY() {
#if KEEP_SETTINGS == 1
  EEPROM.put(68, psMESH_DELAY);
  //EEPROM.updateInt(68, psMESH_DELAY);

#endif
}

void updateEEPROM_LED_MODE() {

#if KEEP_SETTINGS == 1
#if LOG_ON == 1
  Serial.print(F("Save Mode:")); Serial.println(psLED_MODE);
#endif
  EEPROM.put(64, psLED_MODE);
  //EEPROM.updateByte(64, psLED_MODE);
#endif
}

void updateEEPROM_RESET() {
#if KEEP_SETTINGS == 1
  EEPROM.put(INITBYTE_POS, 0);
  //EEPROM.updateByte(INITBYTE_POS, 0);

#endif
}

void updateEEPROM_INIT() {
#if KEEP_SETTINGS == 1
  EEPROM.put(INITBYTE_POS, INITBYTE_VALUE);
  //EEPROM.updateByte(INITBYTE_POS, INITBYTE_VALUE);
#endif
}

bool readEEPROM_ISINIT() {
#if KEEP_SETTINGS == 1
  byte initValue;
  EEPROM.get(INITBYTE_POS, initValue);
  //initValue = EEPROM.readByte(INITBYTE_POS);
#if (SETTINGS_LOG == 1 )
  Serial.println(initValue); Serial.println(INITBYTE_VALUE);
#endif
  return initValue == INITBYTE_VALUE;
#else
  return false;
#endif
}

void updateEEPROM_LOW_PASS() {
#if KEEP_SETTINGS == 1
  EEPROM.put(80, LOW_PASS);
  //EEPROM.updateInt(80, LOW_PASS);
#endif
}

void updateEEPROM_SPEKTR_LOW_PASS() {
#if KEEP_SETTINGS == 1
  EEPROM.put(84, psSPEKTR_LOW_PASS);
  //EEPROM.updateInt(84, psSPEKTR_LOW_PASS);
#endif
}

int readEEPROM_LOW_PASS() {
#if KEEP_SETTINGS == 1
  EEPROM.get(80, LOW_PASS);
  //  LOW_PASS = EEPROM.readByte(80);
#endif
  return LOW_PASS;
}

int readEEPROM_SPEKTR_LOW_PASS() {
#if KEEP_SETTINGS == 1
  EEPROM.get(84, psSPEKTR_LOW_PASS);
  //psSPEKTR_LOW_PASS = EEPROM.readByte(84);
#endif
  return psSPEKTR_LOW_PASS;
}

void eepromTick() {
#if KEEP_SETTINGS == 1
  if (eeprom_flag)
    if (millis() - eeprom_timer > 30000) {  // 30 секунд после последнего нажатия с пульта
      eeprom_flag = false;
      eeprom_timer = millis();
      updateEEPROM();
    }
#endif
}


void setup_persistency() {
  // в INITBYTE_POS ячейке хранится число INITBYTE_VALUE. Если нет - значит это первый запуск системы
#if KEEP_SETTINGS == 1
  if (!readEEPROM_ISINIT()) {
    //Serial.println(F("First start"));
    updateEEPROM_INIT();
    updateEEPROM();
  } else {
    readEEPROM();
  }
  printEEPROMSettings();
#endif
  readEEPROM();

}
