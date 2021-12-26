/*
  Скетч к проекту "Светомузыка на Arduino"
  Страница проекта (схемы, описания): https://alexgyver.ru/colormusic/
  Исходники на GitHub: https://github.com/AlexGyver/ColorMusic
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  https://AlexGyver.ru/
  Как откалибровать уровень шума и как пользоваться пультом
  расписано на странице проекта! https://alexgyver.ru/colormusic/
*/

/*
   Версия 2.10
   Исправлен глюк с большим количеством светодиодов на МЕГЕ
*/

/*
  Объединен с новогодней гирляндой "notamesh4_gyver_v1.1" by Feniragon 1.0
*/

// ***************************** НАСТРОЙКИ *****************************
// ----- настройка ИК пульта

// ----- отрисовка
#define MAIN_LOOP 5               // период основного цикла отрисовки (по умолчанию 5)

// ----- сигнал
#define MONO 0                    // 1 - только один канал (ПРАВЫЙ!!!!! SOUND_R!!!!!), 0 - два канала
#define EXP 1.4                   // степень усиления сигнала (для более "резкой" работы) (по умолчанию 1.4)
#define POTENT 0                  // 1 - используем потенциометр, 0 - используется внутренний источник опорного напряжения 1.1 В
#define psEMPTY_COLOR HUE_PURPLE    // цвет "не горящих" светодиодов. Будет чёрный, если яркость 0

// ----- нижний порог шумов
#define AUTO_LOW_PASS 0           // разрешить настройку нижнего порога шумов при запуске (по умолч. 0)
#define EEPROM_LOW_PASS 1         // порог шумов хранится в энергонезависимой памяти (по умолч. 1)
#define LOW_PASS_ADD 13           // "добавочная" величина к нижнему порогу, для надёжности (режим VU)
#define LOW_PASS_FREQ_ADD 3       // "добавочная" величина к нижнему порогу, для надёжности (режим частот)

// ----- режим шкала громкости
#define MAX_COEF 1.8              // коэффициент громкости (максимальное равно срднему * этот коэф) (по умолчанию 1.8)

// ----- режим цветомузыки
#define psSMOOTH_STEP 20            // шаг уменьшения яркости в режиме цветомузыки (чем больше, тем быстрее гаснет)
#define LOW_COLOR HUE_RED         // цвет низких частот
#define MID_COLOR HUE_GREEN       // цвет средних
#define HIGH_COLOR HUE_YELLOW     // цвет высоких

// ----- режим стробоскопа
#define STROBE_DUTY 20            // скважность вспышек (1 - 99) - отношение времени вспышки ко времени темноты
#define STROBE_COLOR HUE_YELLOW   // цвет стробоскопа
#define STROBE_SAT 0              // насыщенность. Если 0 - цвет будет БЕЛЫЙ при любом цвете (0 - 255)


// ----- режим анализатора спектра
#define LIGHT_SMOOTH 2

/*
  Цвета для HSV
  HUE_RED
  HUE_ORANGE
  HUE_YELLOW
  HUE_GREEN
  HUE_AQUA
  HUE_BLUE
  HUE_PURPLE
  HUE_PINK
*/

// ------------------------------ ДЛЯ РАЗРАБОТЧИКОВ --------------------------------
#define MODE_AMOUNT 9      // количество режимов

#define STRIPE NUM_LEDS / 5
float freq_to_stripe = NUM_LEDS / 40; // /2 так как симметрия, и /20 так как 20 частот

#define FHT_N 64         // ширина спектра х2
#define LOG_OUT 1
#include <FHT.h>         // преобразование Хартли

// градиент-палитра от зелёного к красному
DEFINE_GRADIENT_PALETTE(soundlevel_gp) {
  0,    0,    255,  0,  // green
  100,  255,  255,  0,  // yellow
  150,  255,  100,  0,  // orange
  200,  255,  50,   0,  // red
  255,  255,  0,    0   // red
};
CRGBPalette32 myPal = soundlevel_gp;

int Rlenght, Llenght;
float RsoundLevel, RsoundLevel_f;
float LsoundLevel, LsoundLevel_f;

