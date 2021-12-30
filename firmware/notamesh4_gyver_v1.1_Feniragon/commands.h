#ifndef COMMANDS_H
#define COMMANDS_H

//здесь находятся команды обрабатываемые скетчем
//формат такой
// #define IR_Key_XXXX   YYYYY
// где IR_Key_XXXX - код команды, это лучше не менять
//     YYYYY -Код клавиши пульта или аналоговой клавиатуры
//            Если кнопка не используется, то тогда надо либо поставить код 0, либо закоментировать строку
//                Например #define IR_Key_XXXX   0xff4ab5
//                или так  #define IR_Key_XXXX   KEY_0


#if false
//IRremote 3.2.0

// кнопка на пульте

#define IR_Key_Power                        0x84C88F27  // power                    // выключить и включить гирлянду, а не просто послылать цвет черный. 
#define IR_Key_Brightness_plus              0x1D6570FF  // ▲                       // Увеличить максимальную яркость 
#define IR_Key_Brightness_minus             0xF9E068E9  // ▼                       // Уменьшить максимальную яркость 
#define IR_Key_Reset                        0x79A86627  // open/close               // Сброс всех настроек и перезагрузка 
#define IR_Key_Demo                         0xD5E1C4B7  // +                        // Включит демо режим (перебор) 
#define IR_Key_Demo_Random                  0x34BD8C8B  // -                        // Включит демо режим (Случайно) 
#define IR_Key_Demo_MyMode                  0xB0FB6A2B  // #                        // Включит демо режим (перебор) 
#define IR_Key_Demo_MyMode_Random           0x6EA9D52F  // ♭                        // Включит демо режим (Случайно) 

#define IR_Key_Length_Garland_plus          0         // L/R                    // Увеличить количество светодиодов в гирлянде 
#define IR_Key_Length_Garland_minus         0         // Slow                   // Уменьшить количество светодиодов в гирлянде
#define IR_Key_Rotate                       0xEF8B0B67  // repeat                   // Сменить направление движения эффектов
#define IR_Key_Speed_minus                  0x97E5E3EB  // <<                       // Замедлить движение
#define IR_Key_Speed_plus                   0x285A35E7  // >>                       // Ускорить движение

#define IR_Key_Glitter                      0x34EF0A47  // search song              // Включить/выключить сверкание
#define IR_Key_BackGround                   0xA0ACA54B  // cancel                   // Включить/выключить заполнение фона
#define IR_Key_Candle                       0xB9327ED   // reserve                  // Включить/выключить свечки

#define IR_Key_Previous_mode                0x7932B41D  // ◄                        // Предыдущий эффект. Отключает демонстрационный режим
#define IR_Key_Next_mode                    0x328B792F  // ►                        // Следующий эффект. Отключает демонстрационный режим
#define IR_Key_OK                           0x34B48EF9  // Ok


//Вариант выбора используя кнопки 0-9 и +10  кнопками 0-9 меняются только единицы числа, а кнопкой +10 увеличиваются десятки числа
#define IR_Key_SetMode_X0                   0                                   // Набор номера выбор эффекта единицы кнопка 0
#define IR_Key_SetMode_X1                   0                                   // Набор номера выбор эффекта единицы кнопка 1
#define IR_Key_SetMode_X2                   0                                   // Набор номера выбор эффекта единицы кнопка 2
#define IR_Key_SetMode_X3                   0                                   // Набор номера выбор эффекта единицы кнопка 3
#define IR_Key_SetMode_X4                   0                                   // Набор номера выбор эффекта единицы кнопка 4
#define IR_Key_SetMode_X5                   0                                   // Набор номера выбор эффекта единицы кнопка 5
#define IR_Key_SetMode_X6                   0                                   // Набор номера выбор эффекта единицы кнопка 6
#define IR_Key_SetMode_X7                   0                                   // Набор номера выбор эффекта единицы кнопка 7
#define IR_Key_SetMode_X8                   0                                   // Набор номера выбор эффекта единицы кнопка 8
#define IR_Key_SetMode_X9                   0                                   // Набор номера выбор эффекта единицы кнопка 9
#define IR_Key_SetMode_1X                   0                                   // Набор номера выбор эффекта десядки кнопка +10

