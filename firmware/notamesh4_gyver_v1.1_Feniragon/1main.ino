#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define NOTAMESH_VERSION 103                                  // Just a continuation of seirlight and previously aalight.

#include "commands.h"                                         // The commands.

//pins definitions
#include "pins.h"
#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 0                       // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#include "FastLED.h"                                          // https://github.com/FastLED/FastLEDCRGB leds[NUM_LEDS];

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

const byte COLOR_MUSIC_MODE = 42;
CRGB leds[NUM_LEDS];

CRGBPalette16 gCurrentPalette;                               // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 gTargetPalette;                                // Also support smooth palette transitioning
CRGB solid = CRGB::Black;                                    // Цвет закраски

#if  IR_ON == 1                                               // This is included with base install
#include "PinChangeInterrupt.h"
#include "IRLremote.h"
CHashIR IRLremote;
uint32_t IRdata;
#endif


#if USE_BTN == 1
#include "GyverButton.h"
GButton btn(BTN_PIN);
#endif

#if KEY_ON == 1                                                 //Для аналоговых кнопок
int key_input = 0;                                            //Последнее нажатие кнопки
int key_input_new;                                            //только что пришедьшее нажатие кнопки
bool key_bounce = 0;                                          //для антидребезга
uint32_t key_time;                                            //время последнего нажатия
#endif

#if ( IR_ON == 1 || KEY_ON == 1 || USE_BTN == 1 )
uint8_t  IR_New_Mode = 0;                                      //Выбор эффекта
uint32_t IR_Time_Mode = 0;                                     //время последнего нажатия
#endif

// Serial definition
#define SERIAL_BAUDRATE 57600                                 // Or 115200.

extern const TProgmemRGBGradientPalettePtr gGradientPalettes[]; // These are for the fixed palettes in gradient_palettes.h
extern const uint8_t gGradientPaletteCount;                     // Total number of fixed palettes to display.
uint8_t gCurrentPaletteNumber = 0;                              // Current palette number from the 'playlist' of color palettes
uint8_t currentPatternIndex = 0;                                // Index number of which pattern is current
uint32_t demo_time = 0;                                         // время демо режима

TBlendType currentBlending = LINEARBLEND;                     // NOBLEND or LINEARBLEND


#if CHANGE_ON == 1
uint8_t newMode = 0;                                        // номер нового режима
uint16_t StepMode = NUM_LEDS;                             // Текущий шаг перехода от нового режима до старого
#endif

uint16_t KolLed;

uint8_t demorun = DEMO_MODE;                                    // 0 = regular mode, 1 = demo mode, 2 = shuffle mode.
#define maxMode  42                                              // Maximum number of modes.

uint8_t Protocol = 0;                                         // Temporary variables to save latest IR input
uint32_t Command = 0;

// Общие переменные ----------------------------------------------------------------------
uint8_t allfreq = 32;                                         // Меняет частоту. Переменная для эффектов one_sin_pal и two_sin.
uint8_t bgclr = 0;                                            // Общий цвет фона. Переменная для эффектов matrix_pal и one_sin_pal.
uint8_t bgbri = 0;                                            // Общая фоновая яркость. Переменная для эффектов matrix_pal и one_sin_pal.
bool    glitter = GLITER_ON;                                  // Флаг включения блеска
bool    background = BACKGR_ON;                               // Флаг включения заполнения фона
#if CANDLE_KOL >0
bool    candle = CANDLE_ON;                                 // Флаг включения свечей
#endif
uint8_t palchg = 3;                                           // Управление палитрой  3 - менять палитру автоматически иначе нет
uint8_t startindex = 0;                                       // С какого цвета начинать. Переменная для эффектов one_sin_pal.
uint8_t thisbeat;                                             // Переменная для эффектов juggle_pal.
uint8_t thiscutoff = 192;                                     // Если яркость ниже этой, то яркость = 0. Переменная для эффектов one_sin_pal и two_sin.
uint8_t thisdiff = 1;                                         // Шаг изменения палитры. Переменная для эффектов confetti_pal, juggle_pal и rainbow_march.
int8_t  thisdir = 1;                                          // Направление движения эффекта. принимает значение -1 или 1.
uint8_t thisfade = 224;                                       // Скорость затухания. Переменная для эффектов confetti_pal и juggle_pal.
uint8_t thishue = 0;                                          // Оттенок Переменная для эффектов two_sin.
uint8_t thisindex = 0;                                        // Указатель ан элемент палитры
uint8_t thisinc = 1;                                          // Изменение начального цвета после каждого прохода. Переменная для эффектов confetti_pal и one_sin_pal.
int     thisphase = 0;                                        // Изменение фазы. Переменная для эффектов one_sin_pal, plasma и two_sin.
uint8_t thisrot = 1;                                          // Измение скорости вращения волны. В настоящее время 0.
int8_t  thisspeed = 4;                                        // Изменение стандартной скорости
uint8_t wavebright = 255;                                     // Вы можете изменить яркость волн / полос, катящихся по экрану.