float averageLevel = 50;
int maxLevel = 100;
int MAX_CH = NUM_LEDS / 2;
int hue;
unsigned long main_timer, hue_timer, strobe_timer, running_timer, color_timer, rainbow_timer;
float averK = 0.006;
byte count;
float index = (float)255 / MAX_CH;   // коэффициент перевода для палитры
boolean lowFlag;
byte low_pass;
int RcurrentLevel, LcurrentLevel;
int colorMusic[3];
float colorMusic_f[3], colorMusic_aver[3];
boolean colorMusicFlash[3], strobeUp_flag, strobeDwn_flag;
int thisBright[3], strobe_bright = 0;
unsigned int light_time = psSTROBE_PERIOD * STROBE_DUTY / 100;
volatile boolean ir_flag;
boolean settings_mode;
int freq_max;
float freq_max_f, rainbow_steps;
int freq_f[32];
int this_color;
boolean running_flag[3];

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
// ------------------------------ ДЛЯ РАЗРАБОТЧИКОВ --------------------------------

void HIGHS() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
}
void MIDS() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
}
void LOWS() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
}
void SILENCE() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
}

// вспомогательная функция, изменяет величину value на шаг incr в пределах minimum.. maximum
int smartIncr(int value, int incr_step, int mininmum, int maximum) {
  int val_buf = value + incr_step;
  val_buf = constrain(val_buf, mininmum, maximum);
  return val_buf;
}

float smartIncrFloat(float value, float incr_step, float mininmum, float maximum) {
  float val_buf = value + incr_step;
  val_buf = constrain(val_buf, mininmum, maximum);
  return val_buf;
}

void analyzeAudio() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(SOUND_R_FREQ);
    fht_input[i] = sample; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}

void autoLowPass() {
  // для режима VU
  delay(10);                                // ждём инициализации АЦП
  int thisMax = 0;                          // максимум
  int thisLevel;
  for (byte i = 0; i < 200; i++) {
    thisLevel = analogRead(SOUND_R);        // делаем 200 измерений
    if (thisLevel > thisMax)                // ищем максимумы
      thisMax = thisLevel;                  // запоминаем
    delay(4);                               // ждём 4мс
  }
  LOW_PASS = thisMax + LOW_PASS_ADD;        // нижний порог как максимум тишины + некая величина

  // для режима спектра
  thisMax = 0;
  for (byte i = 0; i < 100; i++) {          // делаем 100 измерений
    analyzeAudio();                         // разбить в спектр
    for (byte j = 2; j < 32; j++) {         // первые 2 канала - хлам
      thisLevel = fht_log_out[j];
      if (thisLevel > thisMax)              // ищем максимумы
        thisMax = thisLevel;                // запоминаем
    }
    delay(4);                               // ждём 4мс
  }
  psSPEKTR_LOW_PASS = thisMax + LOW_PASS_FREQ_ADD;  // нижний порог как максимум тишины
  if (EEPROM_LOW_PASS && !AUTO_LOW_PASS) {
    updateEEPROM_LOW_PASS();
    updateEEPROM_SPEKTR_LOW_PASS();
  }
}

void fullLowPass() {
  digitalWrite(MLED_PIN, MLED_ON);   // включить светодиод
  FastLED.setBrightness(0); // погасить ленту
  FastLED.clear();          // очистить массив пикселей
  FastLED.show();           // отправить значения на ленту
  delay(500);               // подождать чутка
  autoLowPass();            // измерить шумы
  delay(500);               // подождать
  FastLED.setBrightness(psMAX_BRIGHT);  // вернуть яркость
  digitalWrite(MLED_PIN, !MLED_ON);    // выключить светодиод
}

void buttonTick() {
#if USE_BTN
  btn.tick();  // обязательная функция отработки. Должна постоянно опрашиваться
  if (btn.isSingle())                              // если единичное нажатие
    if (++psMUSIC_MODE >= MODE_AMOUNT) psMUSIC_MODE = 0;   // изменить режим

  if (btn.isHolded()) {     // кнопка удержана
    fullLowPass();
  }
#endif
}


