// --------  TO-DO
// - добавить 3 кнопки (увеличить уменьшить и "ок")  ✅
// - вывод из меню на экран парметра ✅
// - подключить инкремент и декремент в замисимости от параметра ✅
// - активные и неактивные стрелочки для перелистывания меню ✅
// - кидать кубик ✅
// - анимация кнопки ОК ✅
// - добавить возможность входить и выходить в меню настроек ✅
// - добавить в меню настроек : ✅
//        - яркость [ | | | | | ] auto ✅
//        - тёмная/светлая тема ✅
//        - порог сигнализации батареи ???? ✅
//        - "вернуться назад" | маленькое wiki[?] | ✅
//        - [reset] создать нового мачкино сбросив все параметры и вернуться на главный экран ✅
//        - менять иконку батареи в завистимости от происходит заряд/разряд/заряжена на 100% ✅
// - на игровом поле при выборе шестерни заполнить чем-то большой квадрат ✅
// - добавить снижение бонусов в отрицательные значения (хотя бы до - 9)✅
// - сделать режимы :
//        - игра ✅
//        - сон ✅
//        - экран низкого заряда ✅
//        - сервис меню ✅
// - переключение между режимами по кнопке [ON/OFF], в зависимости от заряда акб ✅
// - режим сна с пробуждением и замером напряжения на акб ✅
// - добавить поворот экрана на 180 град ✅
// - РАЗОБРАТЬ БАРДАК С ДЕФАЙНАМИ, ПЕРЕМЕННЫМИ, НЕЙМИНГОМ ВЕЗДЕ!!!!! ✅
//
//
//
// - Сервис меню:
//        - калибровка константы измерения напряжения на акб ✅
//        - проверка кнопок ✅
//        - проверка дисплея
//        - выход из сервис меню ✅
//        - хард ресет (всех настроек/ всех настроек и калибровочных данных/сохранений) ❗
//
// - батарея :
//        - чекать заряд ✅
//        - если заряд меньше 16% отображать на игровом поле вместо шестерёнки батарею и мигать ей ✅
//        - если заряд ниже нижнего порога заряда по настройкам (ниже 3.4 в) не давать запускать игру ✅
//        - режим низкого заряда (отображать на экране пустую батарейку и через 1,5 сек выключать экран)  ✅
//        - откалибровать соотношение заряда к процентам ❗
//        - отображать заряд в настройках по процентно
//
// - сохранять параметры манчкина и настройки в EEPROM а при перезагрузке спрашивать в настройках добавить сброс манчкина
//        - расписать карту хранения данных по адресам (ключ первого включения, калибровочные константы,
//          настройки пользователя, манчкин)✅
//        - сохранение манчкина по таймингу или после действий ✅
//        - сохранение параметров настроек после выхода из настроек ✅
//        - загрузка всех сохранённых данных перед запуском игры ✅
//        - сброс настроек на дефолтные❗
//
// - добавить анимации:
//        - бросок кубика ✅
//        - вход/выход в настройки
//        - включение/выключение экрана
//
// - РАЗОБРАТЬ БАРДАК В СЕТАПЕ, СОЗДАТЬ ПРАВИЛЬНУЮ ПОСЛЕДОВАТЕЛЬНОСТЬ ИНИЦИАЛИЗАЦИИ УСТРОЙСТВА❗❗
// - РАЗОБРАТЬ БАРДАК C ПЕРЕМЕННЫМИ, ЛОКАЛИЗИРОВАТЬ ПЕРЕМЕННЫЕ ОТНОСИТЕЛЬНО РЕЖИМОВ ПРОГРАММЫ❗❗
// - РАЗОБРАТЬ БАРДАК СО ШРИФТАМИ, УНИФИЦИРОВАТЬ❗❗
//
// - добавить звуки ❓
// - если останется память добавить пару лёгких игор(понг, змейка, динозаврик) ❓

//-------------------EEEPROM MAP------------------------------------
// кол-во памяти 1кб | адреса 0 - 1023
//
// | адрес |  тип/размер  |  дефолтное знач. |    значение
// |     0 |   1 байт     |                  | ключ первого включения( если зачение не совпадает - вписываем в EEPROM дефолтные данные)
// |       |              |                  |
// |     2 |   float/4б   |     1.1          | калибровочная константа вольтметра
// |       |              |                  |
// |     5 |   int/2б     |       0          | последнее замеренное напряжение на батарее в режиме low_battery перед отключением ❗❗❗❗❗
// |       |              |                  |
// |    10 | структура/?б |   во флэше       | место сохранений настроек ( дефолтные настройки хранятся в флеш памяти) 100б зарезервированно
// |       |              |                  |
// |   110 | структура/?б |   во флэше       | место для хранения манчкина ( дефолтные параметры хранятся в флеш памяти) 100б зарезервированно
// |       |              |                  |
// |   220 |              |                  |
// |       |              |                  |
// |       |              |                  |
//-------------------EEEPROM MAP------------------------------------

//-------------------VERSIONS LOG-----------------------------------

//---"v: 01.01.05"
//  - переменные координат/длины/ширины фигур переопределены в дефайны
//  - неймин дефайнов подтянут к верхнему регистру
//  - исправлены ошибки в нейминге переменных и констант

//-------------------VERSIONS LOG-----------------------------------

//---------------------LIBRARIES------------------------------------
#include <Arduino.h>
#include <U8g2lib.h> //для дисплея
#include <SPI.h>
#include <EEPROM.h>     // библиотека для работы со внутренней памятью ардуино
#include <gyverpower.h> // библиотека для сна

// How to Drive the SSD1306 OLED LCD display
// https://www.best-microcontroller-projects.com/ssd1306.html

//--------------------DEV CONSTANTS----------------------------------
//-version
const char program_version[] PROGMEM = "v: 01.01.05";

//----------------OLED SETUP---------------------------------------

// SPI Arduino Nano v3 Pinout
//  OLED_SCLK 13
//  OLED_MISO 12
//  OLED_MOSI 11
//  OLED_SS 10

#define OLED_CLK 13   // SCK
#define OLED_MOSI 11  // SDA
#define OLED_RESET 10 // RES
#define OLED_DC A2    // DC
#define OLED_CS A3    // CS

// U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 8);

// U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 8, /* dc=*/ 9, /* reset=*/ 10); // ~50fps
// U8G2_SSD1306_128X64_NONAME_2_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 8, /* dc=*/ 9, /* reset=*/ 10); // ~76fps

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/OLED_CS, /* dc=*/OLED_DC, /* reset=*/OLED_RESET); // ~105fps 1Kb RAM

//---- DISPLAY ENTITY COORDINATES -----

//--- BIG BOX------
#define WIDTH_BIG_BOX 44
#define HEIGHT_BIG_BOX 44
#define RADIUS_BIG_BOX 4

//---LEFT BIG BOX
#define X_LEFT_BIG_BOX 19
#define Y_LEFT_BIG_BOX 19

//--- RIGHT BIG BOX
#define X_RIGHT_BIG_BOX 65
#define Y_RIGHT_BIG_BOX 19

//--- small box-------
#define Y_SMALL_BOX 2
#define WIDTH_SMALL_BOX 14
#define HEIGHT_SMALL_BOX 14
#define RADIUS_SMALL_BOX 3
//--- small long box--
#define WIDTH_SMALL_LONG_BOX 30

//--- small LVL BOX
#define X_LVL_SMALL_BOX 1

//--- small GENDER BOX---
#define X_GENDER_SMALL_BOX 17

//--- small long BONUS BOX
#define X_BONUS_SMALL_BOX 33

//--- small long POWER BOX
#define X_POWER_SMALL_BOX 65

//--- small DICE BOX
#define X_DICE_SMALL_BOX 97

//--- small MENU BOX
#define X_MENU_SMALL_BOX 113

//--- SETTINGS BOX
#define SETTINGS_FRAME_X 18
#define SETTINGS_FRAME_Y 1
#define SETTINGS_FRAME_WIDTH 92
#define SETTINGS_FRAME_HEIGHT 62
#define SETTINGS_FRAME_RADIUS 4
//---- DISPLAY ENTITY COORDINATES -----

//--- BITMAPS --------------------------
#define BIG_ICON_WIDTH 40
#define BIG_ICON_HEIGHT 40

static const unsigned char female_cyrillic_bits[] U8X8_PROGMEM = {
    0xFC,
    0x01,
    0xFF,
    0x80,
    0x3F,
    0xFC,
    0x01,
    0xFF,
    0x80,
    0x3F,
    0xFC,
    0x01,
    0xFF,
    0x80,
    0x3F,
    0xF0,
    0x03,
    0x7E,
    0xC0,
    0x0F,
    0xF0,
    0x03,
    0x7E,
    0xC0,
    0x0F,
    0xF0,
    0x03,
    0x7E,
    0xC0,
    0x0F,
    0xE0,
    0x07,
    0x7E,
    0xE0,
    0x07,
    0xE0,
    0x07,
    0x7E,
    0xE0,
    0x07,
    0xE0,
    0x07,
    0x7E,
    0xE0,
    0x07,
    0xC0,
    0x0F,
    0x7E,
    0xF0,
    0x03,
    0xC0,
    0x0F,
    0x7E,
    0xF0,
    0x03,
    0xC0,
    0x0F,
    0x7E,
    0xF0,
    0x03,
    0x80,
    0x1F,
    0x7E,
    0xF8,
    0x01,
    0x80,
    0x1F,
    0x7E,
    0xF8,
    0x01,
    0x80,
    0x1F,
    0x7E,
    0xF8,
    0x01,
    0x00,
    0x3F,
    0x7E,
    0xFC,
    0x00,
    0x00,
    0x3F,
    0x7E,
    0xFC,
    0x00,
    0x00,
    0x3F,
    0x7E,
    0xFC,
    0x00,
    0x00,
    0xFC,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0xFC,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0xFC,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0xFC,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0x3F,
    0x7E,
    0xFC,
    0x00,
    0x00,
    0x3F,
    0x7E,
    0xFC,
    0x00,
    0x00,
    0x3F,
    0x7E,
    0xFC,
    0x00,
    0x80,
    0x1F,
    0x7E,
    0xF8,
    0x01,
    0x80,
    0x1F,
    0x7E,
    0xF8,
    0x01,
    0x80,
    0x1F,
    0x7E,
    0xF8,
    0x01,
    0xC0,
    0x0F,
    0x7E,
    0xF0,
    0x03,
    0xC0,
    0x0F,
    0x7E,
    0xF0,
    0x03,
    0xC0,
    0x0F,
    0x7E,
    0xF0,
    0x03,
    0xE0,
    0x07,
    0x7E,
    0xE0,
    0x07,
    0xE0,
    0x07,
    0x7E,
    0xE0,
    0x07,
    0xE0,
    0x07,
    0x7E,
    0xE0,
    0x07,
    0xF0,
    0x03,
    0x7E,
    0xC0,
    0x0F,
    0xF0,
    0x03,
    0x7E,
    0xC0,
    0x0F,
    0xF0,
    0x03,
    0x7E,
    0xC0,
    0x0F,
    0xFC,
    0x01,
    0xFF,
    0x80,
    0x3F,
    0xFC,
    0x01,
    0xFF,
    0x80,
    0x3F,
    0xFC,
    0x01,
    0xFF,
    0x80,
    0x3F,
};