bool powerOff = false;
bool isChristmasLight = true;

#if MY_MODE
const PROGMEM uint8_t my_mode[] = { MY_MODE };            //массив выбранных режимов
const uint8_t my_mode_count = sizeof( my_mode );          //колличество выбрано режимов
uint8_t tek_my_mode = 0;                                  //Указатель на текущий режим
#endif

#if AVAILABLE_MODES
const PROGMEM uint8_t available_mode[] = { AVAILABLE_MODES };    //массив выбранных режимов
const uint8_t available_mode_count = sizeof( available_mode );  //колличество выбрано режимов
uint8_t tek_available_mode = 0;                                  //Указатель на текущий режим
#endif

#if CHANGE_SPARK == 4
uint8_t rand_spark = 0;
#endif

long summ = 0;

void strobe_mode(uint8_t newMode, bool mc);
void bootme();
void meshwait();
void getirl();
void demo_check();



//main variables for EEPROM
#include "persistentVar.h"

// Display functions -----------------------------------------------------------------------

// Support functions
#include "addings.h"

// Display routines
#include "confetti_pal.h"
#include "gradient_palettes.h"
#include "juggle_pal.h"
#include "matrix_pal.h"
#include "noise16_pal.h"
#include "noise8_pal.h"
#include "one_sin_pal.h"
#include "plasma.h"
#include "rainbow_march.h"
#include "rainbow_beat.h"
#include "serendipitous_pal.h"
#include "three_sin_pal.h"
#include "two_sin.h"
#include "blendwave.h"
#include "fire.h"
#include "candles.h"
#include "colorwave.h"
#include "getirl.h"
#if COLOR_MUSIC_AVAILABLE
#include "colorMusic.h"
#endif



/*------------------------------------------------------------------------------------------
  --------------------------------------- Start of code --------------------------------------
  ------------------------------------------------------------------------------------------*/
void setup() {


#if BUZZER_ON == 1
  pinMode(BUZZER_PIN, OUTPUT);
#endif

#if KEY_ON == 1
  pinMode(PIN_KEY, INPUT);                                                        //Для аналоговых кнопок
#endif

#if (LOG_ON == 1 || SETTINGS_LOG == 1)
  Serial.begin(SERIAL_BAUDRATE);                                                  // Setup serial baud rate

  Serial.println(F(" ")); Serial.println(F("---SETTING UP---"));
#endif
  delay(1000);                                                                    // Slow startup so we can re-upload in the case of errors.

  if (POWER_I > 0)
    FastLED.setMaxPowerInVoltsAndMilliamps(POWER_V, POWER_I);                     //Настройка блока питания

  random16_set_seed(4832);                                                        // Awesome randomizer of awesomeness
  random16_add_entropy(analogRead(2));


#if IR_ON == 1
  if (!IRLremote.begin(PIN_IR))
    Serial.println(F("You did not choose a valid pin."));
#endif

  setup_persistency();

  LEDS.setBrightness(psMAX_BRIGHT);                                                 // Set the generic maximum brightness value.

#if LED_CK
  LEDS.addLeds<CHIPSET, LED_DT, LED_CK, COLOR_ORDER>(leds, MAX_LEDS);
  //   FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

#else
  LEDS.addLeds<CHIPSET, LED_DT, COLOR_ORDER >(leds, NUM_LEDS);                   //Для светодиодов WS2812B
#endif



#if BLACKSTART == 1
  solid = CRGB::Black;                 //Запуск с пустого поля
  newMode = psLED_MODE;
  psLED_MODE = 100;
  StepMode = 1;
#else
#if MY_MODE
  switch (demorun)  {
    case 3:   psLED_MODE = pgm_read_byte(my_mode + tek_my_mode); break;            // демо 3
    case 4:   psLED_MODE = pgm_read_byte(my_mode + random8(my_mode_count)); break;   // демо 4
  }
#endif

#endif
  gCurrentPalette = gGradientPalettes[0];
  gTargetPalette = gGradientPalettes[0];
  strobe_mode(psLED_MODE, 1);                                                        // Initialize the first sequence
  newMode = psLED_MODE;

#if LOG_ON == 1
  if (DEMO_MODE) {
    Serial.print(F("DEMO MODE "));
    Serial.println(DEMO_MODE);
  }
  Serial.println(F("---SETUP COMPLETE---"));
#endif
#if COLOR_MUSIC_AVAILABLE
  initColorMusic();
#endif

} // setup()