void animation() {
  // согласно режиму
  switch (psMUSIC_MODE) {
    case 0:
      count = 0;
      for (int i = (MAX_CH - 1); i > ((MAX_CH - 1) - Rlenght); i--) {
        leds[i] = ColorFromPalette(myPal, (count * index));   // заливка по палитре " от зелёного к красному"
        count++;
      }
      count = 0;
      for (int i = (MAX_CH); i < (MAX_CH + Llenght); i++ ) {
        leds[i] = ColorFromPalette(myPal, (count * index));   // заливка по палитре " от зелёного к красному"
        count++;
      }
      if (psEMPTY_BRIGHT > 0) {
        CHSV this_dark = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
        for (int i = ((MAX_CH - 1) - Rlenght); i > 0; i--)
          leds[i] = this_dark;
        for (int i = MAX_CH + Llenght; i < NUM_LEDS; i++)
          leds[i] = this_dark;
      }
      break;
    case 1:
      if (millis() - rainbow_timer > 30) {
        rainbow_timer = millis();
        hue = floor((float)hue + psRAINBOW_STEP);
      }
      count = 0;
      for (int i = (MAX_CH - 1); i > ((MAX_CH - 1) - Rlenght); i--) {
        leds[i] = ColorFromPalette(RainbowColors_p, (count * index) / 2 - hue);  // заливка по палитре радуга
        count++;
      }
      count = 0;
      for (int i = (MAX_CH); i < (MAX_CH + Llenght); i++ ) {
        leds[i] = ColorFromPalette(RainbowColors_p, (count * index) / 2 - hue); // заливка по палитре радуга
        count++;
      }
      if (psEMPTY_BRIGHT > 0) {
        CHSV this_dark = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
        for (int i = ((MAX_CH - 1) - Rlenght); i > 0; i--)
          leds[i] = this_dark;
        for (int i = MAX_CH + Llenght; i < NUM_LEDS; i++)
          leds[i] = this_dark;
      }
      break;
    case 2:
      for (int i = 0; i < NUM_LEDS; i++) {
        if (i < STRIPE)          leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
        else if (i < STRIPE * 2) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
        else if (i < STRIPE * 3) leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
        else if (i < STRIPE * 4) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
        else if (i < STRIPE * 5) leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
      }
      break;
    case 3:
      for (int i = 0; i < NUM_LEDS; i++) {
        if (i < NUM_LEDS / 3)          leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
        else if (i < NUM_LEDS * 2 / 3) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
        else if (i < NUM_LEDS)         leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
      }
      break;
    case 4:
      switch (psFREQ_STROBE_MODE) {
        case 0:
          if (colorMusicFlash[2]) HIGHS();
          else if (colorMusicFlash[1]) MIDS();
          else if (colorMusicFlash[0]) LOWS();
          else SILENCE();
          break;
        case 1:
          if (colorMusicFlash[2]) HIGHS();
          else SILENCE();
          break;
        case 2:
          if (colorMusicFlash[1]) MIDS();
          else SILENCE();
          break;
        case 3:
          if (colorMusicFlash[0]) LOWS();
          else SILENCE();
          break;
      }
      break;
    case 5:
      if (strobe_bright > 0)
        for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(STROBE_COLOR, STROBE_SAT, strobe_bright);
      else
        for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
      break;
    case 6:
      switch (psLIGHT_MODE) {
        case 0: for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(psLIGHT_COLOR, psLIGHT_SAT, 255);
          break;
        case 1:
          if (millis() - color_timer > psCOLOR_SPEED) {
            color_timer = millis();
            if (++this_color > 255) this_color = 0;
          }
          for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(this_color, psLIGHT_SAT, 255);
          break;
        case 2:
          if (millis() - rainbow_timer > 30) {
            rainbow_timer = millis();
            this_color += psRAINBOW_PERIOD;
            if (this_color > 255) this_color = 0;
            if (this_color < 0) this_color = 255;
          }
          rainbow_steps = this_color;
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV((int)floor(rainbow_steps), 255, 255);
            rainbow_steps += psRAINBOW_STEP_2;
            if (rainbow_steps > 255) rainbow_steps = 0;
            if (rainbow_steps < 0) rainbow_steps = 255;
          }
          break;
      }
      break;
    case 7:
      switch (psFREQ_STROBE_MODE) {
        case 0:
          if (running_flag[2]) leds[NUM_LEDS / 2] = CHSV(HIGH_COLOR, 255, thisBright[2]);
          else if (running_flag[1]) leds[NUM_LEDS / 2] = CHSV(MID_COLOR, 255, thisBright[1]);
          else if (running_flag[0]) leds[NUM_LEDS / 2] = CHSV(LOW_COLOR, 255, thisBright[0]);
          else leds[NUM_LEDS / 2] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
          break;
        case 1:
          if (running_flag[2]) leds[NUM_LEDS / 2] = CHSV(HIGH_COLOR, 255, thisBright[2]);
          else leds[NUM_LEDS / 2] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
          break;
        case 2:
          if (running_flag[1]) leds[NUM_LEDS / 2] = CHSV(MID_COLOR, 255, thisBright[1]);
          else leds[NUM_LEDS / 2] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
          break;
        case 3:
          if (running_flag[0]) leds[NUM_LEDS / 2] = CHSV(LOW_COLOR, 255, thisBright[0]);
          else leds[NUM_LEDS / 2] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
          break;
      }
      leds[(NUM_LEDS / 2) - 1] = leds[NUM_LEDS / 2];
      if (millis() - running_timer > psRUNNING_SPEED) {
        running_timer = millis();
        for (int i = 0; i < NUM_LEDS / 2 - 1; i++) {
          leds[i] = leds[i + 1];
          leds[NUM_LEDS - i - 1] = leds[i];
        }
      }
      break;
    case 8:
      byte HUEindex = psHUE_START;
      for (int i = 0; i < NUM_LEDS / 2; i++) {
        byte this_bright = map(freq_f[(int)floor((NUM_LEDS / 2 - i) / freq_to_stripe)], 0, freq_max_f, 0, 255);
        this_bright = constrain(this_bright, 0, 255);
        leds[i] = CHSV(HUEindex, 255, this_bright);
        leds[NUM_LEDS - i - 1] = leds[i];
        HUEindex += psHUE_STEP;
        if (HUEindex > 255) HUEindex = 0;
      }
      break;
  }
}