static const unsigned char male_cyrillic_bits[] U8X8_PROGMEM = {
    0xf8, 0x01, 0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x80, 0x1f, 0xf8, 0x03,
    0x00, 0xc0, 0x1f, 0xf8, 0x03, 0x00, 0xc0, 0x1f, 0xf8, 0x07, 0x00, 0xe0,
    0x1f, 0xf8, 0x07, 0x00, 0xe0, 0x1f, 0xf8, 0x0f, 0x00, 0xf0, 0x1f, 0xf8,
    0x0f, 0x00, 0xf0, 0x1f, 0xf8, 0x1f, 0x00, 0xf8, 0x1f, 0xf8, 0x1f, 0x00,
    0xf8, 0x1f, 0xf8, 0x1f, 0x00, 0xf8, 0x1f, 0xf8, 0x3f, 0x00, 0xfc, 0x1f,
    0xf8, 0x3f, 0x00, 0xfc, 0x1f, 0xf8, 0x7f, 0x00, 0xfe, 0x1f, 0xf8, 0x7f,
    0x00, 0xfe, 0x1f, 0xf8, 0xff, 0x00, 0xff, 0x1f, 0xf8, 0xff, 0x00, 0xff,
    0x1f, 0xf8, 0xfd, 0x81, 0xbf, 0x1f, 0xf8, 0xfd, 0x81, 0xbf, 0x1f, 0xf8,
    0xf9, 0x81, 0x9f, 0x1f, 0xf8, 0xf9, 0xc3, 0x9f, 0x1f, 0xf8, 0xf9, 0xc3,
    0x9f, 0x1f, 0xf8, 0xf1, 0xe7, 0x8f, 0x1f, 0xf8, 0xf1, 0xe7, 0x8f, 0x1f,
    0xf8, 0xe1, 0xff, 0x87, 0x1f, 0xf8, 0xe1, 0xff, 0x87, 0x1f, 0xf8, 0xc1,
    0xff, 0x83, 0x1f, 0xf8, 0xc1, 0xff, 0x83, 0x1f, 0xf8, 0xc1, 0xff, 0x83,
    0x1f, 0xf8, 0x81, 0xff, 0x81, 0x1f, 0xf8, 0x81, 0xff, 0x81, 0x1f, 0xf8,
    0x01, 0xff, 0x80, 0x1f, 0xf8, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x01, 0x7e,
    0x80, 0x1f, 0xf8, 0x01, 0x7e, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x80, 0x1f,
    0xf8, 0x01, 0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x80, 0x1f, 0xf8, 0x01,
    0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x80, 0x1f};

#define SMALL_ICON_WIDTH 16
#define SMALL_ICON_HEIGHT 16

static const unsigned char small_female_cyrillic_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x19, 0x98, 0x19, 0xb0, 0x0d,
    0xb0, 0x0d, 0xe0, 0x07, 0xe0, 0x07, 0xb0, 0x0d, 0xb0, 0x0d, 0x98, 0x19,
    0x98, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const unsigned char small_male_cyrillic_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x38, 0x1c, 0x38, 0x1c,
    0x78, 0x1e, 0xf8, 0x1f, 0xf8, 0x1f, 0xd8, 0x1b, 0x98, 0x19, 0x18, 0x18,
    0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const unsigned char small_bous_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0xf0, 0x0c, 0xf0, 0x0b,
    0xf8, 0x1f, 0x98, 0x19, 0x98, 0x19, 0x38, 0x1c, 0x70, 0x0e, 0x70, 0x0e,
    0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const unsigned char small_power_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x14, 0x00, 0x0a,
    0x00, 0x05, 0x90, 0x02, 0x60, 0x01, 0xc0, 0x00, 0xa0, 0x00, 0x18, 0x01,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const unsigned char small_dice_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x30, 0x0c,
    0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x30, 0x0c, 0x30, 0x0c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const unsigned char small_menu_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0xd0, 0x0b, 0x20, 0x04,
    0x98, 0x19, 0x50, 0x0a, 0x50, 0x0a, 0x98, 0x19, 0x20, 0x04, 0xd0, 0x0b,
    0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define SCREEN_BUTTON_WIDTH 8
#define SCREEN_BUTTON_HEIGHT 9

static const unsigned char O_bits[] U8X8_PROGMEM = {
    0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00};

static const unsigned char K_bits[] U8X8_PROGMEM = {
    0x00, 0x24, 0x24, 0x14, 0x0c, 0x14, 0x24, 0x24, 0x00};

static const unsigned char LEFT_bits[] U8X8_PROGMEM = {
    0x20, 0x30, 0x38, 0x3c, 0x3e, 0x3c, 0x38, 0x30, 0x20};

static const unsigned char RIGHT_bits[] U8X8_PROGMEM = {
    0x04, 0x0c, 0x1c, 0x3c, 0x7c, 0x3c, 0x1c, 0x0c, 0x04};

static const unsigned char DOWN_bits[] U8X8_PROGMEM = {
    0x7f, 0x3e, 0x3e, 0x1c, 0x1c, 0x08, 0x00, 0x00, 0x00};

static const unsigned char UP_bits[] U8X8_PROGMEM = {
    0x08, 0x1c, 0x1c, 0x3e, 0x3e, 0x7f, 0x00, 0x00, 0x00};

// WIDTH CONST
#define w24 24
#define w16 16
#define w8 8

// HEIGHT CONST
#define h12 12
#define h10 10
#define h9 9
#define h8 8
#define h7 7
#define h6 6

static const unsigned char go_back_bits_w24_h10[] U8X8_PROGMEM = {
    0x00, 0x06, 0x00, 0x80, 0x05, 0x00, 0x60, 0xfc, 0x0f, 0x18, 0x00, 0x34,
    0x06, 0x00, 0x24, 0x18, 0x00, 0x44, 0x60, 0xfc, 0x47, 0x80, 0x05, 0x44,
    0x00, 0x06, 0x44, 0x00, 0x00, 0x7c};

static const unsigned char brightness_bits_w16_h12[] U8X8_PROGMEM = {
    0x80, 0x00, 0x00, 0x08, 0xc8, 0x03, 0x20, 0x04, 0x10, 0x08, 0x10, 0x68,
    0x16, 0x08, 0x10, 0x08, 0x20, 0x04, 0xc0, 0x13, 0x10, 0x00, 0x00, 0x01};

static const unsigned char turn_off_bits_w16_h10[] U8X8_PROGMEM = {
    0x80, 0x01, 0xa0, 0x05, 0x90, 0x09, 0x88, 0x11, 0x88, 0x11, 0x88, 0x11,
    0x08, 0x10, 0x10, 0x08, 0x20, 0x04, 0xc0, 0x03};

static const unsigned char dark_mod_bits_w16_h10[] U8X8_PROGMEM = {
    0xc0, 0x03, 0x30, 0x0f, 0x10, 0x0f, 0x08, 0x1f, 0x08, 0x1f, 0x08, 0x1f,
    0x08, 0x1f, 0x10, 0x0f, 0x30, 0x0f, 0xc0, 0x03};

static const unsigned char brigtness_25_bits_w16_h8[] U8X8_PROGMEM = {
    0xbc, 0x2f, 0xf4, 0x3d, 0xfc, 0x3f, 0xbc, 0x2f, 0xf4, 0x3d, 0xfc, 0x3f,
    0xbc, 0x2f, 0xf4, 0x3d};

static const unsigned char brigtness_50_bits_w16_h8[] U8X8_PROGMEM = {
    0xee, 0x2e, 0xba, 0x3b, 0xee, 0x2e, 0xba, 0x3b, 0xee, 0x2e, 0xba, 0x3b,
    0xee, 0x2e, 0xba, 0x3b};

static const unsigned char brigtness_75_bits_w16_h8[] U8X8_PROGMEM = {
    0xac, 0x2a, 0x54, 0x35, 0xac, 0x2a, 0x54, 0x35, 0xac, 0x2a, 0x54, 0x35,
    0xac, 0x2a, 0x54, 0x35};

static const unsigned char brigtness_100_bits_w16_h8[] U8X8_PROGMEM = {
    0x8c, 0x28, 0x24, 0x22, 0x8c, 0x08, 0x24, 0x22, 0x8c, 0x08, 0x24, 0x22,
    0x8c, 0x08, 0x24, 0x22};

static const unsigned char battery_bits_w16_h7[] U8X8_PROGMEM = {
    0xf0, 0x1f, 0x10, 0x10, 0x18, 0x10, 0x18, 0x10, 0x18, 0x10, 0x10, 0x10,
    0xf0, 0x1f};

static const unsigned char switch_on_bits_w16_h6[] U8X8_PROGMEM = {
    0x30, 0x12, 0x48, 0x16, 0x48, 0x1a, 0x48, 0x12, 0x48, 0x12, 0x30, 0x12};

static const unsigned char switch_off_bits_w16_h6[] U8X8_PROGMEM = {
    0xcc, 0x7b, 0x52, 0x08, 0xd2, 0x39, 0x52, 0x08, 0x52, 0x08, 0x4c, 0x08};

static const unsigned char wiki_bits_w8_h10[] U8X8_PROGMEM = {
    0x1c, 0x3e, 0x66, 0x60, 0x30, 0x18, 0x18, 0x00, 0x18, 0x18};

static const unsigned char charging_bits_w16_h9[] U8X8_PROGMEM = {
    0x00, 0x02, 0x30, 0x19, 0x90, 0x11, 0xd8, 0x10, 0xd8, 0x17, 0x18, 0x16,
    0x10, 0x13, 0x30, 0x19, 0x80, 0x00};

static const unsigned char reset_bits_w16_h10[] U8X8_PROGMEM = {
    0xd0, 0x03, 0x30, 0x04, 0x70, 0x08, 0x00, 0x10, 0x08, 0x10, 0x08, 0x10,
    0x08, 0x10, 0x10, 0x08, 0x20, 0x04, 0xc0, 0x03};

//--- BITMAPS --------------------------

//---- ANIMATIONS ---
//-- ok press

uint8_t animation_ok_button_y = 22;
#define ANIMATION_OK_BUTTON_MAX 22

//-- dice roll
boolean animation_dice_roll = false;    //флаг начала и конца анимации после отжатия кнопки ОК
int8_t animation_dice_roll_delay = 1;   // шаг по кадрам с которым будет меняться фрейм кубика на экране
int8_t animation_dice_roll_period = 30; // кол-во кадров которые кубик будет крутиться после отжатия кнопки ОК

// -- BATTERY LOW
boolean battery_blink; // для моргания батарейкой
//---- ANIMATIONS ---

//---- BUTTONS AND PINS -----

#define L 7
boolean L_flag = false;

#define R 3
boolean R_flag = false;