bool onFlag = true;
//------------------MAIN LOOP---------------------------------------------------------------
void loop() {
#if COLOR_MUSIC_AVAILABLE
  if (!isChristmasLight) {
    colorMusicLoop();
    return;
  }
#endif

#if (USE_BTN == 1)
  static bool stepFlag = false;
  static bool brightDir = true;
  btn.tick();
  if (btn.isSingle()) {
    onFlag = !onFlag;
    FastLED.setBrightness(onFlag ? psMAX_BRIGHT : 0);
    FastLED.show();
  }
  if (btn.isDouble()) {
#if MY_MODE
    if (++tek_my_mode >= (my_mode_count - 1)) tek_my_mode = 0;
    newMode = pgm_read_byte(my_mode + tek_my_mode);
#else
    if (++newMode >= maxMode) newMode = 0;
#endif
    SetMode(newMode);
  }
  if (btn.isTriple()) {
#if MY_MODE
    if (--tek_my_mode <= 0) tek_my_mode = my_mode_count - 1;
    newMode = pgm_read_byte(my_mode + tek_my_mode);
#else
    if (--newMode <= 0) newMode = maxMode;
#endif
    SetMode(newMode);
  }
  if (btn.hasClicks()) {
    if (btn.getClicks() == 4) glitter = !glitter;
  }
  if (stepFlag && btn.isRelease()) {
    stepFlag = false;
    brightDir = !brightDir;
  }
  if (btn.isStep()) {
    stepFlag = true;
    psMAX_BRIGHT += (brightDir ? 20 : -20);
    psMAX_BRIGHT = constrain(psMAX_BRIGHT, 0, 255);
    FastLED.setBrightness(psMAX_BRIGHT);
  }
#endif

#if ( IR_ON == 1 || KEY_ON == 1 || USE_BTN == 1 )
  getirl();                                                                   // Обработка команд с пульта и аналоговых кнопок
#endif

  if (onFlag) {
    demo_check();                                                                 // Работа если включен демонстрационный режим

    EVERY_N_MILLISECONDS(50) {                                                  // Плавный переход от одной палитры в другую
      uint8_t maxChanges = 24;
#if CHANGE_ON == 1
      if (StepMode == NUM_LEDS )
#endif
        nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, maxChanges);
    }

#if PALETTE_TIME>0
    if (palchg) {
      EVERY_N_SECONDS(PALETTE_TIME) {                                            // Смена палитры
        if (palchg == 3) {
          if (gCurrentPaletteNumber < (gGradientPaletteCount - 1))  gCurrentPaletteNumber++;
          else                                                    gCurrentPaletteNumber = 0;
#if LOG_ON == 1
          Serial.print(F("New Palette: "));  Serial.println(gCurrentPaletteNumber);
#endif
        }
        gTargetPalette = gGradientPalettes[gCurrentPaletteNumber];                // We're just ensuring that the gTargetPalette WILL be assigned.
      }
    }
#endif

#if DIRECT_TIME > 0
    EVERY_N_SECONDS(DIRECT_TIME) {                                            // Меняем направление
      thisdir = thisdir * -1;
    }
#endif

    EVERY_N_MILLIS_I(thistimer, psTHIS_DELAY) {                                    // Sets the original delay time.
      thistimer.setPeriod(psTHIS_DELAY);                                           // This is how you update the delay value on the fly.
      KolLed = NUM_LEDS;                                                        // Выводим Эффект на все светодиоды
#if CHANGE_ON == 1
      if (StepMode < NUM_LEDS) {                                                // требуется наложить новый эффект
        KolLed = StepMode;                                                      // Выводим Эффект на все светодиоды
        if (StepMode > 10) psLED_MODE = newMode;                                  // отобразить э
#if CHANGE_SPARK == 4
        sparkler(rand_spark);
#else
        sparkler(CHANGE_SPARK);                                                             // бенгальский огонь
#endif
      }
#endif
      strobe_mode(psLED_MODE, 0);                                                  // отобразить эффект;
    }

#if CHANGE_ON == 1
    EVERY_N_MILLISECONDS(CHANGE_TIME * 1000 / NUM_LEDS) {                      // Движение плавной смены эффектов
      if ( StepMode < NUM_LEDS)
      { StepMode++;
        if (StepMode == 10) strobe_mode(newMode, 1);
        if (StepMode >= NUM_LEDS)
        { psLED_MODE = newMode;
          StepMode = NUM_LEDS;
#if LOG_ON == 1
          Serial.println(F("End SetMode"));
#endif
        }
        nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, NUM_LEDS);
      }
    }