#if REMOTE_TYPE != 0
void remoteTick() {
  if (IRLremote.available())  {
    auto data = IRLremote.read();
    IRdata = data.command;
    Serial.print("0x");
    Serial.println(data.command, HEX);
    /* respond to button */

    if (!Protocol) {
      Protocol = 1;                                        // update the values to the newest valid input
      ir_flag = true;
    }
  }

  if (powerOff && IRdata != IR_Key_Power)
    return;

  if (Protocol) {
    bool isSignalCorrect = true;

    if (ir_flag) { // если данные пришли
      eeprom_timer = millis();
      eeprom_flag = true;
      switch (IRdata) {
#if IR_Key_Power
        case IR_Key_Power :   ////////////////////////////////////////////////////////////////////////// Выключить/включить гирлянду
          powerOff = !powerOff;
          if (powerOff) {
            LEDS.setBrightness(0);
          } else {
            LEDS.setBrightness(psMAX_BRIGHT);
          }
          break;
#endif
        // return to CristmasMusic
        case IR_Key_Rotate:
          isChristmasLight = true;
          SetMode(0);
          break;
        // режимы
        case BUTT_1: psMUSIC_MODE = 0;
          break;
        case BUTT_2: psMUSIC_MODE = 1;
          break;
        case BUTT_3: psMUSIC_MODE = 2;
          break;
        case BUTT_4: psMUSIC_MODE = 3;
          break;
        case BUTT_5: psMUSIC_MODE = 4;
          break;
       // case BUTT_6: psMUSIC_MODE = 5;
        //  break;
        case BUTT_7: psMUSIC_MODE = 6;
          break;
        case BUTT_8: psMUSIC_MODE = 7;
          break;
        case BUTT_9: psMUSIC_MODE = 8;
          break;
        case BUTT_0: fullLowPass();
          break;
        case BUTT_STAR: ONstate = !ONstate; FastLED.clear(); FastLED.show(); updateEEPROM();
          break;
        case BUTT_HASH:
          switch (psMUSIC_MODE) {
            case 4:
            case 7: if (++psFREQ_STROBE_MODE > 3) psFREQ_STROBE_MODE = 0;
              break;
            case 6: if (++psLIGHT_MODE > 2) psLIGHT_MODE = 0;
              break;
          }
          break;
        case BUTT_OK: digitalWrite(MLED_PIN, settings_mode ^ MLED_ON); settings_mode = !settings_mode;
          break;
        case BUTT_UP:
          if (settings_mode) {
            // ВВЕРХ общие настройки
            psEMPTY_BRIGHT = smartIncr(psEMPTY_BRIGHT, 5, 0, 255);
          } else {
            switch (psMUSIC_MODE) {
              case 0:
                break;
              case 1: psRAINBOW_STEP = smartIncrFloat(psRAINBOW_STEP, 0.5, 0.5, 20);
                break;
              case 2:
              case 3:
              case 4: psMAX_COEF_FREQ = smartIncrFloat(psMAX_COEF_FREQ, 0.1, 0, 5);
                break;
              case 5: psSTROBE_PERIOD = smartIncr(psSTROBE_PERIOD, 20, 1, 1000);
                break;
              case 6:
                switch (psLIGHT_MODE) {
                  case 0: psLIGHT_SAT = smartIncr(psLIGHT_SAT, 20, 0, 255);
                    break;
                  case 1: psLIGHT_SAT = smartIncr(psLIGHT_SAT, 20, 0, 255);
                    break;
                  case 2: psRAINBOW_STEP_2 = smartIncrFloat(psRAINBOW_STEP_2, 0.5, 0.5, 10);
                    break;
                }
                break;
              case 7: psMAX_COEF_FREQ = smartIncrFloat(psMAX_COEF_FREQ, 0.1, 0.0, 10);
                break;
              case 8: psHUE_START = smartIncr(psHUE_START, 10, 0, 255);
                break;
            }
          }
          break;
        case BUTT_DOWN:
          if (settings_mode) {
            // ВНИЗ общие настройки
            psEMPTY_BRIGHT = smartIncr(psEMPTY_BRIGHT, -5, 0, 255);
          } else {
            switch (psMUSIC_MODE) {
              case 0:
                break;
              case 1: psRAINBOW_STEP = smartIncrFloat(psRAINBOW_STEP, -0.5, 0.5, 20);
                break;
              case 2:
              case 3:
              case 4: psMAX_COEF_FREQ = smartIncrFloat(psMAX_COEF_FREQ, -0.1, 0, 5);
                break;
              case 5: psSTROBE_PERIOD = smartIncr(psSTROBE_PERIOD, -20, 1, 1000);
                break;
              case 6:
                switch (psLIGHT_MODE) {
                  case 0: psLIGHT_SAT = smartIncr(psLIGHT_SAT, -20, 0, 255);
                    break;
                  case 1: psLIGHT_SAT = smartIncr(psLIGHT_SAT, -20, 0, 255);
                    break;
                  case 2: psRAINBOW_STEP_2 = smartIncrFloat(psRAINBOW_STEP_2, -0.5, 0.5, 10);
                    break;
                }
                break;
              case 7: psMAX_COEF_FREQ = smartIncrFloat(psMAX_COEF_FREQ, -0.1, 0.0, 10);
                break;
              case 8: psHUE_START = smartIncr(psHUE_START, -10, 0, 255);
                break;
            }
          }
          break;
        case BUTT_LEFT:
          if (settings_mode) {
            // ВЛЕВО общие настройки
            psMAX_BRIGHT = smartIncr(psMAX_BRIGHT, -20, 0, 255);
            FastLED.setBrightness(psMAX_BRIGHT);
          } else {
            switch (psMUSIC_MODE) {
              case 0:
              case 1: psSMOOTH = smartIncrFloat(psSMOOTH, -0.05, 0.05, 1);
                break;
              case 2:
              case 3:
              case 4: psSMOOTH_FREQ = smartIncrFloat(psSMOOTH_FREQ, -0.05, 0.05, 1);
                break;
              case 5: psSTROBE_SMOOTH = smartIncr(psSTROBE_SMOOTH, -20, 0, 255);
                break;
              case 6:
                switch (psLIGHT_MODE) {
                  case 0: psLIGHT_COLOR = smartIncr(psLIGHT_COLOR, -10, 0, 255);
                    break;
                  case 1: psCOLOR_SPEED = smartIncr(psCOLOR_SPEED, -10, 0, 255);
                    break;
                  case 2: psRAINBOW_PERIOD = smartIncr(psRAINBOW_PERIOD, -1, -20, 20);
                    break;
                }
                break;
              case 7: psRUNNING_SPEED = smartIncr(psRUNNING_SPEED, -10, 1, 255);
                break;
              case 8: psHUE_STEP = smartIncr(psHUE_STEP, -1, 1, 255);
                break;
            }
          }
          break;
        case BUTT_RIGHT:
          if (settings_mode) {
            // ВПРАВО общие настройки
            psMAX_BRIGHT = smartIncr(psMAX_BRIGHT, 20, 0, 255);
            FastLED.setBrightness(psMAX_BRIGHT);
          } else {
            switch (psMUSIC_MODE) {
              case 0:
              case 1: psSMOOTH = smartIncrFloat(psSMOOTH, 0.05, 0.05, 1);
                break;
              case 2:
              case 3:
              case 4: psSMOOTH_FREQ = smartIncrFloat(psSMOOTH_FREQ, 0.05, 0.05, 1);
                break;
              case 5: psSTROBE_SMOOTH = smartIncr(psSTROBE_SMOOTH, 20, 0, 255);
                break;
              case 6:
                switch (psLIGHT_MODE) {
                  case 0: psLIGHT_COLOR = smartIncr(psLIGHT_COLOR, 10, 0, 255);
                    break;
                  case 1: psCOLOR_SPEED = smartIncr(psCOLOR_SPEED, 10, 0, 255);
                    break;
                  case 2: psRAINBOW_PERIOD = smartIncr(psRAINBOW_PERIOD, 1, -20, 20);
                    break;
                }
                break;
              case 7: psRUNNING_SPEED = smartIncr(psRUNNING_SPEED, 10, 1, 255);
                break;
              case 8: psHUE_STEP = smartIncr(psHUE_STEP, 1, 1, 255);
                break;
            }
          }
          break;
        default:  isSignalCorrect = false; eeprom_flag = false;   // если не распознали кнопку, не обновляем настройки!
          break;
      }
      if (isSignalCorrect)
        playTone();

      Protocol = 0;
      ir_flag = false;
#if LOG_ON == 1
      Serial.println(psMUSIC_MODE);
#endif
    }
  }
}
#endif