#define UP 6
boolean UP_flag = false;

#define DOWN 4
boolean DOWN_flag = false;

#define OK 5
boolean OK_flag = false;

#define ON_OFF_SWITCHER 2  // D2
#define ON_OFF_INTERRUPT 0 // 0 - D2 || 1 - D3
volatile boolean ON_OFF_flag = true;
volatile unsigned long ON_OFF_debounce_timer = 0;

#define IS_CHARGING_PIN A0

#define CHARGED_PIN A1

#define RANDOM_SEED_PIN A7
//---- BUTTONS AND PINS -----

//---- TIMERS -----------------------

unsigned long timer = 0;
#define TIMER_DELAY 50

unsigned long timer2 = 0;
#define TIMER_2_DELAY 100

unsigned long timer3 = 0;
#define TIMER_3_DELAY 500

unsigned long timer_save_munchkin = 0; // таймер на 5 мин для сохранения манчкина

unsigned long timer_game_sleep = 0;   //для ухода в сон из режима игры
#define TIMER_GAME_SLEEP_DELAY 900000 //по истечению n мс автоматический выход из режима игра и вход в сон
//                                      // минуты в мс переводить по формуле: минуты * 60 * 1000 = мс
//                                      // 15 мин = 900000 мс

unsigned long timer_low_battery = 0;
#define TIMER_DISPLAY_OFF_DELAY 1500

unsigned long timer_service_quit = 0;
#define TIMER_SERVICE_QUIT_DELAY 300000 // по истечению n мс автоматический выход из сервис меню
//                                      // минуты в мс переводить по формуле: минуты * 60 * 1000 = мс
//                                      // 5 мин = 300000 мс

//---- TIMERS -----------------------

// ---- GAME UPPER MENU and MUNCHKIN PARAMS

struct
{
  int8_t level = 1;   // 1 >= level <= 99
  boolean gender = 0; // 0 - female, 1 - male
  int8_t bonus = 0;   // 1 >= bonus <= 99
} munchkin;

struct
{
  int8_t level = 1;   // 1 >= level <= 99
  boolean gender = 0; // 0 - female, 1 - male
  int8_t bonus = 0;   // 1 >= bonus <= 99
} munchkinToDefault;

int8_t game_Power = 0; // lvl + bonus
uint8_t dice = 5;      // 1 >= dice <= 6

uint8_t game_Pointer = 0; // 0 >= game_Pointer <= 5

// - LVL 2 SIGHN FOR ONES AND TENS
int8_t level_tens = 0;
int8_t level_ones = 0;

// - BONUS 2 SIGHN FOR ONES AND TENS
int8_t bonus_tens = 0;
int8_t bonus_ones = 0;

// - POWER 2 SIGHN FOR ONES AND TENS
int8_t power_tens = 0;
int8_t power_ones = 0;

// - RIGHT BIG BOX 2 SIGHN FOR ONES AND TENS
int8_t right_big_box_tens = 0;
int8_t right_big_box_ones = 0;
// ---- UPPER MENU and MUNCHKIN PARAMS

//--- SETTINGS ---------------------
#define BATTERY_LOW 3.4 // нижний порог срабатывания защиты от переразрядки аккумулятора, в Вольтах!
// #define initial_calibration 0 // калибровка вольтметра 1 - включить, 0 - выключить

#define WDT_CALIBRATION_CONST 8000 // калибровка WatchDogTimer для более точной тработки пробуждений по таймеру

struct
{
  uint8_t contrastIndex;       // 0 >= contrastIndex <= 4
  boolean black_theme_display; // true - ON | false - OFF
} settings;

struct
{
  uint8_t contrastIndex = 4;           // 0 >= contrastIndex <= 4
  boolean black_theme_display = false; // true - ON | false - OFF
} settingsToDefault;

boolean settings_trigger = false; // false - menu closed, true - menu opens
uint8_t settings_index_x = 0, settings_index_y = 0;

boolean apply_new_settings_flag = true; // must be true by defaul for apply settings after restart

uint8_t contrastVal[] = {0, 64, 128, 192, 255};
//--- SETTINGS ---------------------

//--- BATTERY MANEGEMENT SYSTEM ---------------------------------------------------------
//-----------флажки----------------------------------

boolean flag_can_work;        // флаг, разрешающий включеие устройства (защита от КЗ, обрыва, разрядки)
boolean is_charging, charged; // для проверки стадии зарядки
boolean low_battery_flag;     // если заряд ниже 16% этим флагом включается анимация

volatile boolean device_sleep = false; // состояние устройства вкл или выкл false - включен | true - спит
boolean sevice_menu_flag = false;      // флаг входа в сервис меню

float my_vcc_const;      // константа вольтметра EEPROM adress 8
int bat_vol, bat_volt_f; // хранит напряжение на акуме
int capacity;            // заряд батареи в %
//-------переменные и коэффициенты для фильтра-------
int analog_val;
int bat_old;
float filter_k = 0.04;
//--- BATTERY MANEGEMENT SYSTEM ---------------------------------------------------------

//--- SERVICE MENU-----------------------------------------------------------------------
int8_t service_menu_pointer = 0;

boolean service_menu_calibration = false;
int16_t calibration_mill_volts = 5000;          //для смены калибровочной константы
int8_t calibration_pointer = 0;                 // показывает на разрядный знак калибровочной константы
boolean service_menu_calibration_apply = false; //для вызова меню подтверждения смены константы

boolean service_menu_button_checker = false;

//--- SERVICE MENU-----------------------------------------------------------------------

//------------FUNCTIONS PROTOTYPES--------------------------------------------------------
void drawGameFrame();
void drawServiceMenuFrame();
void drawLowBatteryFrame();

void munchkinParamChecker();
void animationOkButton();
void animationDiceRoll();
uint8_t randomDice();

void settingsApply();
void settingsSave();
void settingsLoad();
void settingsReset();

void munchkinSave();
void munchkinLoad();
void munchkinReset();

void gameButtonState();
void serviceButtonState();
void powerOn();
void powerOff();

void batteryManagementSystem();
void calibration();
long readVcc();
//------------FUNCTIONS PROTOTYPES--------------------------------------------------------

//---------------------------------------------------------------------------------------

void setup()
{
  //-------настройка входов, выходов, кнопок -------------------------
  pinMode(L, INPUT_PULLUP);    //клавиши управления
  pinMode(R, INPUT_PULLUP);    //клавиши управления
  pinMode(UP, INPUT_PULLUP);   //клавиши управления
  pinMode(DOWN, INPUT_PULLUP); //клавиши управления
  pinMode(OK, INPUT_PULLUP);   //клавиши управления

  pinMode(ON_OFF_SWITCHER, INPUT_PULLUP); //клавиша включения/выключения

  pinMode(IS_CHARGING_PIN, INPUT_PULLUP); // получение состояния заряда с платы зарядки акб
  pinMode(CHARGED_PIN, INPUT_PULLUP);     // получение состояния заряда с платы зарядки акб
                                          //-------настройка входов, выходов, кнопок -------------------------

  //проверяем на первое включение устройства
  // EEPROM.update(0, 255); //===========================DEBUG

  int8_t first_activation_key = EEPROM.read(0);
  if (first_activation_key != 0)
  {

    //если это первое включение записываем в EEPROM
    //дефолтные настройки
    settings.black_theme_display = settingsToDefault.black_theme_display;
    settings.contrastIndex = settingsToDefault.contrastIndex;
    EEPROM.put(10, settings);
    //дефолтные параметры манчкина
    munchkin.level = munchkinToDefault.level;
    munchkin.bonus = munchkinToDefault.bonus;
    munchkin.gender = munchkinToDefault.gender;
    EEPROM.put(110, munchkin);
    //выставляем флаг первого включения в 0
    EEPROM.update(0, 0);
    //принудительно переводим в режим калибровки
    sevice_menu_flag = true;
    service_menu_pointer = 0;
    service_menu_calibration = true;
    //устанавливаем дефолтное калибровочное значение для измерения напряжения
    // my_vcc_const = 1.1;
    EEPROM.get(2, my_vcc_const);
  }
  else
  {
    //если включение не первое читаем из EEPROM сохранённые данные и назначаем
    //настройки
    settingsLoad();

    //параметры манчкина
    munchkinLoad();

    //калибровочное значение для измерения напряжения
    EEPROM.get(2, my_vcc_const);
  }

  // измерить напряжение аккумулятора
  bat_vol = readVcc();
  bat_old = bat_vol;

  // проверка заряда акума, если разряжен то прекратить работу
  if (bat_vol < BATTERY_LOW * 1000)
  {
    flag_can_work = 0;
    //выключить девайс
  }
  else
  {
    flag_can_work = 1;
    //оставить девайс вкл
  }

  //----калибруем WDT-------
  power.calibrate(WDT_CALIBRATION_CONST);
  power.setSleepMode(POWERDOWN_SLEEP); // устанавливаем режим сна
  //----калибруем WDT-------

  //----задаём сид число для генерации рандомных чисел----
  const unsigned long seed = analogRead(RANDOM_SEED_PIN); // случайное число для генерации случайных чисел
  randomSeed(seed);
  //----задаём сид число для генерации рандомных чисел----

  //----калибруем WDT-------
  power.calibrate(WDT_CALIBRATION_CONST);
  //----калибруем WDT-------

  //----инициализация дисплея----
  // u8g2.setBusClock(8000000);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setDisplayRotation(U8G2_R2); // U8G2_R0 - No rotation, landscape | U8G2_R2	180 degree clockwise rotation
  //----инициализация дисплея----

  //обработчик входа в сервис меню
  if (!digitalRead(DOWN) && !digitalRead(ON_OFF_SWITCHER)) // если при будет зажаты клавиши [DOWN] и [ON_OF_SWITCHER]
  {
    sevice_menu_flag = true;
  }

  munchkinParamChecker();
  apply_new_settings_flag = true;
  settingsApply(); //применить сохранённые настройки

  //----DEBUG HARDCODET VALS------

  //----DEBUG HARDCODET VALS------
}