#endif

    if (glitter) addglitter(10);                                                // If the glitter flag is set, let's add some.
#if CANDLE_KOL >0
    if (candle)  addcandle();
#endif

    if (background) addbackground();                                            // Включить заполнение черного цвета фоном
  }

#if KEY_ON == 1                                                             //Для аналоговых кнопок
  key_input_new = analogRead(PIN_KEY);                                      //прочитаем аналоговые кнопки
  if  ( ( ( (key_input - KEY_DELTA) > key_input_new) ||                     //Пришло новое значение отличное от прошлого
          ( (key_input + KEY_DELTA) < key_input_new) ) &&
        !key_bounce ) {                                                     // и еще ничего не приходило
    key_bounce = 1;                                                         //Начинаем обрабатывать
    key_time = millis();                                                    //Запомним время
  }
  else if (  key_bounce &&                                                    //Обрабатываем нажатия
             ((millis() - key_time) >= 50 )  ) {                               //Закончилось время дребезга
    key_bounce = 0;                                                       //Больше не обрабатываем
    key_input = key_input_new;
#if LOG_ON == 1
    Serial.print(F("Analog Key: ")); Serial.println(key_input);
#endif

#if KEY_0 >= KEY_DELTA
    if  ( ( (KEY_0 - KEY_DELTA) < key_input) &&
          ( (KEY_0 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_0
      Protocol = 1;
      Command = KEY_0;
    }
#endif
#if KEY_1 >= KEY_DELTA
    if  ( ( (KEY_1 - KEY_DELTA) < key_input) &&
          ( (KEY_1 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_1
      Protocol = 1;
      Command = KEY_1;
    }
#endif
#if KEY_2 >= KEY_DELTA
    if  ( ( (KEY_2 - KEY_DELTA) < key_input) &&
          ( (KEY_2 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_2
      Protocol = 1;
      Command = KEY_2;
    }
#endif
#if KEY_3 >= KEY_DELTA
    if  ( ( (KEY_3 - KEY_DELTA) < key_input) &&
          ( (KEY_3 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_3
      Protocol = 1;
      Command = KEY_3;
    }
#endif
#if KEY_4 >= KEY_DELTA
    if  ( ( (KEY_4 - KEY_DELTA) < key_input) &&
          ( (KEY_4 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_4
      Protocol = 1;
      Command = KEY_4;
    }
#endif
#if KEY_5 >= KEY_DELTA
    if  ( ( (KEY_5 - KEY_DELTA) < key_input) &&
          ( (KEY_5 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_5
      Protocol = 1;
      Command = KEY_5;
    }
#endif
#if KEY_6 >= KEY_DELTA
    if  ( ( (KEY_6 - KEY_DELTA) < key_input) &&
          ( (KEY_6 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_6
      Protocol = 1;
      Command = KEY_6;
    }
#endif
#if KEY_7 >= KEY_DELTA
    if  ( ( (KEY_7 - KEY_DELTA) < key_input) &&
          ( (KEY_7 + KEY_DELTA) > key_input) )  {                       //Нашли нажатую кнопку KEY_7
      Protocol = 1;
      Command = KEY_7;
    }
#endif
  }
#endif

#if ( IR_ON == 1 || KEY_ON == 1 || USE_BTN == 1 )
  if ( (IR_Time_Mode > 0) &&                                                //Идет отчет времени
       ((millis() - IR_Time_Mode) >= 2000 )  ) {                            //И прошло больше 2 секунд
    IR_Time_Mode = 0;
    if (IR_New_Mode <= maxMode) SetMode(IR_New_Mode);
    IR_New_Mode = 0;
  }
#endif

#if IR_ON == 1


  if (IRLremote.available())  {
    auto data = IRLremote.read();
    IRdata = data.command;
    //  Serial.print("0x");
    //  Serial.println(data.command, HEX);
    /* respond to button */

    if (!Protocol) {
      Protocol = 1;                                        // update the values to the newest valid input

#if IR_REPEAT == 1
      if ( IRdata != 0xffffffff)                    //Если не повтор то вставить новую команду
        Command = IRdata;
#else
      Command = IRdata;
#endif
    }

  }
#endif

  static uint32_t showTimer = 0;

  if (onFlag && millis() - showTimer >= 10) {
    showTimer = millis();
#if IR_ON == 1
    if (!IRLremote.receiving())
#endif
      FastLED.show();
  }
} // loop()


//-------------------OTHER ROUTINES----------------------------------------------------------
void strobe_mode(uint8_t mode, bool mc) {                  // mc stands for 'Mode Change', where mc = 0 is strobe the routine, while mc = 1 is change the routine

  if (mc) {
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));                // Clean up the array for the first time through. Don't show display though, so you may have a smooth transition.
#if LOG_ON == 1
    Serial.print(F("Mode: "));
    Serial.println(mode);
    //Serial.println(millis());
#endif
#if PALETTE_TIME>0
    if (palchg == 0) palchg = 3;
#else
    if (palchg == 0) palchg = 1;
#endif
  }

  // psLED_MODE = mode;
  switch (mode) {                                          // First time through a new mode, so let's initialize the variables for a given display.
    /*case  0: if (mc) {
        psTHIS_DELAY = 10;
        palchg = 0;
      } blendwave(); break;
      case  1: if (mc) {
        psTHIS_DELAY = 10;
        palchg = 0;
      } rainbow_beat(); break;*/
    case  2: if (mc) {
        psTHIS_DELAY = 10;
        allfreq = 2;
        thisspeed = 1;
        thatspeed = 2;
        thishue = 0;
        thathue = 128;
        thisdir = 1;
        thisrot = 1;
        thatrot = 1;
        thiscutoff = 128;
        thatcutoff = 192;
      } two_sin(); break;
    /* case  3: if (mc) {
         psTHIS_DELAY = 20;
         allfreq = 4;
         bgclr = 0;
         bgbri = 0;
         startindex = 64;
         thisinc = 2;
         thiscutoff = 224;
         thisphase = 0;
         thiscutoff = 224;
         thisrot = 0;
         thisspeed = 4;
         wavebright = 255;
       } one_sin_pal(); break;*/
    case  4: if (mc) {
        psTHIS_DELAY = 10;
      } noise8_pal(); break;
    /*case  5: if (mc) {
        psTHIS_DELAY = 10;
        allfreq = 4;
        thisspeed = -1;
        thatspeed = 0;
        thishue = 64;
        thathue = 192;
        thisdir = 1;
        thisrot = 0;
        thatrot = 0;
        thiscutoff = 64;
        thatcutoff = 192;
      } two_sin(); break;
      case  6: if (mc) {
        psTHIS_DELAY = 20;
        allfreq = 10;
        bgclr = 64;
        bgbri = 4;
        startindex = 64;
        thisinc = 2;
        thiscutoff = 224;
        thisphase = 0;
        thiscutoff = 224;
        thisrot = 0;
        thisspeed = 4;
        wavebright = 255;
      } one_sin_pal(); break;
      /* case  7: if (mc) {
        psTHIS_DELAY = 10;
        numdots = 2;
        thisfade = 16;
        thisbeat = 8;
        thisdiff = 64;
      } juggle_pal(); break;
      case  8: if (mc) {
        psTHIS_DELAY = 40;
        thisindex = 128;
        thisdir = 1;
        thisrot = 0;
        bgclr = 200;
        bgbri = 6;
      } matrix_pal(); break;*/
    case  9: if (mc) {
        psTHIS_DELAY = 10;
        allfreq = 6;
        thisspeed = 2;
        thatspeed = 3;
        thishue = 96;
        thathue = 224;
        thisdir = 1;
        thisrot = 0;
        thatrot = 0;
        thiscutoff = 64;
        thatcutoff = 64;
      } two_sin(); break;
    /*  case 10: if (mc) {
          psTHIS_DELAY = 20;
          allfreq = 16;
          bgclr = 0;
          bgbri = 0;
          startindex = 64;
          thisinc = 2;
          thiscutoff = 224;
          thisphase = 0;
          thiscutoff = 224;
          thisrot = 0;
          thisspeed = 4;
          wavebright = 255;
        } one_sin_pal(); break;
      case 11: if (mc) {
          psTHIS_DELAY = 50;
          mul1 = 5;
          mul2 = 8;
          mul3 = 7;
        } three_sin_pal(); break;
      case 12: if (mc) {
          psTHIS_DELAY = 10;
        } serendipitous_pal(); break;
      case 13: if (mc) {
          psTHIS_DELAY = 20;
          allfreq = 8;
          bgclr = 0;
          bgbri = 4;
          startindex = 64;
          thisinc = 2;
          thiscutoff = 224;
          thisphase = 0;
          thiscutoff = 224;
          thisrot = 0;
          thisspeed = 4;
          wavebright = 255;
        } one_sin_pal(); break;*/
    case 14: if (mc) {
        psTHIS_DELAY = 10;
        allfreq = 20;
        thisspeed = 2;
        thatspeed = -1;
        thishue = 24;
        thathue = 180;
        thisdir = 1;
        thisrot = 0;
        thatrot = 1;
        thiscutoff = 64;
        thatcutoff = 128;
      } two_sin(); break;
    /* case 15: if (mc) {
         psTHIS_DELAY = 50;
         thisindex = 64;
         thisdir = -1;
         thisrot = 1;
         bgclr = 100;
         bgbri = 10;
       } matrix_pal(); break;*/
    case 16: if (mc) {
        psTHIS_DELAY = 10;
      } noise8_pal(); break; // By: Andrew Tuline
    /* case 17: if (mc) {
         psTHIS_DELAY = 10;
       } plasma(11, 23, 4, 18); break;
      case 18: if (mc) {
         psTHIS_DELAY = 20;
         allfreq = 10;
         thisspeed = 1;
         thatspeed = -2;
         thishue = 48;
         thathue = 160;
         thisdir = -1;
         thisrot = 1;
         thatrot = -1;
         thiscutoff = 128;
         thatcutoff = 192;
       } two_sin(); break;
      case 19: if (mc) {
         psTHIS_DELAY = 50;
         palchg = 0;
         thisdir = 1;
         thisrot = 1;
         thisdiff = 1;
       } rainbow_march(); break;*/
    case 20: if (mc) {
        psTHIS_DELAY = 10;
        mul1 = 6;
        mul2 = 9;
        mul3 = 11;
      } three_sin_pal(); break;
    /*  case 21: if (mc) {
          psTHIS_DELAY = 10;
          palchg = 0;
          thisdir = 1;
          thisrot = 2;
          thisdiff = 10;
        } rainbow_march(); break;
      case 22: if (mc) {
          psTHIS_DELAY = 20;
          palchg = 0;
          hxyinc = random16(1, 15);
          octaves = random16(1, 3);
          hue_octaves = random16(1, 5);
          hue_scale = random16(10, 50);
          x = random16();
          xscale = random16();
          hxy = random16();
          hue_time = random16();
          hue_speed = random16(1, 3);
          x_speed = random16(1, 30);
        } noise16_pal(); break;
      case 23: if (mc) {
          psTHIS_DELAY = 20;
          allfreq = 6;
          bgclr = 0;
          bgbri = 0;
          startindex = 64;
          thisinc = 2;
          thiscutoff = 224;
          thisphase = 0;
          thiscutoff = 224;
          thisrot = 0;
          thisspeed = 4;
          wavebright = 255;
        } one_sin_pal(); break;
      case 24: if (mc) {
          psTHIS_DELAY = 10;
        } plasma(23, 15, 6, 7); break;
      case 25: if (mc) {
          psTHIS_DELAY = 20;
          thisinc = 1;
          thisfade = 2;
          thisdiff = 32;
        } confetti_pal(); break;*/
    case 26: if (mc) {
        psTHIS_DELAY = 10;
        thisspeed = 2;
        thatspeed = 3;
        thishue = 96;
        thathue = 224;
        thisdir = 1;
        thisrot = 1;
        thatrot = 2;
        thiscutoff = 128;
        thatcutoff = 64;
      } two_sin(); break;
    /*case 27: if (mc) {
        psTHIS_DELAY = 30;
        thisindex = 192;
        thisdir = -1;
        thisrot = 0;
        bgclr = 50;
        bgbri = 0;
      } matrix_pal(); break;
      case 28: if (mc) {
        psTHIS_DELAY = 20;
        allfreq = 20;
        bgclr = 0;
        bgbri = 0;
        startindex = 64;
        thisinc = 2;
        thiscutoff = 224;
        thisphase = 0;
        thiscutoff = 224;
        thisrot = 0;
        thisspeed = 4;
        wavebright = 255;
      } one_sin_pal(); break;
      case 29: if (mc) {
        psTHIS_DELAY = 20;
        thisinc = 2;
        thisfade = 8;
        thisdiff = 64;
      } confetti_pal(); break;
      case 30: if (mc) {
        psTHIS_DELAY = 10;
      } plasma(8, 7, 9, 13); break;
      case 31: if (mc) {
        psTHIS_DELAY = 10;
        numdots = 4;
        thisfade = 32;
        thisbeat = 12;
        thisdiff = 20;
      } juggle_pal(); break;
      case 32: if (mc) {
        psTHIS_DELAY = 30;
        allfreq = 4;
        bgclr = 64;
        bgbri = 4;
        startindex = 64;
        thisinc = 2;
        thiscutoff = 224;
        thisphase = 0;
        thiscutoff = 128;
        thisrot = 1;
        thisspeed = 8;
        wavebright = 255;
      } one_sin_pal(); break;*/
    case 33: if (mc) {
        psTHIS_DELAY = 50;
        mul1 = 3;
        mul2 = 4;
        mul3 = 5;
      } three_sin_pal(); break;
    /*case 34: if (mc) {
        psTHIS_DELAY = 10;
        palchg = 0;
        thisdir = -1;
        thisrot = 1;
        thisdiff = 5;
      } rainbow_march(); break;
      case 35: if (mc) {
        psTHIS_DELAY = 10;
      } plasma(11, 17, 20, 23); break;
      case 36: if (mc) {
        psTHIS_DELAY = 20;
        thisinc = 1;
        thisfade = 1;
      } confetti_pal(); break;
      case 37: if (mc) {
        psTHIS_DELAY = 20;
        palchg = 0;
        octaves = 1;
        hue_octaves = 2;
        hxy = 6000;
        x = 5000;
        xscale = 3000;
        hue_scale = 50;
        hue_speed = 15;
        x_speed = 100;
      } noise16_pal(); break;
      case 38: if (mc) {
        psTHIS_DELAY = 10;
      } noise8_pal(); break;*/
    case 39: if (mc) {
        psTHIS_DELAY = 10;
        palchg = 0;
      } fire(); break;
    case 40: if (mc) {
        psTHIS_DELAY = 10;
        palchg = 0;
      } candles(); break;
    case 41: if (mc) {
        psTHIS_DELAY = 10;
      } colorwaves(); break;
#if COLOR_MUSIC_AVAILABLE
    case COLOR_MUSIC_MODE: isChristmasLight = false; break;
#endif
    case 100: if (mc) {
        palchg = 0;
      } fill_solid(leds, NUM_LEDS,  solid); break;    //Установить цвет
    case 201: fill_solid(leds, NUM_LEDS, CRGB::Black); fill_solid(leds, psMESH_DELAY, CRGB(255, 255, 255)); break; //Зажеч гирлянду длинной psMESH_DELAY
    default : //psLED_MODE = 0;
      SetMode(0); break;        //нет такого режима принудительно ставим нулевой

  } // switch mode

#if LOG_ON == 1
  if (mc) {
    if ( palchg == 0 ) Serial.println(F("Change palette off"));
    else if ( palchg == 1 ) Serial.println(F("Change palette Stop"));
    else if ( palchg == 3 ) Serial.println(F("Change palette ON"));
  }
#endif

} // strobe_mode()

void demo_check() {

  if (demorun) {
    Serial.println(F("demorun"));

    if ((millis() - demo_time) >= (DEMO_TIME * 1000L )) {         //Наступило время смены эффекта
      demo_time = millis();                                       //Запомним время
      gCurrentPaletteNumber = random8(0, gGradientPaletteCount);  //Случайно поменяем палитру
#if CHANGE_ON == 1
      switch (demorun)  {
        case 2:      // демо 2
#if AVAILABLE_MODES
          newMode = pgm_read_byte(available_mode + random8(available_mode_count));
#else
          newMode = random8(0, maxMode);
          if (newMode == COLOR_MUSIC_MODE)
            newMode = 0;
#endif
          break;
#if MY_MODE
        case 3:   if (tek_my_mode >= (my_mode_count - 1)) tek_my_mode = 0; // демо 3
          else tek_my_mode++;
          newMode = pgm_read_byte(my_mode + tek_my_mode);
          break;
        case 4:   newMode = pgm_read_byte(my_mode + random8(my_mode_count));  // демо 4
          break;
#endif
        default :  // демо 1
#if AVAILABLE_MODES
          if (tek_available_mode >= (available_mode_count - 1)) tek_available_mode = 0;
          else tek_available_mode++;
          newMode = pgm_read_byte(available_mode + tek_available_mode);
#else
          if (newMode >= maxMode) newMode = 0;
          else newMode++;
          break;
          if (newMode == COLOR_MUSIC_MODE)
            newMode = 0;
#endif
      }
      StepMode = 1;
#if CHANGE_SPARK == 4
      rand_spark = random8(3) + 1;
#endif

#if LOG_ON == 1
      Serial.println(F("Start SetMode"));
#endif
#else
      gTargetPalette = gGradientPalettes[gCurrentPaletteNumber];  //Применим палитру
#if LOG_ON == 1
      Serial.print(F("New Palette: "));  Serial.println(gCurrentPaletteNumber);
#endif
      switch (demorun)  {
        case 2:      // демо 2
#if AVAILABLE_MODES
          newMode = pgm_read_byte(available_mode + random8(available_mode_count));
#else
          newMode = random8(0, maxMode);
          if (newMode == COLOR_MUSIC_MODE)
            newMode = 0;
#endif
          break;
#if MY_MODE
        case 3:   if (tek_my_mode >= (my_mode_count - 1)) tek_my_mode = 0; // демо 3
          else tek_my_mode++;
          newMode = pgm_read_byte(my_mode + tek_my_mode);
          break;
        case 4:   newMode = pgm_read_byte(my_mode + random8(my_mode_count));  // демо 4
          break;
#endif
        default :  // демо 1
#if AVAILABLE_MODES
          if (tek_available_mode >= (available_mode_count - 1)) tek_available_mode = 0;
          else tek_available_mode++;
          newMode = pgm_read_byte(available_mode + tek_available_mode);
#else
          if (newMode >= maxMode) newMode = 0;
          else newMode++;
          if (newMode == COLOR_MUSIC_MODE)
            newMode = 0;
          break;
#endif
      }
      strobe_mode(newMode, 1);                                // Does NOT reset to 0.
#if CANDLE_KOL >0
      PolCandle = random8(CANDLE_KOL);
#endif
#endif
    } // if lastSecond
  } // if demorun

} // demo_check()