void mainLoop() {
  // главный цикл отрисовки
  if (ONstate) {
    if (millis() - main_timer > MAIN_LOOP) {
      // сбрасываем значения
      RsoundLevel = 0;
      LsoundLevel = 0;

      // перваые два режима - громкость (VU meter)
      if (psMUSIC_MODE == 0 || psMUSIC_MODE == 1) {
        for (byte i = 0; i < 100; i ++) {                                 // делаем 100 измерений
          RcurrentLevel = analogRead(SOUND_R);                            // с правого
          if (!MONO) LcurrentLevel = analogRead(SOUND_L);                 // и левого каналов

          if (RsoundLevel < RcurrentLevel) RsoundLevel = RcurrentLevel;   // ищем максимальное
          if (!MONO) if (LsoundLevel < LcurrentLevel) LsoundLevel = LcurrentLevel;   // ищем максимальное
        }

        // фильтруем по нижнему порогу шумов
        RsoundLevel = map(RsoundLevel, LOW_PASS, 1023, 0, 500);
        if (!MONO)LsoundLevel = map(LsoundLevel, LOW_PASS, 1023, 0, 500);

        // ограничиваем диапазон
        RsoundLevel = constrain(RsoundLevel, 0, 500);
        if (!MONO)LsoundLevel = constrain(LsoundLevel, 0, 500);

        // возводим в степень (для большей чёткости работы)
        RsoundLevel = pow(RsoundLevel, EXP);
        if (!MONO)LsoundLevel = pow(LsoundLevel, EXP);

        // фильтр
        RsoundLevel_f = RsoundLevel * psSMOOTH + RsoundLevel_f * (1 - psSMOOTH);
        if (!MONO)LsoundLevel_f = LsoundLevel * psSMOOTH + LsoundLevel_f * (1 - psSMOOTH);

        if (MONO) LsoundLevel_f = RsoundLevel_f;  // если моно, то левый = правому

        // заливаем "подложку", если яркость достаточная
        if (psEMPTY_BRIGHT > 5) {
          for (int i = 0; i < NUM_LEDS; i++)
            leds[i] = CHSV(psEMPTY_COLOR, 255, psEMPTY_BRIGHT);
        }

        // если значение выше порога - начинаем самое интересное
        if (RsoundLevel_f > 15 && LsoundLevel_f > 15) {

          // расчёт общей средней громкости с обоих каналов, фильтрация.
          // Фильтр очень медленный, сделано специально для автогромкости
          averageLevel = (float)(RsoundLevel_f + LsoundLevel_f) / 2 * averK + averageLevel * (1 - averK);

          // принимаем максимальную громкость шкалы как среднюю, умноженную на некоторый коэффициент MAX_COEF
          maxLevel = (float)averageLevel * MAX_COEF;

          // преобразуем сигнал в длину ленты (где MAX_CH это половина количества светодиодов)
          Rlenght = map(RsoundLevel_f, 0, maxLevel, 0, MAX_CH);
          Llenght = map(LsoundLevel_f, 0, maxLevel, 0, MAX_CH);

          // ограничиваем до макс. числа светодиодов
          Rlenght = constrain(Rlenght, 0, MAX_CH);
          Llenght = constrain(Llenght, 0, MAX_CH);

          animation();       // отрисовать
        }
      }

      // 3-5 режим - цветомузыка
      if (psMUSIC_MODE == 2 || psMUSIC_MODE == 3 || psMUSIC_MODE == 4 || psMUSIC_MODE == 7 || psMUSIC_MODE == 8) {
        analyzeAudio();
        colorMusic[0] = 0;
        colorMusic[1] = 0;
        colorMusic[2] = 0;
        for (int i = 0 ; i < 32 ; i++) {
          if (fht_log_out[i] < psSPEKTR_LOW_PASS) fht_log_out[i] = 0;
        }
        // низкие частоты, выборка со 2 по 5 тон (0 и 1 зашумленные!)
        for (byte i = 2; i < 6; i++) {
          if (fht_log_out[i] > colorMusic[0]) colorMusic[0] = fht_log_out[i];
        }
        // средние частоты, выборка с 6 по 10 тон
        for (byte i = 6; i < 11; i++) {
          if (fht_log_out[i] > colorMusic[1]) colorMusic[1] = fht_log_out[i];
        }
        // высокие частоты, выборка с 11 по 31 тон
        for (byte i = 11; i < 32; i++) {
          if (fht_log_out[i] > colorMusic[2]) colorMusic[2] = fht_log_out[i];
        }
        freq_max = 0;
        for (byte i = 0; i < 30; i++) {
          if (fht_log_out[i + 2] > freq_max) freq_max = fht_log_out[i + 2];
          if (freq_max < 5) freq_max = 5;

          if (freq_f[i] < fht_log_out[i + 2]) freq_f[i] = fht_log_out[i + 2];
          if (freq_f[i] > 0) freq_f[i] -= LIGHT_SMOOTH;
          else freq_f[i] = 0;
        }
        freq_max_f = freq_max * averK + freq_max_f * (1 - averK);
        for (byte i = 0; i < 3; i++) {
          colorMusic_aver[i] = colorMusic[i] * averK + colorMusic_aver[i] * (1 - averK);  // общая фильтрация
          colorMusic_f[i] = colorMusic[i] * psSMOOTH_FREQ + colorMusic_f[i] * (1 - psSMOOTH_FREQ);      // локальная
          if (colorMusic_f[i] > ((float)colorMusic_aver[i] * psMAX_COEF_FREQ)) {
            thisBright[i] = 255;
            colorMusicFlash[i] = true;
            running_flag[i] = true;
          } else colorMusicFlash[i] = false;
          if (thisBright[i] >= 0) thisBright[i] -= psSMOOTH_STEP;
          if (thisBright[i] < psEMPTY_BRIGHT) {
            thisBright[i] = psEMPTY_BRIGHT;
            running_flag[i] = false;
          }
        }
        animation();
      }
      if (psMUSIC_MODE == 5) {
        if ((long)millis() - strobe_timer > psSTROBE_PERIOD) {
          strobe_timer = millis();
          strobeUp_flag = true;
          strobeDwn_flag = false;
        }
        if ((long)millis() - strobe_timer > light_time) {
          strobeDwn_flag = true;
        }
        if (strobeUp_flag) {                    // если настало время пыхнуть
          if (strobe_bright < 255)              // если яркость не максимальная
            strobe_bright += psSTROBE_SMOOTH;     // увелчить
          if (strobe_bright > 255) {            // если пробили макс. яркость
            strobe_bright = 255;                // оставить максимум
            strobeUp_flag = false;              // флаг опустить
          }
        }

        if (strobeDwn_flag) {                   // гаснем
          if (strobe_bright > 0)                // если яркость не минимальная
            strobe_bright -= psSTROBE_SMOOTH;     // уменьшить
          if (strobe_bright < 0) {              // если пробили мин. яркость
            strobeDwn_flag = false;
            strobe_bright = 0;                  // оставить 0
          }
        }
        animation();
      }
      if (psMUSIC_MODE == 6) animation();

      if (!IRLremote.receiving())    // если на ИК приёмник не приходит сигнал (без этого НЕ РАБОТАЕТ!)
        FastLED.show();         // отправить значения на ленту

      if (psMUSIC_MODE != 7)       // 7 режиму не нужна очистка!!!
        FastLED.clear();          // очистить массив пикселей
      main_timer = millis();    // сбросить таймер
    }
  }
}