//Вариант выбора используя кнопки 0-9  кнопками 0-9 число сдвигается на разряд и добавляется единица, соответствующая номеру кнопки
#define IR_Key_SetMode_0                    0x8E6AE48F  //0                         // Набор номера выбор эффекта сдвиг + кнопка 0
#define IR_Key_SetMode_1                    0xE26AD627  //1                         // Набор номера выбор эффекта сдвиг + кнопка 1
#define IR_Key_SetMode_2                    0xD35010AB  //2                         // Набор номера выбор эффекта сдвиг + кнопка 2
#define IR_Key_SetMode_3                    0x5EA8B3C7  //3                         // Набор номера выбор эффекта сдвиг + кнопка 3
#define IR_Key_SetMode_4                    0x3C9B626B  //4                         // Набор номера выбор эффекта сдвиг + кнопка 4
#define IR_Key_SetMode_5                    0xDB41B243  //5                         // Набор номера выбор эффекта сдвиг + кнопка 5
#define IR_Key_SetMode_6                    0xF98E2CCB  //6                         // Набор номера выбор эффекта сдвиг + кнопка 6
#define IR_Key_SetMode_7                    0x2C040D07  //7                         // Набор номера выбор эффекта сдвиг + кнопка 7
#define IR_Key_SetMode_8                    0x72DF3667  //8                         // Набор номера выбор эффекта сдвиг + кнопка 8
#define IR_Key_SetMode_9                    0x911E9403  //9                         // Набор номера выбор эффекта сдвиг + кнопка 9

#define IR_Key_Mode_0                       0                                   //  Эффект 0
#define IR_Key_Mode_1                       0                                   //  Эффект 1
#define IR_Key_Mode_2                       0                                   //  Эффект 2
#define IR_Key_Mode_3                       0                                   //  Эффект 3
#define IR_Key_Mode_4                       0                                   //  Эффект 4
#define IR_Key_Mode_5                       0                                   //  Эффект 5
#define IR_Key_Mode_6                       0                                   //  Эффект 6
#define IR_Key_Mode_7                       0                                   //  Эффект 7
#define IR_Key_Mode_8                       0                                   //  Эффект 8
#define IR_Key_Mode_9                       0                                   //  Эффект 9
#define IR_Key_Mode_10                      0                                    //  Эффект 10
#define IR_Key_Mode_11                      0                                    //  Эффект 11
#define IR_Key_Mode_12                      0                                    //  Эффект 12
#define IR_Key_Mode_13                      0                                    //  Эффект 13
#define IR_Key_Mode_14                      0                                    //  Эффект 14
#define IR_Key_Mode_15                      0                                    //  Эффект 15
#define IR_Key_Mode_16                      0                                    //  Эффект 16
#define IR_Key_Mode_17                      0                                    //  Эффект 17
#define IR_Key_Mode_18                      0                                    //  Эффект 18
#define IR_Key_Mode_19                      0                                    //  Эффект 19
#define IR_Key_Mode_20                      0                                    //  Эффект 20
#define IR_Key_Mode_21                      0                                    //  Эффект 21
#define IR_Key_Mode_22                      0                                    //  Эффект 22
#define IR_Key_Mode_23                      0                                    //  Эффект 23
#define IR_Key_Mode_24                      0                                    //  Эффект 24
#define IR_Key_Mode_25                      0                                    //  Эффект 25
#define IR_Key_Mode_26                      0                                    //  Эффект 26
#define IR_Key_Mode_27                      0                                    //  Эффект 27
#define IR_Key_Mode_28                      0                                    //  Эффект 28
#define IR_Key_Mode_29                      0                                    //  Эффект 29
#define IR_Key_Mode_30                      0                                    //  Эффект 30
#define IR_Key_Mode_31                      0                                    //  Эффект 31
#define IR_Key_Mode_32                      0                                    //  Эффект 32
#define IR_Key_Mode_33                      0                                    //  Эффект 33
#define IR_Key_Mode_34                      0                                    //  Эффект 34
#define IR_Key_Mode_35                      0                                    //  Эффект 35
#define IR_Key_Mode_36                      0                                    //  Эффект 36
#define IR_Key_Mode_37                      0                                    //  Эффект 37
#define IR_Key_Mode_38                      0                                    //  Эффект 38
#define IR_Key_Mode_39                      KEY_0                                //  Эффект 39
#define IR_Key_Mode_40                      0                                    //  Эффект 40
#define IR_Key_Mode_41                      0                                    //  Эффект 40