void drawGameFrame()
{
  u8g2.firstPage();
  do
  {
    if (!settings_trigger)
    {
      //------- DRAW GAME CONTENT -------

      // u8g2.setDrawColor(1);
      // u8g2.setFont(u8g2_font_6x13_t_cyrillic);
      // u8g2.setCursor(10, 10);
      // u8g2.print(10);

      u8g2.setFontMode(1);
      u8g2.setDrawColor(1);
      u8g2.setBitmapMode(1);
      u8g2.setFont(u8g2_font_courR10_tn); // for small menu bar

      //-- small LVL
      if (level_tens == 0)
      {
        u8g2.setCursor(X_LVL_SMALL_BOX + 3, Y_SMALL_BOX + 12);
        u8g2.print(level_ones);
      }
      else
      {
        u8g2.setCursor(X_LVL_SMALL_BOX, Y_SMALL_BOX + 12);
        u8g2.print(level_tens);
        u8g2.setCursor(X_LVL_SMALL_BOX + 6, Y_SMALL_BOX + 12);
        u8g2.print(level_ones);
      }

      // -- small GENDER
      if (munchkin.gender)
      {
        u8g2.drawXBMP(X_GENDER_SMALL_BOX - 1, Y_SMALL_BOX - 1, SMALL_ICON_WIDTH, SMALL_ICON_HEIGHT, small_male_cyrillic_bits);
      }
      else
      {
        u8g2.drawXBMP(X_GENDER_SMALL_BOX - 1, Y_SMALL_BOX - 1, SMALL_ICON_WIDTH, SMALL_ICON_HEIGHT, small_female_cyrillic_bits);
      }

      // -- small BONUS
      u8g2.drawXBMP(X_BONUS_SMALL_BOX - 1, Y_SMALL_BOX - 1, SMALL_ICON_WIDTH, SMALL_ICON_HEIGHT, small_bous_bits);

      if (munchkin.bonus < 0)
      {
        u8g2.setCursor(X_BONUS_SMALL_BOX + 13, Y_SMALL_BOX + 12);
        u8g2.print(F("-"));
        u8g2.setCursor(X_BONUS_SMALL_BOX + 20, Y_SMALL_BOX + 12);
        u8g2.print(bonus_ones);
      }
      else if (bonus_tens == 0)
      {
        u8g2.setCursor(X_BONUS_SMALL_BOX + 14, Y_SMALL_BOX + 12);
        u8g2.print(bonus_ones);
      }
      else
      {
        u8g2.setCursor(X_BONUS_SMALL_BOX + 13, Y_SMALL_BOX + 12);
        u8g2.print(bonus_tens);
        u8g2.setCursor(X_BONUS_SMALL_BOX + 20, Y_SMALL_BOX + 12);
        u8g2.print(bonus_ones);
      }

      // -- small POWER
      u8g2.drawXBMP(X_POWER_SMALL_BOX - 1, Y_SMALL_BOX - 1, SMALL_ICON_WIDTH, SMALL_ICON_HEIGHT, small_power_bits);

      if (game_Power < 0)
      {
        u8g2.setCursor(X_POWER_SMALL_BOX + 13, Y_SMALL_BOX + 12);
        u8g2.print(F("-"));
        u8g2.setCursor(X_POWER_SMALL_BOX + 20, Y_SMALL_BOX + 12);
        u8g2.print(power_ones);
      }
      else if (power_tens == 0)
      {
        u8g2.setCursor(X_POWER_SMALL_BOX + 14, Y_SMALL_BOX + 12);
        u8g2.print(power_ones);
      }
      else
      {
        u8g2.setCursor(X_POWER_SMALL_BOX + 13, Y_SMALL_BOX + 12);
        u8g2.print(power_tens);
        u8g2.setCursor(X_POWER_SMALL_BOX + 20, Y_SMALL_BOX + 12);
        u8g2.print(power_ones);
      }

      // -- small DICE
      u8g2.drawXBMP(X_DICE_SMALL_BOX - 1, Y_SMALL_BOX - 1, SMALL_ICON_WIDTH, SMALL_ICON_HEIGHT, small_dice_bits);

      // -- small MENU
      //при разрядке батареи ниже 16% на дисплее будет поочерёдно меняться иконки настроек меню
      // с иконкой пустой батерии давая сигнал о зарядке
      if (low_battery_flag && battery_blink)
      {
        u8g2.drawXBMP(X_MENU_SMALL_BOX - 1, Y_SMALL_BOX + 3, w16, h7, battery_bits_w16_h7);
      }
      else
      {
        u8g2.drawXBMP(X_MENU_SMALL_BOX - 1, Y_SMALL_BOX - 1, SMALL_ICON_WIDTH, SMALL_ICON_HEIGHT, small_menu_bits);
      }

      // -- DRAW LEFT BIG LVL
      u8g2.setFont(u8g2_font_logisoso38_tn);

      if (level_tens == 0)
      {
        u8g2.setCursor(X_LEFT_BIG_BOX + 10, Y_LEFT_BIG_BOX + 41);
        u8g2.print(level_ones);
      }
      else
      {
        u8g2.setCursor(X_LEFT_BIG_BOX - 1, Y_LEFT_BIG_BOX + 41);
        u8g2.print(level_tens);
        u8g2.setCursor(X_LEFT_BIG_BOX + 20, Y_LEFT_BIG_BOX + 41);
        u8g2.print(level_ones);
      }

      //-- DRAW RIGHT BOX CONTENT
      if (game_Pointer < 2)
      {
        // -- BIG GENDER SYMBOL BITMAP
        if (munchkin.gender)
        {
          u8g2.drawXBMP(X_RIGHT_BIG_BOX + 2, Y_RIGHT_BIG_BOX + 2, BIG_ICON_WIDTH, BIG_ICON_HEIGHT, male_cyrillic_bits);
        }
        else
        {
          u8g2.drawXBMP(X_RIGHT_BIG_BOX + 2, Y_RIGHT_BIG_BOX + 2, BIG_ICON_WIDTH, BIG_ICON_HEIGHT, female_cyrillic_bits);
        }
      }
      else if (game_Pointer == 2 || game_Pointer == 3)
      {

        // -- BIG BONUS OR POWER
        u8g2.setFont(u8g2_font_logisoso38_tn);

        if (right_big_box_tens == 0 && (munchkin.bonus < 0 || game_Power < 0)) // рисует отрицательные единицы
        {

          u8g2.setCursor(X_RIGHT_BIG_BOX - 1, Y_RIGHT_BIG_BOX + 41);
          u8g2.print(F("-"));
          u8g2.setCursor(X_RIGHT_BIG_BOX + 20, Y_RIGHT_BIG_BOX + 41);
          u8g2.print(right_big_box_ones);
        }
        else if (right_big_box_tens == 0) //рисует положительные единицы
        {

          u8g2.setCursor(X_RIGHT_BIG_BOX + 10, Y_RIGHT_BIG_BOX + 41);
          u8g2.print(right_big_box_ones);
        }
        else // рисует положительные десятки и единицы
        {

          u8g2.setCursor(X_RIGHT_BIG_BOX - 1, Y_RIGHT_BIG_BOX + 41);
          u8g2.print(right_big_box_tens);
          u8g2.setCursor(X_RIGHT_BIG_BOX + 20, Y_RIGHT_BIG_BOX + 41);
          u8g2.print(right_big_box_ones);
        }
      }
      else if (game_Pointer == 4)
      {

        // -- BIG DICE
        //  A,D | 2,  4,5,6
        //  B,E | 6
        //  C,F | 4,5,6
        //  G   | 1,3,5

        u8g2.drawRFrame(X_RIGHT_BIG_BOX + 2, Y_RIGHT_BIG_BOX + 2, WIDTH_BIG_BOX - 4, HEIGHT_BIG_BOX - 4, RADIUS_BIG_BOX); // DICE BOX

        // DOT A, D
        if (dice >= 2)
        {
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 6, Y_RIGHT_BIG_BOX + 6, 8, 8, 3);   // A
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 30, Y_RIGHT_BIG_BOX + 30, 8, 8, 3); // D
        }

        // DOT B, E
        if (dice == 6)
        {
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 6, Y_RIGHT_BIG_BOX + 18, 8, 8, 3);  // B
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 30, Y_RIGHT_BIG_BOX + 18, 8, 8, 3); // E
        }
        // DOT C, F
        if (dice >= 4)
        {
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 6, Y_RIGHT_BIG_BOX + 30, 8, 8, 3); // C
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 30, Y_RIGHT_BIG_BOX + 6, 8, 8, 3); // F
        }
        // DOT 7 G
        if (dice == 1 || dice == 3 || dice == 5)
        {
          u8g2.drawRBox(X_RIGHT_BIG_BOX + 18, Y_RIGHT_BIG_BOX + 18, 8, 8, 3);
        }
      }
      else if (game_Pointer == 5)
      {
        // u8g2.setFont(u8g2_font_t0_12_tr);
        u8g2.setFont(u8g2_font_squeezed_r7_tr);
        u8g2.setCursor(X_RIGHT_BIG_BOX + 6, Y_RIGHT_BIG_BOX + 15);
        u8g2.print(F("HOLD [OK]"));
        u8g2.setCursor(X_RIGHT_BIG_BOX + 8, Y_RIGHT_BIG_BOX + 25);
        u8g2.print(F("TO OPEN"));
        u8g2.setCursor(X_RIGHT_BIG_BOX + 7, Y_RIGHT_BIG_BOX + 35);
        u8g2.print(F("SETTINGS"));
      }

      //-- Draw SMALL BOXES
      u8g2.drawRFrame(X_LVL_SMALL_BOX, Y_SMALL_BOX, WIDTH_SMALL_BOX, HEIGHT_SMALL_BOX, RADIUS_SMALL_BOX);        // LEVEL
      u8g2.drawRFrame(X_GENDER_SMALL_BOX, Y_SMALL_BOX, WIDTH_SMALL_BOX, HEIGHT_SMALL_BOX, RADIUS_SMALL_BOX);     // GENDER
      u8g2.drawRFrame(X_BONUS_SMALL_BOX, Y_SMALL_BOX, WIDTH_SMALL_LONG_BOX, HEIGHT_SMALL_BOX, RADIUS_SMALL_BOX); // BONUS
      u8g2.drawRFrame(X_POWER_SMALL_BOX, Y_SMALL_BOX, WIDTH_SMALL_LONG_BOX, HEIGHT_SMALL_BOX, RADIUS_SMALL_BOX); // POWER
      u8g2.drawRFrame(X_DICE_SMALL_BOX, Y_SMALL_BOX, WIDTH_SMALL_BOX, HEIGHT_SMALL_BOX, RADIUS_SMALL_BOX);       // DICE
      u8g2.drawRFrame(X_MENU_SMALL_BOX, Y_SMALL_BOX, WIDTH_SMALL_BOX, HEIGHT_SMALL_BOX, RADIUS_SMALL_BOX);       // MENU

      //-- DRAW BIG BOXES

      u8g2.drawRFrame(X_LEFT_BIG_BOX, Y_LEFT_BIG_BOX, WIDTH_BIG_BOX, HEIGHT_BIG_BOX, RADIUS_BIG_BOX); // LEFT BIG BOX

      u8g2.drawRFrame(X_RIGHT_BIG_BOX, Y_RIGHT_BIG_BOX, WIDTH_BIG_BOX, HEIGHT_BIG_BOX, RADIUS_BIG_BOX); // RIGHT BIG BOX

      //-- DRAW P0INTERS
      if (game_Pointer == 0)
      {
        //- POINTER ON LEVEL
        u8g2.drawBox(X_LVL_SMALL_BOX + 2, Y_SMALL_BOX - 2, 10, 2);
        u8g2.drawBox(X_LVL_SMALL_BOX + 2, Y_SMALL_BOX + 14, 10, 2);
      }
      else if (game_Pointer == 1)
      {
        //- POINTER ON GENDER
        u8g2.drawBox(X_GENDER_SMALL_BOX + 2, Y_SMALL_BOX - 2, 10, 2);
        u8g2.drawBox(X_GENDER_SMALL_BOX + 2, Y_SMALL_BOX + 14, 10, 2);
      }
      else if (game_Pointer == 2)
      {
        //- POINTER ON BONUS
        u8g2.drawBox(X_BONUS_SMALL_BOX + 3, Y_SMALL_BOX - 2, 24, 2);
        u8g2.drawBox(X_BONUS_SMALL_BOX + 3, Y_SMALL_BOX + 14, 24, 2);
      }
      else if (game_Pointer == 3)
      {
        //- POINTER ON POWER
        u8g2.drawBox(X_POWER_SMALL_BOX + 3, Y_SMALL_BOX + 14, 24, 2);
        u8g2.drawBox(X_POWER_SMALL_BOX + 3, Y_SMALL_BOX - 2, 24, 2);
      }
      else if (game_Pointer == 4)
      {
        //- POINTER ON DICE
        u8g2.drawBox(X_DICE_SMALL_BOX + 2, Y_SMALL_BOX - 2, 10, 2);
        u8g2.drawBox(X_DICE_SMALL_BOX + 2, Y_SMALL_BOX + 14, 10, 2);
      }
      else if (game_Pointer == 5)
      {
        //- POINTER ON MENU
        u8g2.drawBox(X_MENU_SMALL_BOX + 2, Y_SMALL_BOX - 2, 10, 2);
        u8g2.drawBox(X_MENU_SMALL_BOX + 2, Y_SMALL_BOX + 14, 10, 2);
      }
    }
    else
    {
      //--------------DRAW SETTINGS CONTENT -------------
      // SETTINGS_FRAME_X = 19, SETTINGS_FRAME_Y = 2, SETTINGS_FRAME_WIDTH = 92, SETTINGS_FRAME_HEIGHT = 62, SETTINGS_FRAME_RADIUS = 4;
      u8g2.setFontMode(1);
      u8g2.setDrawColor(1);
      u8g2.setBitmapMode(1);

      //--- BATTERY
      u8g2.drawRFrame(111, 1, 15, 15, 3);
      if (is_charging == true)
      {
        u8g2.drawXBMP(110, 4, w16, h9, charging_bits_w16_h9); // if is charging
      }
      else if (charged)
      {

        u8g2.drawBox(115, 6, 7, 5);
        u8g2.drawXBMP(110, 5, w16, h7, battery_bits_w16_h7);
      }
      else
      {

        u8g2.drawXBMP(110, 5, w16, h7, battery_bits_w16_h7);
        if (capacity <= 100 && capacity > 86)
        {
          u8g2.drawBox(115, 6, 7, 5);
        }
        else if (capacity < 86 && capacity > 72)
        {
          u8g2.drawBox(116, 6, 6, 5);
        }
        else if (capacity < 72 && capacity > 58)
        {
          u8g2.drawBox(117, 6, 5, 5);
        }
        else if (capacity < 58 && capacity > 44)
        {
          u8g2.drawBox(118, 6, 4, 5);
        }
        else if (capacity < 44 && capacity > 30)
        {
          u8g2.drawBox(119, 6, 3, 5);
        }
        else if (capacity < 30 && capacity > 16)
        {
          u8g2.drawBox(120, 6, 2, 5);
        }
        else if (capacity < 16 && capacity > 0)
        {
          if (battery_blink)
          {
            u8g2.drawBox(121, 6, 1, 5);
          }
        }
      }

      //---- SETTINGS FRAME
      u8g2.drawRFrame(SETTINGS_FRAME_X, SETTINGS_FRAME_Y, SETTINGS_FRAME_WIDTH, SETTINGS_FRAME_HEIGHT, SETTINGS_FRAME_RADIUS);

      //-- BRIGHTNESS
      u8g2.drawXBMP(SETTINGS_FRAME_X + 9, SETTINGS_FRAME_Y + 4, w16, h12, brightness_bits_w16_h12); // br icon

      u8g2.drawRFrame(SETTINGS_FRAME_X + 32, SETTINGS_FRAME_Y + 5, 54, 10, 3); // brightness bar
      if (settings.contrastIndex > 0)
        u8g2.drawXBMP(SETTINGS_FRAME_X + 31, SETTINGS_FRAME_Y + 6, w16, h8, brigtness_25_bits_w16_h8);
      if (settings.contrastIndex > 1)
        u8g2.drawXBMP(SETTINGS_FRAME_X + 45, SETTINGS_FRAME_Y + 6, w16, h8, brigtness_50_bits_w16_h8);
      if (settings.contrastIndex > 2)
        u8g2.drawXBMP(SETTINGS_FRAME_X + 58, SETTINGS_FRAME_Y + 6, w16, h8, brigtness_75_bits_w16_h8);
      if (settings.contrastIndex > 3)
        u8g2.drawXBMP(SETTINGS_FRAME_X + 71, SETTINGS_FRAME_Y + 6, w16, h8, brigtness_100_bits_w16_h8);

      //-- DARK MODE
      u8g2.drawXBMP(SETTINGS_FRAME_X + 9, SETTINGS_FRAME_Y + 19, w16, h10, dark_mod_bits_w16_h10); // dark mode icon

      u8g2.drawRFrame(SETTINGS_FRAME_X + 40, SETTINGS_FRAME_Y + 19, 38, 10, 3); // switcher bar

      if (settings.black_theme_display)
      {
        //-if ON
        u8g2.drawRFrame(SETTINGS_FRAME_X + 59, SETTINGS_FRAME_Y + 21, 17, 6, 3);                     // switcher right ON
        u8g2.drawXBMP(SETTINGS_FRAME_X + 42, SETTINGS_FRAME_Y + 21, w16, h6, switch_on_bits_w16_h6); // on
      }
      else
      {
        //-if OFF
        u8g2.drawRFrame(SETTINGS_FRAME_X + 42, SETTINGS_FRAME_Y + 21, 17, 6, 3);                      // switcher left OFF
        u8g2.drawXBMP(SETTINGS_FRAME_X + 60, SETTINGS_FRAME_Y + 21, w16, h6, switch_off_bits_w16_h6); // off
      }

      //-- PLACEHOLDER

      //--- LOWER BUTTONS
      // u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 45 , 86, 14, 3);

      //-- GO BACK
      u8g2.drawXBMP(SETTINGS_FRAME_X + 5, SETTINGS_FRAME_Y + 47, w24, h10, go_back_bits_w24_h10); // go back icon

      //-- (?) WIKI
      u8g2.drawXBMP(SETTINGS_FRAME_X + 42, SETTINGS_FRAME_Y + 47, w8, h10, wiki_bits_w8_h10); // wiki icon

      //-- SHUTDOWN
      u8g2.drawXBMP(SETTINGS_FRAME_X + 42 + 25, SETTINGS_FRAME_Y + 47, w16, h10, reset_bits_w16_h10); // turn Off icon

      //--- DRAW SETTINGS POINTERS ----
      if (settings_index_y == 0)
      {
        //-- BRIGHTNESS

        // u8g2.drawBox(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 3, 86, 14);

        // u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 3, 28, 14, 3);
        u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 3, 86, 14, 3);
      }
      else if (settings_index_y == 1)
      {
        //-- DARK MODE
        // u8g2.drawBox(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 17, 86, 14);
        // u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 17, 28, 14, 3);
        u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 17, 86, 14, 3);
      }
      else if (settings_index_y == 2)
      {
        //-- PLACEHOLDER
        // u8g2.drawBox(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 31, 86, 14);
        u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 31, 86, 14, 3);
      }
      else if (settings_index_y == 3)
      {
        //--- LOWER BUTTONS
        // u8g2.drawBox(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 45 , 86, 14);
        if (settings_index_x == 0)
        {
          //-- GO BACK
          // u8g2.drawBox(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 45, 28, 14);

          u8g2.drawRFrame(SETTINGS_FRAME_X + 3, SETTINGS_FRAME_Y + 45, 28, 14, 3);
        }
        else if (settings_index_x == 1)
        {
          //-- (?) WIKI
          // u8g2.drawBox(SETTINGS_FRAME_X + 3 + 29, SETTINGS_FRAME_Y + 45, 28, 14);
          u8g2.drawRFrame(SETTINGS_FRAME_X + 32, SETTINGS_FRAME_Y + 45, 28, 14, 3);
        }
        else if (settings_index_x == 2)
        {
          //-- SHUTDOWN
          // u8g2.drawBox(SETTINGS_FRAME_X + 3 + 58, SETTINGS_FRAME_Y + 45, 28, 14);
          u8g2.drawRFrame(SETTINGS_FRAME_X + 61, SETTINGS_FRAME_Y + 45, 28, 14, 3);
        }
      }
    }

    // -------------- DRAW ACTIVE BUTTONS ---------------
    if (!settings_trigger)
    {
      u8g2.setDrawColor(1);
      u8g2.setBitmapMode(1);

      // LEFT POINTER
      if (game_Pointer > 0)
      {
        u8g2.drawRFrame(2, 46, 15, 17, 3);
        u8g2.drawXBMP(2 + 3, 46 + 4, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, LEFT_bits);
        if (L_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(3, 47, 13, 15); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }

      // OK BUTTON
      if (game_Pointer > 3)
      {
        u8g2.drawRFrame(2, Y_LEFT_BIG_BOX, 15, 25, 3);
        u8g2.drawXBMP(3, Y_LEFT_BIG_BOX + 8, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, O_bits);
        u8g2.drawXBMP(8, Y_LEFT_BIG_BOX + 8, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, K_bits);

        if (OK_flag)
        {
          u8g2.setDrawColor(2);
          // u8g2.drawBox(3, 15 + 5, 13, 23); //------IS PRESSED
          // u8g2.drawBox(3, 15 + 5 + animation_ok_button_y + 1, 13, 23 - animation_ok_button_y - 1); //------IS PRESSED/
          u8g2.drawBox(3, 15 + 5 + animation_ok_button_y, 13, 23 - animation_ok_button_y); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }

      // RIGHT POINTER
      if (game_Pointer < 5)
      {
        u8g2.drawRFrame(111, 46, 15, 17, 3);
        u8g2.drawXBMP(111 + 3, 46 + 4, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, RIGHT_bits);
        if (R_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(112, 47, 13, 15); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }
      // UP/DOWN BUTTONS
      if (game_Pointer < 3)
      {
        u8g2.drawRFrame(111, Y_LEFT_BIG_BOX, 15, 25, 3);
        u8g2.drawBox(111, Y_LEFT_BIG_BOX + 11, 15, 3);
        u8g2.drawXBMP(111 + 4, 15 + 7, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, UP_bits);
        u8g2.drawXBMP(111 + 4, 26 + 9, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, DOWN_bits);
        u8g2.setDrawColor(2);
        u8g2.drawLine(111, Y_LEFT_BIG_BOX + 12, 125, Y_LEFT_BIG_BOX + 12);
        u8g2.setDrawColor(1);

        //-UP
        if (UP_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(112, 15 + 5, 13, 10); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
        //-DOWN
        if (DOWN_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(112, 15 + 18, 13, 10); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }
    }
    else
    {
      u8g2.setDrawColor(1);
      u8g2.setBitmapMode(1);

      // OK BUTTON
      if (settings_index_y != 0 && settings_index_y != 2)
      {
        u8g2.drawRFrame(2, Y_LEFT_BIG_BOX, 15, 25, 3);
        u8g2.drawXBMP(3, Y_LEFT_BIG_BOX + 8, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, O_bits);
        u8g2.drawXBMP(8, Y_LEFT_BIG_BOX + 8, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, K_bits);

        if (OK_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(3, 15 + 5 + animation_ok_button_y, 13, 23 - animation_ok_button_y); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }

      // LEFT POINTER
      if (settings_index_y != 1 && settings_index_y != 2)
      {
        u8g2.drawRFrame(2, 46, 15, 17, 3);
        u8g2.drawXBMP(2 + 3, 46 + 4, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, LEFT_bits);
        if (L_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(3, 47, 13, 15); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }

      // RIGHT POINTER
      if (settings_index_y != 1 && settings_index_y != 2)
      {
        u8g2.drawRFrame(111, 46, 15, 17, 3);
        u8g2.drawXBMP(111 + 3, 46 + 4, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, RIGHT_bits);
        if (R_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(112, 47, 13, 15); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }

      // UP/DOWN BUTTONS
      if (true)
      {
        u8g2.drawRFrame(111, Y_LEFT_BIG_BOX, 15, 25, 3);
        u8g2.drawBox(111, Y_LEFT_BIG_BOX + 11, 15, 3);
        u8g2.drawXBMP(111 + 4, 15 + 7, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, UP_bits);
        u8g2.drawXBMP(111 + 4, 26 + 9, SCREEN_BUTTON_WIDTH, SCREEN_BUTTON_HEIGHT, DOWN_bits);
        u8g2.setDrawColor(2);
        u8g2.drawLine(111, Y_LEFT_BIG_BOX + 12, 125, Y_LEFT_BIG_BOX + 12);
        u8g2.setDrawColor(1);

        //-UP
        if (UP_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(112, 15 + 5, 13, 10); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
        //-DOWN
        if (DOWN_flag)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(112, 15 + 18, 13, 10); //------IS PRESSED
          u8g2.setDrawColor(1);
        }
      }
    }

  } while (u8g2.nextPage());
}

void drawServiceMenuFrame()
{
  u8g2.firstPage();
  do
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_t0_12_tr);

    //--Menu--
    u8g2.setCursor(1, 9);
    u8g2.print(F("Battery: "));
    u8g2.setCursor(50, 9);
    u8g2.print((float)bat_volt_f / 1000);
    u8g2.setCursor(70, 9);
    u8g2.print(F(" V | "));
    u8g2.setCursor(100, 9);
    u8g2.print(capacity);
    u8g2.setCursor(120, 9);
    u8g2.print(F("%"));

    u8g2.setCursor(0, 22);
    u8g2.print(F("-Calibrate"));
    u8g2.setCursor(0, 32);
    u8g2.print(F("-Buttons"));
    u8g2.setCursor(0, 42);
    u8g2.print(F("-Display"));
    u8g2.setCursor(0, 52);
    u8g2.print(F("-Reset"));
    u8g2.setCursor(0, 62);
    u8g2.print(F("-QUIT"));
    //--Menu--

    //--context menu --
    u8g2.setDrawColor(2);
    if (service_menu_pointer == 0)
    {
      //-calibration
      u8g2.drawFrame(1, 13, 60, 11);

      if (!service_menu_calibration && service_menu_calibration_apply == false)
      {
        u8g2.setCursor(62, 21);
        u8g2.print(F("Press [OK]"));
        u8g2.setCursor(62, 32);
        u8g2.print(F("to start"));

        u8g2.setCursor(62, 47);
        if (is_charging)
        {
          u8g2.print(F("Charging..."));
        }
        else if (charged)
        {
          u8g2.print(F("Charged! :)"));
        }
        else if (!is_charging && !charged)
        {
          u8g2.print(F("Discharge:("));
        }

        u8g2.setCursor(62, 63);
        u8g2.print(F("const:"));
        u8g2.setCursor(100, 63);
        u8g2.print(my_vcc_const * 1000, 0);
      }
      else if (service_menu_calibration && service_menu_calibration_apply == false)
      {

        u8g2.setCursor(62, 21);
        //           "___________"
        u8g2.print(F("Entr mVolts"));
        u8g2.setCursor(62, 32);
        u8g2.print(F("between pin"));
        u8g2.setCursor(62, 43);
        u8g2.print(F("+5V and GND"));

        // uint16_t calibration_mill_volts = 0; //для смены калибровочной константы
        // int8_t calibration_pointer = 0;      // показывает на разрядный знак калибровочной константы

        u8g2.setCursor(62, 54);
        u8g2.print(calibration_mill_volts);
        u8g2.setCursor(93, 54);
        u8g2.print(F("|QUIT|"));

        if (calibration_pointer == 0)
        {
          u8g2.setCursor(62, 56);
          u8g2.print(F("_"));
        }
        else if (calibration_pointer == 1)
        {
          u8g2.setCursor(62, 56);
          u8g2.print(F(" _"));
        }
        else if (calibration_pointer == 2)
        {
          u8g2.setCursor(62, 56);
          u8g2.print(F("  _"));
        }
        else if (calibration_pointer == 3)
        {
          u8g2.setCursor(62, 56);
          u8g2.print(F("   _"));
        }
        else if (calibration_pointer == 4)
        {
          u8g2.setCursor(93, 56);
          u8g2.print(F("______"));
        }
      }
      // else if (service_menu_calibration && service_menu_calibration_apply == true)
      // {
      //   u8g2.setCursor(62, 21);
      //   //           "___________"
      //   u8g2.print(F("New Vcc"));
      //   u8g2.setCursor(62, 32);
      //   u8g2.print(F("const is:"));
      //   u8g2.setCursor(62, 43);
      //   u8g2.print(calibration_mill_volts);
      // }
    }
    else if (service_menu_pointer == 1)
    {
      //-buttons
      u8g2.drawFrame(1, 23, 60, 11);
      if (!service_menu_button_checker)
      {
        u8g2.setCursor(62, 21);
        u8g2.print(F("Press [OK]"));
        u8g2.setCursor(62, 31);
        u8g2.print(F("to start"));
      }
      else if (service_menu_button_checker)
      {
        boolean L_btn_State = !digitalRead(L);
        boolean R_btn_State = !digitalRead(R);
        boolean UP_btn_State = !digitalRead(UP);
        boolean DOWN_btn_State = !digitalRead(DOWN);
        boolean OK_btn_State = !digitalRead(OK);
        boolean ON_OFF_btn_State = !digitalRead(ON_OFF_SWITCHER);

        u8g2.setCursor(62, 21);
        u8g2.print(F("[OK]+[DOWN]"));
        u8g2.setCursor(62, 31);
        u8g2.print(F("to quit"));

        if (L_btn_State)
        {

          u8g2.setCursor(62, 41);
          u8g2.print(F("[<-]"));
        }
        if (R_btn_State)
        {

          u8g2.setCursor(95, 41);
          u8g2.print(F("[->]"));
        }
        if (UP_btn_State)
        {
          u8g2.setCursor(62, 52);
          u8g2.print(F("[/\\]"));
        }
        if (DOWN_btn_State)
        {
          u8g2.setCursor(95, 52);
          u8g2.print(F("[\\/]"));
        }
        if (OK_btn_State)
        {
          u8g2.setCursor(62, 63);
          u8g2.print(F("[OK]"));
        }
        if (ON_OFF_btn_State)
        {
          u8g2.setCursor(95, 63);
          u8g2.print(F("[(!)]"));
        }
      }
    }
    else if (service_menu_pointer == 2)
    {
      //-display
      u8g2.drawFrame(1, 33, 60, 11);

      //           "___________"
      u8g2.setCursor(62, 21);
      u8g2.print(F("  WORK IN  "));
      u8g2.setCursor(62, 32);
      u8g2.print(F(" PROGRESS  "));
      u8g2.setCursor(62, 43);
      u8g2.print(F("___________"));
      u8g2.setCursor(62, 54);
      u8g2.print(F("___________"));
      u8g2.setCursor(62, 65);
      u8g2.print(F("___________"));
    }
    else if (service_menu_pointer == 3)
    {
      //-reset
      u8g2.drawFrame(1, 43, 60, 11);

      //           "___________"
      u8g2.setCursor(62, 21);
      u8g2.print(F("  WORK IN  "));
      u8g2.setCursor(62, 32);
      u8g2.print(F(" PROGRESS  "));
      u8g2.setCursor(62, 43);
      u8g2.print(F("___________"));
      u8g2.setCursor(62, 54);
      u8g2.print(F("___________"));
      u8g2.setCursor(62, 65);
      u8g2.print(F("___________"));
    }
    else if (service_menu_pointer == 4)
    {
      //-quit
      u8g2.drawFrame(1, 53, 60, 11);

      //           "___________"
      u8g2.setCursor(62, 21);
      u8g2.print(F("Press [OK] "));
      u8g2.setCursor(62, 32);
      u8g2.print(F("to boot in "));
      u8g2.setCursor(62, 43);
      u8g2.print(F("game mode. "));
      // u8g2.setCursor(62, 54);
      // u8g2.print(F("___________"));
      u8g2.setCursor(62, 61);
      char buf[strlen_P(program_version)];
      strcpy_P(buf, program_version);
      u8g2.print(buf);
    }

    //-context menu pointer-

    //--context menu--
    u8g2.drawVLine(60, 10, 54);
    u8g2.drawHLine(0, 10, 128);
    u8g2.setDrawColor(1);

    //--context menu--
  } while (u8g2.nextPage());
}

void drawLowBatteryFrame()
{
  u8g2.firstPage();
  do
  {
    u8g2.firstPage();
    do
    {
      u8g2.drawXBMP(56, 28, 16, 7, battery_bits_w16_h7);
    } while (u8g2.nextPage());
  } while (u8g2.nextPage());
}

void munchkinParamChecker()
{
  // set LVL ONES & TENS
  if (munchkin.level < 1)
  {
    munchkin.level = 1;
  };
  if (munchkin.level > 99)
  {
    munchkin.level = 99;
  };

  if (munchkin.level > 9)
  {
    level_ones = munchkin.level % 10;
    level_tens = (munchkin.level - level_ones) / 10;
  }
  else if (munchkin.level < 10)
  {
    level_ones = munchkin.level;
    level_tens = 0;
  };

  // set BONUS ONES & TENS
  if (munchkin.bonus > 99)
  {
    munchkin.bonus = 99;
  };
  if (munchkin.bonus < -9)
  {
    munchkin.bonus = -9;
  };

  if (munchkin.bonus < 0)
  {
    bonus_ones = abs(munchkin.bonus);
    bonus_tens = 0;
  }
  else if (munchkin.bonus > 9)
  {
    bonus_ones = munchkin.bonus % 10;
    bonus_tens = (munchkin.bonus - bonus_ones) / 10;
  }
  else if (munchkin.bonus < 10)
  {
    bonus_ones = munchkin.bonus;
    bonus_tens = 0;
  };

  // set POWER ONES & TENS
  game_Power = munchkin.level + munchkin.bonus;

  if (game_Power > 99)
  {
    game_Power = 99;
  };
  if (game_Power < -9)
  {
    game_Power = -9;
  };

  if (game_Power < 0)
  {
    power_ones = abs(game_Power);
    power_tens = 0;
  }
  else if (game_Power > 9)
  {
    power_ones = game_Power % 10;
    power_tens = (game_Power - power_ones) / 10;
  }
  else if (game_Power < 10)
  {
    power_ones = game_Power;
    power_tens = 0;
  };

  // set RIGHT ONES & TENS
  if (game_Pointer == 2)
  {
    right_big_box_tens = bonus_tens;
    right_big_box_ones = bonus_ones;
  }
  else if (game_Pointer == 3)
  {
    right_big_box_tens = power_tens;
    right_big_box_ones = power_ones;
  }
}

void animationOkButton()
{
  if (OK_flag)
  {
    if (animation_ok_button_y > 0)
    {
      animation_ok_button_y -= 2; // default 2
    }
  }
  else
  {
    if (animation_ok_button_y != ANIMATION_OK_BUTTON_MAX)
    {
      animation_ok_button_y = ANIMATION_OK_BUTTON_MAX;
    }
  }
}

void animationDiceRoll()
{
  if (animation_dice_roll)
  {

    if (animation_dice_roll_period > 0)
    {

      dice = randomDice();
      animation_dice_roll_period -= animation_dice_roll_delay;
    }
    else if (animation_dice_roll_period <= 0)
    {
      dice = randomDice();
      animation_dice_roll_period = 30;
      animation_dice_roll = false;
    }
  }
  else if (OK_flag && game_Pointer == 4 && animation_ok_button_y < 1)
  {

    dice = randomDice();
  }
}

uint8_t randomDice()
{
  uint8_t random_dice = random(0, 7);
  if (random_dice > 6)
  {
    random_dice = 6;
  }
  else if (random_dice < 1)
  {
    random_dice = 1;
  }

  return random_dice;
}

void settingsApply()
{
  if (apply_new_settings_flag)
  {
    if (settings.black_theme_display)
    {
      u8g2.sendF("c", 0x0a6); // 0x0a6 - black
    }
    else
    {
      u8g2.sendF("c", 0x0a7); // 0x0a7 - white
    }
    u8g2.setContrast(contrastVal[settings.contrastIndex]);
    apply_new_settings_flag = false;
  }
}

void settingsSave()
{
  EEPROM.put(10, settings);
}

void settingsLoad()
{
  EEPROM.get(10, settings);
}

void settingsReset()
{
  //дефолтные настройки
  settings.black_theme_display = settingsToDefault.black_theme_display;
  settings.contrastIndex = settingsToDefault.contrastIndex;
  EEPROM.put(10, settings);
}

void munchkinSave()
{
  EEPROM.put(110, munchkin);
}

void munchkinLoad()
{
  EEPROM.get(110, munchkin);
}

void munchkinReset()
{
  //дефолтные параметры манчкина
  munchkin.level = munchkinToDefault.level;
  munchkin.bonus = munchkinToDefault.bonus;
  munchkin.gender = munchkinToDefault.gender;
  EEPROM.put(110, munchkin);
}

void gameButtonState()
{

  boolean L_btn_State = !digitalRead(L);
  boolean R_btn_State = !digitalRead(R);
  boolean UP_btn_State = !digitalRead(UP);
  boolean DOWN_btn_State = !digitalRead(DOWN);
  boolean OK_btn_State = !digitalRead(OK);

  //добавить сброс таймера ухода в сон от айдла при нажатии кнопок
  if (L_btn_State || R_btn_State || UP_btn_State || DOWN_btn_State || OK_btn_State)
  {
    //если была нажата хоть одна из кнопок - обнуляем таймер бездействия
    //нужно интегрировать это в функцию опроса кнопок с проверкой на флаг service_menu_flag
    timer_game_sleep = millis();
  }

  //-- LEFT BUTTON
  if (L_btn_State && !L_flag)
  {
    L_flag = true;
  }

  if (!L_btn_State && L_flag)
  {
    // GAME
    L_flag = false;
    if (!settings_trigger)
    {
      if (game_Pointer > 0)
      {

        game_Pointer--;
      }
    }
    else
    {
      // SETTINGS

      if (settings_index_x > 0)
      {
        settings_index_x--;
      }

      if (settings_index_y == 0 && settings.contrastIndex > 0)
      {
        settings.contrastIndex--;
        apply_new_settings_flag = true;
      }
    }
  }

  //-- RIGHT BUTTON
  if (R_btn_State && !R_flag)
  {
    R_flag = true;
  }

  if (!R_btn_State && R_flag)
  {
    R_flag = false;

    if (!settings_trigger)
    {
      // GAME

      if (game_Pointer < 5)
      {
        game_Pointer++;
      }
    }
    else
    {
      // SETTINGS
      if (settings_index_x < 2)
      {
        settings_index_x++;
      }

      if (settings_index_y == 0 && settings.contrastIndex < 4)
      {
        settings.contrastIndex++;
        apply_new_settings_flag = true;
      }
    }
  }

  //-- UP DOWN BUTTONS

  //-- UP
  if (UP_btn_State && !UP_flag)
  {
    UP_flag = true;
  }

  if (!UP_btn_State && UP_flag)
  {
    UP_flag = false;
    if (!settings_trigger)
    {
      // GAME
      if (game_Pointer == 0)
      {
        // LVL INCREMENT
        munchkin.level++;
      }
      else if (game_Pointer == 1)
      {
        // GENDER CHANGE
        munchkin.gender = !munchkin.gender;
      }
      else if (game_Pointer == 2)
      {
        // BONUS INCREMENT
        munchkin.bonus++;
      }
    }
    else
    {
      // SETTINGS
      if (settings_index_y > 0)
      {
        settings_index_y--;
      }
    }
  }

  // DOWN
  if (DOWN_btn_State && !DOWN_flag)
  {
    DOWN_flag = true;
  }

  if (!DOWN_btn_State && DOWN_flag)
  {
    DOWN_flag = false;
    if (!settings_trigger)
    {
      // GAME
      if (game_Pointer == 0)
      {
        // LVL DECREMENT
        munchkin.level--;
      }
      else if (game_Pointer == 1)
      {
        // GENDER CHANGE
        munchkin.gender = !munchkin.gender;
      }
      else if (game_Pointer == 2)
      {
        // BONUS DECREMENT
        munchkin.bonus--;
      }
    }
    else
    {
      // SETTINGS
      if (settings_index_y < 3)
      {
        settings_index_y++;
      }
    }
  }

  //-- OK BUTTON

  if (OK_btn_State && !OK_flag)
  {
    OK_flag = true;
  }

  if (!OK_btn_State && OK_flag)
  {
    OK_flag = false;

    if (!settings_trigger)
    {
      // GAME
      // DICE
      if (game_Pointer == 4)
      {
        if (animation_ok_button_y < 1)
        {

          animation_dice_roll = true;
        }
      }
      // GO TO SETTINGS
      if (game_Pointer == 5)
      {
        if (animation_ok_button_y < 1)
        {
          settings_trigger = true;
        }
      }
    }
    else
    {
      // SETTINGS
      if (settings_index_y == 1)
      {
        if (animation_ok_button_y < 1)
        {
          settings.black_theme_display = !settings.black_theme_display;
          apply_new_settings_flag = true;
        }
      }

      // GO BACK to gametable
      if (settings_index_y == 3 && settings_index_x == 0)
      {
        if (animation_ok_button_y < 1)
        {
          settingsSave(); // сохраняем настройки в EEPROM

          settings_trigger = false;
          settings_index_x = 0;
          settings_index_y = 0;
        }
      }

      // RESET munchkin params
      if (settings_index_y == 3 && settings_index_x == 2)
      {
        if (animation_ok_button_y < 1)
        {
          munchkinReset(); // сброс манчкина до дефолтного
        }
      }
    }
  }
}

void serviceButtonState()
{
  // if (!device_sleep)
  //   return;

  boolean L_btn_State = !digitalRead(L);
  boolean R_btn_State = !digitalRead(R);
  boolean UP_btn_State = !digitalRead(UP);
  boolean DOWN_btn_State = !digitalRead(DOWN);
  boolean OK_btn_State = !digitalRead(OK);
  boolean ON_OFF_btn_State = !digitalRead(ON_OFF_SWITCHER);

  if (L_btn_State || R_btn_State || UP_btn_State || DOWN_btn_State || OK_btn_State || ON_OFF_btn_State)
  {
    //если была нажата хоть одна из кнопок - обнуляем таймер бездействия
    //нужно интегрировать это в функцию опроса кнопок с проверкой на флаг service_menu_flag
    timer_service_quit = millis();
  }

  //-- LEFT BUTTON
  if (L_btn_State && !L_flag)
  {
    L_flag = true;
  }

  if (!L_btn_State && L_flag)
  {
    L_flag = false;
    if (service_menu_pointer == 0 && service_menu_calibration == true && calibration_pointer > 0)
    {
      calibration_pointer--;
    }
  }

  //-- RIGHT BUTTON
  if (R_btn_State && !R_flag)
  {
    R_flag = true;
  }

  if (!R_btn_State && R_flag)
  {
    R_flag = false;
    if (service_menu_pointer == 0 && service_menu_calibration == true && calibration_pointer < 4)
    {
      calibration_pointer++;
    }
  }

  //-- UP DOWN BUTTONS

  //-- UP
  if (UP_btn_State && !UP_flag)
  {
    UP_flag = true;
  }

  if (!UP_btn_State && UP_flag)
  {
    UP_flag = false;
    if (service_menu_pointer > 0 && !service_menu_button_checker && !service_menu_calibration)
    {
      // moove down in service menu
      service_menu_pointer--;
    }
    else if (service_menu_pointer == 0 && service_menu_calibration == true && calibration_mill_volts < 6000)
    {
      // encrement calibration_mill_volts depens on pointer
      if (calibration_pointer == 0)
      {
        calibration_mill_volts += 1000;
      }
      else if (calibration_pointer == 1)
      {
        calibration_mill_volts += 100;
      }
      else if (calibration_pointer == 2)
      {
        calibration_mill_volts += 10;
      }
      else if (calibration_pointer == 3)
      {
        calibration_mill_volts += 1;
      }
      if (calibration_mill_volts > 6000)
        calibration_mill_volts = 6000;
    }
  }

  // DOWN
  if (DOWN_btn_State && !DOWN_flag)
  {
    DOWN_flag = true;
  }

  if (!DOWN_btn_State && DOWN_flag)
  {
    DOWN_flag = false;
    if (service_menu_pointer < 4 && !service_menu_button_checker && !service_menu_calibration)
    {
      // moove up in service menu
      service_menu_pointer++;
    }
    else if (service_menu_pointer == 0 && service_menu_calibration == true && calibration_mill_volts > 0)
    {
      // dencrement calibration_mill_volts depens on pointer
      if (calibration_pointer == 0)
      {
        calibration_mill_volts -= 1000;
      }
      else if (calibration_pointer == 1)
      {
        calibration_mill_volts -= 100;
      }
      else if (calibration_pointer == 2)
      {
        calibration_mill_volts -= 10;
      }
      else if (calibration_pointer == 3)
      {
        calibration_mill_volts -= 1;
      }
      if (calibration_mill_volts < 0)
        calibration_mill_volts = 0;
    }
  }

  //-- OK BUTTON

  if (OK_btn_State && !OK_flag)
  {
    OK_flag = true;
  }

  if (!OK_btn_State && OK_flag)
  {
    OK_flag = false;

    if (service_menu_pointer == 0 && service_menu_calibration == false)
    {
      // go to calibration
      service_menu_calibration = true;
    }
    else if (service_menu_pointer == 0 && service_menu_calibration == true && calibration_pointer < 4)
    {
      // apply calibration val and quit
      service_menu_calibration_apply = true;
    }
    else if (service_menu_pointer == 0 && service_menu_calibration == true && calibration_pointer >= 4)
    {
      service_menu_calibration = false;
      calibration_pointer = 0;
    }
    else if (service_menu_pointer == 1 && service_menu_button_checker == false)
    {
      // go to button check menu
      service_menu_button_checker = true;
    }
    else if (service_menu_pointer == 1 && service_menu_button_checker == true && DOWN_btn_State)
    {
      // eqit from button check menu
      service_menu_button_checker = false;
    }
    else if (service_menu_pointer == 4)
    {
      sevice_menu_flag = false;
    }
  }
}

void powerOn()
{

  if (millis() - ON_OFF_debounce_timer >= 250 && ON_OFF_flag == false)
  {
    ON_OFF_debounce_timer = millis();
    ON_OFF_flag = true;

    device_sleep = false;
    power.wakeUp(); //пробуждение из сна
  }
}

void powerOff()
{
  if (millis() - ON_OFF_debounce_timer >= 250 && ON_OFF_flag == false)
  {
    ON_OFF_debounce_timer = millis();
    ON_OFF_flag = true;

    device_sleep = true;
  }
}

void batteryManagementSystem()
{
  bat_vol = readVcc(); // измерить напряжение аккумулятора в миллиВольтах

  bat_volt_f = filter_k * bat_vol + (1 - filter_k) * bat_old; // фильтруем
  bat_old = bat_volt_f;                                       // фильтруем
  if (bat_volt_f < BATTERY_LOW * 1000)
  {                    // если напряжение меньше минимального
    flag_can_work = 0; // прекратить работу
  }
  else
  {
    flag_can_work = 1;
  }

  is_charging = !digitalRead(IS_CHARGING_PIN);
  charged = !digitalRead(CHARGED_PIN);

  if (bat_volt_f > 3870)
  {
    if (bat_volt_f > 4100)
    {
      capacity = 100;
    }
    else
    {
      capacity = map(bat_volt_f, 4100, 3870, 100, 77);
    }
  }
  else if ((bat_volt_f <= 3870) && (bat_volt_f > 3750))
  {
    capacity = map(bat_volt_f, 3870, 3750, 77, 54);
  }
  else if ((bat_volt_f <= 3750) && (bat_volt_f > 3730))
  {
    capacity = map(bat_volt_f, 3750, 3680, 54, 31);
  }
  else if ((bat_volt_f <= 3730) && (bat_volt_f > 3600))
  {
    capacity = map(bat_volt_f, 3730, 3600, 31, 8);
  }
  else if (bat_volt_f <= 3600)
  {
    capacity = map(bat_volt_f, 3600, 3400, 8, 0);
  }

  if (capacity < 16)
  {
    low_battery_flag = true;
    battery_blink = !battery_blink;
  }
  else
  {
    low_battery_flag = false;
  }
}

void calibration()
{
  //--------калибровка----------
  my_vcc_const = 1.1;                              // дефолтная величина внутреннего опорного напряжения
  int Vcc = calibration_mill_volts;                // вводим напряжение полученное после измерением пользователя
  float real_const = (float)1.1 * Vcc / readVcc(); // расчёт константы

  // общаемся с пользователем - выдaёт напряжение после калибровки
  EEPROM.put(2, real_const); // запись в EEPROM
  my_vcc_const = real_const; // переназначаем константу
  calibration_mill_volts = 5000;

  //------конец калибровки-------
}

long readVcc()
{ //функция чтения внутреннего опорного напряжения, универсальная (для всех ардуин)

  //переключение аналогового порта для измерения сигнала
  // MUX3 | MUX2 | MUX1 | MUX0 | ПИН
  //   0  |  0   |  0   |  0   | ADC0
  //   0  |  0   |  0   |  1   | ADC1
  //   0  |  0   |  1   |  0   | ADC2
  //   0  |  0   |  1   |  1   | ADC3
  //   0  |  1   |  0   |  0   | ADC4
  //   0  |  1   |  0   |  1   | ADC5
  //   0  |  1   |  1   |  0   | ADC6
  //   0  |  1   |  1   |  1   | ADC7
  //   1  |  0   |  0   |  0   | ADC8
  // ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // AVR ATmega 168p/328p измерение внутри контроллера
                                                          //
  delay(2);                                               // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);                                    // Start conversion
  while (bit_is_set(ADCSRA, ADSC))
    ;                  // measuring
  uint8_t low = ADCL;  // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high << 8) | low;

  analog_val = result;

  result = my_vcc_const * 1023 * 1000 / result; // расчёт реального VCC
  return result;                                // возвращает VCC
}

void loop()
{
  do
  {
    batteryManagementSystem(); // бмс должен проверять акб перед входом в замкнутые циклы

    //--- GAME MODE
    if (flag_can_work == true && device_sleep == false && sevice_menu_flag == false)
    {
      //------setup--------
      {

        if (ON_OFF_flag == true)
        {
          attachInterrupt(ON_OFF_INTERRUPT, powerOff, FALLING);

          ON_OFF_flag = false;
        }

        //обнуляем таймеры которые будем использовать перед входом в замкнутый цикл
        timer_game_sleep = millis();    // обнуляем таймер для автовыхода из игры и входа в режим сна
        timer = millis();               // таймер проверки нажатий кнопок на 50 мс
        timer3 = millis();              // таймер на 500 мс
        timer_save_munchkin = millis(); // таймер на 5 мин для сохранения манчкина
        u8g2.setPowerSave(false);

        apply_new_settings_flag = true;
        settingsApply(); //применить сохранённые настройки
      }
      //------setup--------

      do
      {
        if (millis() - timer >= 25) //каждые 50 мс
        {
          timer = millis();

          gameButtonState();
          munchkinParamChecker();
          animationOkButton();
          animationDiceRoll();

          settingsApply();
        }

        if (millis() - timer3 >= 500) //каждые 500 мс
        {
          timer3 = millis();

          batteryManagementSystem();
        }

        if (millis() - timer_save_munchkin >= 60000) //каждые 60 сек сохраняет манчкина
        {
          timer_save_munchkin = millis();

          munchkinSave();
        }

        if (millis() - timer_game_sleep >= TIMER_GAME_SLEEP_DELAY) ////проверяет когда засыпать от бездействия
        {
          munchkinSave();
          timer_game_sleep = millis();

          device_sleep = true;
          ON_OFF_flag = true;
        }

        drawGameFrame();

      } while (flag_can_work == true && device_sleep == false);
    }

    //--- SLEEP MODE
    if (device_sleep == true && sevice_menu_flag == false)
    {
      //------setup--------
      {

        if (ON_OFF_flag == true)
        {

          attachInterrupt(ON_OFF_INTERRUPT, powerOn, FALLING);

          ON_OFF_flag = false;
        }

        u8g2.setPowerSave(true);
      }
      //------setup--------

      do
      {
        batteryManagementSystem();

        if (flag_can_work == false) // записываю в EEPROM замер напряжения на батарее только если она сильно разряжена
        {
          EEPROM.put(5, bat_volt_f);
        }

        power.sleepDelay(SLEEP_2048MS);

      } while (device_sleep == true);
    }

    //--- LOWBATTERY MODE
    if (flag_can_work == false && device_sleep == false && sevice_menu_flag == false)
    {
      //------setup--------
      {
        munchkinSave(); // сохраняем манчкина в EEPROM

        if (ON_OFF_flag == true)
        {

          attachInterrupt(ON_OFF_INTERRUPT, powerOff, FALLING);

          ON_OFF_flag = false;
        }

        u8g2.setPowerSave(false);
        u8g2.sendF("c", 0x0a6); // принудительно выставляем тёмную тему для сбережения энергии

        //обнуляем таймеры которые будем использовать перед входом в замкнутый цикл
        timer_low_battery = millis();
        timer3 = millis();
      }
      //------setup--------

      do
      {

        if (millis() - timer_low_battery >= 1500) //каждые 500 мс
        {
          timer_low_battery = millis();

          device_sleep = true;
          ON_OFF_flag = true;
        }

        if (millis() - timer3 >= 500) //каждые 500 мс
        {
          timer3 = millis();

          batteryManagementSystem();
        }

        drawLowBatteryFrame();
      } while (flag_can_work == false && device_sleep == false);
    }

    //--- SERVICE MODE
    if (device_sleep == false && sevice_menu_flag == true)
    {
      //------setup--------
      {

        u8g2.setPowerSave(false);
        u8g2.sendF("c", 0x0a6); // принудительно выставляем тёмную тему для сбережения энергии

        //обнуляем таймеры которые будем использовать перед входом в замкнутый цикл
        timer_service_quit = millis(); // обнуляем таймер для автовыхода из сервис меню
        timer = millis();              // таймер проверки нажатий кнопок на 50 мс
        timer3 = millis();             // таймер на 500 мс
      }
      //------setup--------

      do
      {
        if (false && millis() - timer_service_quit >= TIMER_SERVICE_QUIT_DELAY) //проверяет когда выходить из сервис меню
        {
          timer_service_quit = millis();

          sevice_menu_flag = false;
          device_sleep = false;
          ON_OFF_flag = true;
        }

        if (millis() - timer >= 50) //каждые 50 мс
        {
          timer = millis();
          serviceButtonState();
          if (service_menu_calibration == true && service_menu_calibration_apply == true)
          {
            calibration();
            service_menu_calibration = false;
            service_menu_calibration_apply = false;
          }
        }

        if (millis() - timer3 >= 500) //каждые 500 мс
        {
          timer3 = millis();

          batteryManagementSystem();
        }

        drawServiceMenuFrame();

      } while (sevice_menu_flag);
    }

  } while (true);
}