void colorMusicLoop() {
  buttonTick();     // опрос и обработка кнопки

#if REMOTE_TYPE != 0
  remoteTick();     // опрос ИК пульта
#endif

  mainLoop();       // главный цикл обработки и отрисовки
  eepromTick();     // проверка не пора ли сохранить настройки
}

void initColorMusic() {
  /*#if defined(__AVR_ATmega32U4__)   //Выключение светодиодов на Pro Micro
    TXLED1;                           //на ProMicro выключим и TXLED
    delay (1000);                     //При питании по usb от компьютера нужна задержка перед выключением RXLED. Если питать от БП, то можно убрать эту строку.
    #endif*/
  pinMode(MLED_PIN, OUTPUT);        //Режим пина для светодиода режима на выход
  digitalWrite(MLED_PIN, !MLED_ON); //Выключение светодиода режима

  pinMode(POT_GND, OUTPUT);
  digitalWrite(POT_GND, LOW);
#if USE_BTN
  btn.setTimeout(900);
#endif

  // для увеличения точности уменьшаем опорное напряжение,
  // выставив EXTERNAL и подключив Aref к выходу 3.3V на плате через делитель
  // GND ---[10-20 кОм] --- REF --- [10 кОм] --- 3V3
  // в данной схеме GND берётся из А0 для удобства подключения
  if (POTENT) analogReference(EXTERNAL);
  else
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    analogReference(INTERNAL1V1);
#else
    analogReference(INTERNAL);
#endif

  // жуткая магия, меняем частоту оцифровки до 18 кГц
  // команды на ебучем ассемблере, даже не спрашивайте, как это работает
  // поднимаем частоту опроса аналогового порта до 38.4 кГц, по теореме
  // Котельникова (Найквиста) частота дискретизации будет 19.2 кГц
  // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  if (RESET_SETTINGS) updateEEPROM_RESET();        // сброс флага настроек

  if (AUTO_LOW_PASS && !EEPROM_LOW_PASS) {         // если разрешена автонастройка нижнего порога шумов
    autoLowPass();
  }
  if (EEPROM_LOW_PASS) {                // восстановить значения шумов из памяти
    readEEPROM_LOW_PASS();
    readEEPROM_SPEKTR_LOW_PASS();
  }
}