#define IR_Key_Show_ModeId                  0x3FE3630B    // DISK/SKIP              // Гирлянда проморгает номер эффекта
#define IR_Key_Save_Mode                    0x93E354A3    // audio                  // Сохранить эффект как запускающийся первым
#define IR_Key_Delay_minus                  0x35714D89    // highlight              // Уменьшить задержку на 100ms (используя белые светодиоды)
#define IR_Key_Delay_plus                   0xEB94A8DF    // dimmer                 // Увеличить задержку на 100ms (используя белые светодиоды)

#define IR_Key_Palette_Stop                 0x9D385503  // ■                        // Остановка вращения палитры        Остановить перебор Палитр
#define IR_Key_Palette_Previous             0x83768BCB  // |◄◄                     // Выберите предыдущую палитру       Установить предыдущую палитру
#define IR_Key_Palette_Next                 0x29C933C7  // ►►|                     // Выберите следующую палитру        Установить следующую палитру
#define IR_Key_Palette_Auto                 0x4938636B  // ► ||                     // Включить поворот палитры          Продолжить автоматический перебор палитр

#define IR_Key_Solid_Black                  0  //                        //  Установить цвет Черный  (Выключает гирлянду, Например перед тем как сказать елочка зажгись на детском утреннике)
#define IR_Key_Solid_Red                    0  // >>                     //  Установить цвет Красный
#define IR_Key_Solid_Orange                 0  // |<<                    //  Установить цвет Оранжевый
#define IR_Key_Solid_Yellow                 0  // >>|                    //  Установить цвет Желтый
#define IR_Key_Solid_Green                  0  // |>                     //  Установить цвет Зеленый
#define IR_Key_Solid_SkyBlue                0  // ||                     //  Установить цвет Голубой
#define IR_Key_Solid_Blue                   0  // []                     //  Установить цвет Синий
#define IR_Key_Solid_Violet                 0  // ><                     //  Установить цвет Фиолетовый
#define IR_Key_Solid_White                  0  // Zoom                   //  Установить цвет Белый


#else
#if false

//IRremote 3.4.0

// кнопка на пульте
#define IR_Key_Power                        0x380  // power                    // выключить и включить гирлянду, а не просто послылать цвет черный. 
#define IR_Key_Brightness_plus              0x31A  // ▲                       // Увеличить максимальную яркость 
#define IR_Key_Brightness_minus             0x31D  // ▼                       // Уменьшить максимальную яркость 
#define IR_Key_Reset                        0x394  // open/close               // Сброс всех настроек и перезагрузка 
#define IR_Key_Demo                         0x301  // +                        // Включит демо режим (перебор) 
#define IR_Key_Demo_Random                  0x3CC  // -                        // Включит демо режим (Случайно) 
#define IR_Key_Demo_MyMode                  0x3AC  // #                        // Включит демо режим (перебор) 
#define IR_Key_Demo_MyMode_Random           0x364  // ♭                        // Включит демо режим (Случайно) 

#define IR_Key_Length_Garland_plus          0         // L/R                    // Увеличить количество светодиодов в гирлянде 
#define IR_Key_Length_Garland_minus         0         // Slow                   // Уменьшить количество светодиодов в гирлянде
#define IR_Key_Rotate                       0x3EC  // repeat                   // Сменить направление движения эффектов
#define IR_Key_Speed_minus                  0x3FC  // <<                       // Замедлить движение
#define IR_Key_Speed_plus                   0x3F8  // >>                       // Ускорить движение

#define IR_Key_Glitter                      0x398  // search song              // Включить/выключить сверкание
#define IR_Key_BackGround                   0x3F0  // cancel                   // Включить/выключить заполнение фона
#define IR_Key_Candle                       0x304   // reserve                  // Включить/выключить свечки

#define IR_Key_Previous_mode                0x321  // ◄                        // Предыдущий эффект. Отключает демонстрационный режим
#define IR_Key_Next_mode                    0x324  // ►                        // Следующий эффект. Отключает демонстрационный режим
#define IR_Key_OK                           0x34B48EF9  // Ok                        

//Вариант выбора используя кнопки 0-9 и +10  кнопками 0-9 меняются только единицы числа, а кнопкой +10 увеличиваются десятки числа
#define IR_Key_SetMode_X0                   0                                   // Набор номера выбор эффекта единицы кнопка 0
#define IR_Key_SetMode_X1                   0                                   // Набор номера выбор эффекта единицы кнопка 1
#define IR_Key_SetMode_X2                   0                                   // Набор номера выбор эффекта единицы кнопка 2
#define IR_Key_SetMode_X3                   0                                   // Набор номера выбор эффекта единицы кнопка 3
#define IR_Key_SetMode_X4                   0                                   // Набор номера выбор эффекта единицы кнопка 4
#define IR_Key_SetMode_X5                   0                                   // Набор номера выбор эффекта единицы кнопка 5
#define IR_Key_SetMode_X6                   0                                   // Набор номера выбор эффекта единицы кнопка 6
#define IR_Key_SetMode_X7                   0                                   // Набор номера выбор эффекта единицы кнопка 7
#define IR_Key_SetMode_X8                   0                                   // Набор номера выбор эффекта единицы кнопка 8
#define IR_Key_SetMode_X9                   0                                   // Набор номера выбор эффекта единицы кнопка 9
#define IR_Key_SetMode_1X                   0                                   // Набор номера выбор эффекта десядки кнопка +10

//Вариант выбора используя кнопки 0-9  кнопками 0-9 число сдвигается на разряд и добавляется единица, соответствующая номеру кнопки
#define IR_Key_SetMode_0                    0x3C8  //0                         // Набор номера выбор эффекта сдвиг + кнопка 0
#define IR_Key_SetMode_1                    0x3C4  //1                         // Набор номера выбор эффекта сдвиг + кнопка 1
#define IR_Key_SetMode_2                    0x3C0  //2                         // Набор номера выбор эффекта сдвиг + кнопка 2
#define IR_Key_SetMode_3                    0x3A4  //3                         // Набор номера выбор эффекта сдвиг + кнопка 3
#define IR_Key_SetMode_4                    0x3B8  //4                         // Набор номера выбор эффекта сдвиг + кнопка 4
#define IR_Key_SetMode_5                    0x39C  //5                         // Набор номера выбор эффекта сдвиг + кнопка 5
#define IR_Key_SetMode_6                    0x3A0  //6                         // Набор номера выбор эффекта сдвиг + кнопка 6
#define IR_Key_SetMode_7                    0x3B0  //7                         // Набор номера выбор эффекта сдвиг + кнопка 7
#define IR_Key_SetMode_8                    0x3D0  //8                         // Набор номера выбор эффекта сдвиг + кнопка 8
#define IR_Key_SetMode_9                    0x3B4  //9                         // Набор номера выбор эффекта сдвиг + кнопка 9

#define IR_Key_Mode_0                       0                                   //  Эффект 0
#define IR_Key_Mode_1                       0                                   //  Эффект 1
#define IR_Key_Mode_2                       0                                   //  Эффект 2
#define IR_Key_Mode_3                       0                                   //  Эффект 3
#define IR_Key_Mode_4                       0                                   //  Эффект 4
#define IR_Key_Mode_5                       0                                   //  Эффект 5
#define IR_Key_Mode_6                       0                                   //  Эффект 6
#define IR_Key_Mode_7                       0                                   //  Эффект 7
#define IR_Key_Mode_8                       0                                   //  Эффект 8
#define IR_Key_Mode_9                       0                                   //  Эффект 9
#define IR_Key_Mode_10                      0                                    //  Эффект 10
#define IR_Key_Mode_11                      0                                    //  Эффект 11
#define IR_Key_Mode_12                      0                                    //  Эффект 12
#define IR_Key_Mode_13                      0                                    //  Эффект 13
#define IR_Key_Mode_14                      0                                    //  Эффект 14
#define IR_Key_Mode_15                      0                                    //  Эффект 15
#define IR_Key_Mode_16                      0                                    //  Эффект 16
#define IR_Key_Mode_17                      0                                    //  Эффект 17
#define IR_Key_Mode_18                      0                                    //  Эффект 18
#define IR_Key_Mode_19                      0                                    //  Эффект 19
#define IR_Key_Mode_20                      0                                    //  Эффект 20
#define IR_Key_Mode_21                      0                                    //  Эффект 21
#define IR_Key_Mode_22                      0                                    //  Эффект 22
#define IR_Key_Mode_23                      0                                    //  Эффект 23
#define IR_Key_Mode_24                      0                                    //  Эффект 24
#define IR_Key_Mode_25                      0                                    //  Эффект 25
#define IR_Key_Mode_26                      0                                    //  Эффект 26
#define IR_Key_Mode_27                      0                                    //  Эффект 27
#define IR_Key_Mode_28                      0                                    //  Эффект 28
#define IR_Key_Mode_29                      0                                    //  Эффект 29
#define IR_Key_Mode_30                      0                                    //  Эффект 30
#define IR_Key_Mode_31                      0                                    //  Эффект 31
#define IR_Key_Mode_32                      0                                    //  Эффект 32
#define IR_Key_Mode_33                      0                                    //  Эффект 33
#define IR_Key_Mode_34                      0                                    //  Эффект 34
#define IR_Key_Mode_35                      0                                    //  Эффект 35
#define IR_Key_Mode_36                      0                                    //  Эффект 36
#define IR_Key_Mode_37                      0                                    //  Эффект 37
#define IR_Key_Mode_38                      0                                    //  Эффект 38
#define IR_Key_Mode_39                      KEY_0                                //  Эффект 39
#define IR_Key_Mode_40                      0                                    //  Эффект 40
#define IR_Key_Mode_41                      0                                    //  Эффект 40

#define IR_Key_Show_ModeId                  0x388    // DISK/SKIP              // Гирлянда проморгает номер эффекта
#define IR_Key_Save_Mode                    0x384    // audio                  // Сохранить эффект как запускающийся первым
#define IR_Key_Delay_minus                  0x317    // highlight              // Уменьшить задержку на 100ms (используя белые светодиоды)
#define IR_Key_Delay_plus                   0x334    // dimmer                 // Увеличить задержку на 100ms (используя белые светодиоды)

#define IR_Key_Palette_Stop                 0x3E4  // ■                        // Остановка вращения палитры        Остановить перебор Палитр
#define IR_Key_Palette_Previous             0x3D8  // |◄◄                     // Выберите предыдущую палитру       Установить предыдущую палитру
#define IR_Key_Palette_Next                 0x3DC  // ►►|                     // Выберите следующую палитру        Установить следующую палитру
#define IR_Key_Palette_Auto                 0x3E8  // ► ||                     // Включить поворот палитры          Продолжить автоматический перебор палитр

#define IR_Key_Solid_Black                  0  //                        //  Установить цвет Черный  (Выключает гирлянду, Например перед тем как сказать елочка зажгись на детском утреннике)
#define IR_Key_Solid_Red                    0  // >>                     //  Установить цвет Красный
#define IR_Key_Solid_Orange                 0  // |<<                    //  Установить цвет Оранжевый
#define IR_Key_Solid_Yellow                 0  // >>|                    //  Установить цвет Желтый
#define IR_Key_Solid_Green                  0  // |>                     //  Установить цвет Зеленый
#define IR_Key_Solid_SkyBlue                0  // ||                     //  Установить цвет Голубой
#define IR_Key_Solid_Blue                   0  // []                     //  Установить цвет Синий
#define IR_Key_Solid_Violet                 0  // ><                     //  Установить цвет Фиолетовый
#define IR_Key_Solid_White                  0  // Zoom                   //  Установить цвет Белый

#else 

//IRLremoTE

// кнопка на пульте
#define IR_Key_Power                        0xB17286F9  // power                    // выключить и включить гирлянду, а не просто послылать цвет черный. 
#define IR_Key_Brightness_plus              0x7AFC4FF9  // ▲                       // Увеличить максимальную яркость 
#define IR_Key_Brightness_minus             0x8CBA9F9   // ▼                       // Уменьшить максимальную яркость 
#define IR_Key_Reset                        0x2D5E59F9  // open/close               // Сброс всех настроек и перезагрузка 
#define IR_Key_Demo                         0xD5B19032  // +                        // Включит демо режим (перебор) 
#define IR_Key_Demo_Random                  0x3C764B32  // -                        // Включит демо режим (Случайно) 
#define IR_Key_Demo_MyMode                  0x638861F9  // ^                        // Включит демо режим (перебор) 
#define IR_Key_Demo_MyMode_Random           0xFA4964F9  // ♭                        // Включит демо режим (Случайно) 

#define IR_Key_Length_Garland_plus          0         // L/R                    // Увеличить количество светодиодов в гирлянде 
#define IR_Key_Length_Garland_minus         0         // Slow                   // Уменьшить количество светодиодов в гирлянде
#define IR_Key_Rotate                       0xBDF5D2F9  // any key                   // Сменить направление движения эффектов
#define IR_Key_Speed_minus                  0xCF40ADF9  // |<<                       // Замедлить движение
#define IR_Key_Speed_plus                   0x6DC229F9  // >>|                       // Ускорить движение

#define IR_Key_Glitter                      0x881847F9  // menu              // Включить/выключить сверкание
#define IR_Key_BackGround                   0x451EC4F9  // play list                   // Включить/выключить заполнение фона
#define IR_Key_Candle                       0xEA64D6F9   // title list                  // Включить/выключить свечки

#define IR_Key_Previous_mode                0x9C77E7F9  // ◄                        // Предыдущий эффект. Отключает демонстрационный режим
#define IR_Key_Next_mode                    0x5B6E4F9   // ►                        // Следующий эффект. Отключает демонстрационный режим
#define IR_Key_OK                           0x996322F9  // Ok

//Вариант выбора используя кнопки 0-9  кнопками 0-9 число сдвигается на разряд и добавляется единица, соответствующая номеру кнопки
#define IR_Key_SetMode_0                    0xA33C70F9  //0                         // Набор номера выбор эффекта сдвиг + кнопка 0
#define IR_Key_SetMode_1                    0xA027ABF9  //1                         // Набор номера выбор эффекта сдвиг + кнопка 1
#define IR_Key_SetMode_2                    0x36E8AEF9  //2                         // Набор номера выбор эффекта сдвиг + кнопка 2
#define IR_Key_SetMode_3                    0x33D3E9F9  //3                         // Набор номера выбор эффекта сдвиг + кнопка 3
#define IR_Key_SetMode_4                    0xD919FBF9  //4                         // Набор номера выбор эффекта сдвиг + кнопка 4
#define IR_Key_SetMode_5                    0xD60536F9  //5                         // Набор номера выбор эффекта сдвиг + кнопка 5
#define IR_Key_SetMode_6                    0x6CC639F9  //6                         // Набор номера выбор эффекта сдвиг + кнопка 6
#define IR_Key_SetMode_7                    0x69B174F9  //7                         // Набор номера выбор эффекта сдвиг + кнопка 7
#define IR_Key_SetMode_8                    0xF780CEF9  //8                         // Набор номера выбор эффекта сдвиг + кнопка 8
#define IR_Key_SetMode_9                    0xF46C09F9  //9                         // Набор номера выбор эффекта сдвиг + кнопка 9

#define IR_Key_Show_ModeId                  0x5DDC38F9    // info             // Гирлянда проморгает номер эффекта
#define IR_Key_Save_Mode                    0xC46BD9F9    // rec                  // Сохранить эффект как запускающийся первым
#define IR_Key_Delay_minus                  0xA6B479F9    // cancel              // Уменьшить задержку на 100ms (используя белые светодиоды)
#define IR_Key_Delay_plus                   0x3B946FF9    // repeat                 // Увеличить задержку на 100ms (используя белые светодиоды)

#define IR_Key_Palette_Stop                 0x4832CF9  // ■                        // Остановка вращения палитры        Остановить перебор Палитр
#define IR_Key_Palette_Previous             0x3EA934F9  // ◄◄                     // Выберите предыдущую палитру       Установить предыдущую палитру
#define IR_Key_Palette_Next                 0xD25572F9  // ►►                     // Выберите следующую палитру        Установить следующую палитру
#define IR_Key_Palette_Auto                 0x70D6EEF9  // ►||                     // Включить поворот палитры          Продолжить автоматический перебор палитр

#define IR_Key_Solid_Black                  0  //                        //  Установить цвет Черный  (Выключает гирлянду, Например перед тем как сказать елочка зажгись на детском утреннике)
#define IR_Key_Solid_Red                    0  // >>                     //  Установить цвет Красный
#define IR_Key_Solid_Orange                 0  // |<<                    //  Установить цвет Оранжевый
#define IR_Key_Solid_Yellow                 0  // >>|                    //  Установить цвет Желтый
#define IR_Key_Solid_Green                  0  // |>                     //  Установить цвет Зеленый
#define IR_Key_Solid_SkyBlue                0  // ||                     //  Установить цвет Голубой
#define IR_Key_Solid_Blue                   0  // []                     //  Установить цвет Синий
#define IR_Key_Solid_Violet                 0  // ><                     //  Установить цвет Фиолетовый
#define IR_Key_Solid_White                  0  // Zoom                   //  Установить цвет Белый

#endif
#endif

#define REMOTE_TYPE 3       // 0 - без пульта, 1 - пульт от WAVGAT, 2 - пульт от KEYES, 3 - кастомный пульт
// система может работать С ЛЮБЫМ ИК ПУЛЬТОМ (практически). Коды для своего пульта можно задать начиная со строки 160 в прошивке. Коды пультов определяются скетчем IRtest_2.0, читай инструкцию

// ----- КНОПКИ ПУЛЬТА WAVGAT -----
#if REMOTE_TYPE == 1
#define BUTT_UP     0xF39EEBAD
#define BUTT_DOWN   0xC089F6AD
#define BUTT_LEFT   0xE25410AD
#define BUTT_RIGHT  0x14CE54AD
#define BUTT_OK     0x297C76AD
#define BUTT_1      0x4E5BA3AD
#define BUTT_2      0xE51CA6AD
#define BUTT_3      0xE207E1AD
#define BUTT_4      0x517068AD
#define BUTT_5      0x1B92DDAD
#define BUTT_6      0xAC2A56AD
#define BUTT_7      0x5484B6AD
#define BUTT_8      0xD22353AD
#define BUTT_9      0xDF3F4BAD
#define BUTT_0      0xF08A26AD
#define BUTT_STAR   0x68E456AD
#define BUTT_HASH   0x151CD6AD
#endif

// ----- КНОПКИ ПУЛЬТА KEYES -----
#if REMOTE_TYPE == 2
#define BUTT_UP     0xE51CA6AD
#define BUTT_DOWN   0xD22353AD
#define BUTT_LEFT   0x517068AD
#define BUTT_RIGHT  0xAC2A56AD
#define BUTT_OK     0x1B92DDAD
#define BUTT_1      0x68E456AD
#define BUTT_2      0xF08A26AD
#define BUTT_3      0x151CD6AD
#define BUTT_4      0x18319BAD
#define BUTT_5      0xF39EEBAD
#define BUTT_6      0x4AABDFAD
#define BUTT_7      0xE25410AD
#define BUTT_8      0x297C76AD
#define BUTT_9      0x14CE54AD
#define BUTT_0      0xC089F6AD
#define BUTT_STAR   0xAF3F1BAD
#define BUTT_HASH   0x38379AD
#endif

// ----- КНОПКИ СВОЕГО ПУЛЬТА -----
#if REMOTE_TYPE == 3
#define BUTT_UP     IR_Key_Brightness_plus
#define BUTT_DOWN   IR_Key_Brightness_minus
#define BUTT_LEFT   IR_Key_Previous_mode
#define BUTT_RIGHT  IR_Key_Next_mode
#define BUTT_OK     IR_Key_OK
#define BUTT_1      IR_Key_SetMode_1
#define BUTT_2      IR_Key_SetMode_2
#define BUTT_3      IR_Key_SetMode_3
#define BUTT_4      IR_Key_SetMode_4
#define BUTT_5      IR_Key_SetMode_5
#define BUTT_6      IR_Key_SetMode_6
#define BUTT_7      IR_Key_SetMode_7
#define BUTT_8      IR_Key_SetMode_8
#define BUTT_9      IR_Key_SetMode_9
#define BUTT_0      IR_Key_SetMode_0
#define BUTT_STAR   IR_Key_Demo_MyMode_Random  // *
#define BUTT_HASH   IR_Key_Demo_MyMode   // #
#endif

#endif //COMMANDS_H
