// Если будут использоваться стандартные RGBW прожекторы на 8 каналов каждый,
// то код подходит отлично. Если будут использоваться другие прожекторы,
// нужна переделка

#include <DmxSimple.h>
#define channel_all 16   //указать общее количество каналов всех устройств. Парное!!!
int mas_Value[8];

#define pinOut 9        //указать пин управления, где есть шим

#include <GyverEncoder.h>
#define CLK 3
#define DT 2
#define SW 4
Encoder enc(CLK, DT, SW);

#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

volatile boolean write_flag;  // флаг факта записи

int mode = 0;  // свитч режимов
bool bright_flag = true;  // яркость
int brightness = 255;

int rainbowDelay = 220;  //переменные для радуги цветов
byte rain_max = 255;
byte rain_min = 211;

int white_bright = 200; // переменная для яркости белого

byte val_max = 255;   // верхняя граница всех
byte val_min = 0;     // нижняя граница всех

bool set = true;  //для ручного режима
bool main_flag = false; // флаг ручного режима
int val_color = 255;
int channel = 0;

void setup() {
  Serial.begin(9600);
  DmxSimple.usePin(pinOut);
  DmxSimple.maxChannel(channel_all);
  enc.setType(TYPE2);
  attachInterrupt(0, isrCLK, CHANGE);    // прерывание на 2 пине! CLK у энка
  attachInterrupt(1, isrDT, CHANGE);    // прерывание на 3 пине! DT у энка
  oled.init();        // инициализация
  oled.clear();
  oled.setScale(2);
  oled.home();
  oled.print("1. РАДУГА");
  oled.setCursor(0, 3);
  oled.print("Ярк: ");
  oled.setCursor(0, 6);
  oled.print("Поз: ");
  write_flag =  1;
  dmx_write(7, rainbowDelay);
}

void isrCLK() {
  enc.tick();  // отработка в прерывании
  write_flag =  1;
}
void isrDT() {
  enc.tick();  // отработка в прерывании
  write_flag =  1;
}

void dmx_write(byte channel_write, byte value_write) {
  if (write_flag) {
    DmxSimple.write(channel_write, value_write);
    DmxSimple.write(channel_write + 8, value_write);
    write_flag = 0;
  }
}

void dmx_white(byte chanel_write, byte vale_write) {
  if (write_flag) {
    for (int y = 0; y <= 3; y++) {
      DmxSimple.write(chanel_write + y, vale_write);
    }
    for (int u = 8; u <= 11; u++) {
      DmxSimple.write(chanel_write + u, vale_write);
    }
    write_flag = 0;
  }
}

void loop() {
  enc.tick();
  if (enc.isRightH()) {
    for (int i = 2; i <= 8; i++) {
      write_flag = 1;
      dmx_write(i, 0);
    }
    mode++;
    if (mode > 3) mode = 3;            // условия мода
    else if (mode < 0) mode = 0;
    main_flag = false;
    oled.clear();
    oled.home();
    if (mode == 0) {
      oled.print("1. РАДУГА");
      oled.setCursor(0, 3);
      oled.print("Ярк: ");
      oled.setCursor(0, 6);
      oled.print("Скор: ");
      write_flag =  1;
      dmx_write(7, rainbowDelay);
    }
    else if (mode == 1) {
      oled.print("2. БЕЛЫЙ");
      oled.setCursor(0, 3);
      oled.print("Ярк: ");
      oled.setCursor(0, 6);
      oled.print("Поз: ");
      write_flag =  1;
      dmx_write(1, brightness);
      write_flag =  1;
      dmx_white(2, white_bright);
    }
    else if (mode == 2) {
      oled.print("3. ВЫБ.ЦВЕТ");
      oled.setCursor(0, 3);
      oled.print("Ярк: ");
      oled.setCursor(0, 6);
      oled.print("Поз: ");
      write_flag =  1;
      dmx_write(1, brightness);
      dmx_write(2, val_color); //красный
    }
    else if (mode == 3) {
      for (int i = 1; i <= 8; i++) {
        write_flag =  1;
        dmx_write(i, 0);
      }
      oled.print("4. РУЧНОЙ");
      oled.setCursor(0, 3);
      oled.print("Канал: ");
      oled.setCursor(0, 6);
      oled.print("Знач: ");
      main_flag = true;
    }
  }

  if (enc.isLeftH()) {
    for (int i = 2; i <= 8; i++) {
      write_flag = 1;
      dmx_write(i, 0);
    }
    mode--;
    if (mode > 3) mode = 3;            // условия мода
    else if (mode < 0) mode = 0;
    main_flag = false;
    oled.clear();
    oled.home();
    if (mode == 0) {
      oled.print("1. РАДУГА");
      oled.setCursor(0, 3);
      oled.print("Ярк: ");
      oled.setCursor(0, 6);
      oled.print("Скор: ");
      write_flag =  1;
      dmx_write(7, rainbowDelay);
    }
    else if (mode == 1) {
      oled.print("2. БЕЛЫЙ");
      oled.setCursor(0, 3);
      oled.print("Ярк: ");
      oled.setCursor(0, 6);
      oled.print("Поз: ");
      write_flag =  1;
      dmx_write(1, brightness);
      write_flag =  1;
      dmx_white(2, white_bright);
    }
    else if (mode == 2) {
      oled.print("3. ВЫБ.ЦВЕТ");
      oled.setCursor(0, 3);
      oled.print("Ярк: ");
      oled.setCursor(0, 6);
      oled.print("Поз: ");
      write_flag =  1;
      dmx_write(1, brightness);
      dmx_write(2, val_color); //красный
    }
    else if (mode == 3) {
      for (int i = 1; i <= 8; i++) {
        write_flag =  1;
        dmx_write(i, 0);
      }
      oled.print("4. РУЧНОЙ");
      oled.setCursor(0, 3);
      oled.print("Канал: ");
      oled.setCursor(0, 6);
      oled.print("Знач: ");
      main_flag = true;
    }
  }

  if (enc.isClick()) {
    if (mode == 0 or mode == 1 or mode == 2) bright_flag = !bright_flag;
    if (mode == 3) set = !set;
  }

  if (enc.isRight()) {
    if (bright_flag) brightness++;
    if (mode == 0 && !bright_flag) rainbowDelay++;
    else if (mode == 1 && !bright_flag) white_bright += 2;
    else if (mode == 2 && !bright_flag) val_color += 5;
    else if (mode == 3 && set == 1) channel++;
    else if (mode == 3 && set == 0) mas_Value[channel]++;
  }

  if (enc.isLeft()) {
    if (bright_flag) brightness--;
    if (mode == 0 && !bright_flag) rainbowDelay--;
    else if (mode == 1 && !bright_flag) white_bright -= 2;
    else if (mode == 2 && !bright_flag) val_color -= 5;
    else if (mode == 3 && set == 1) channel--;
    else if (mode == 3 && set == 0) mas_Value[channel]--;
  }

  if (enc.isFastR()) {
    if (bright_flag) brightness += 8;
    if (mode == 0 && !bright_flag) rainbowDelay += 2;
    else if (mode == 1 && !bright_flag) white_bright += 8;
    else if (mode == 2 && !bright_flag) val_color += 28;
    else if (mode == 3 && set == 0) mas_Value[channel] += 6;
  }
  if (enc.isFastL()) {
    if (bright_flag) brightness -= 8;
    if (mode == 0 && !bright_flag) rainbowDelay -= 2;
    else if (mode == 1 && !bright_flag) white_bright -= 8;
    else if (mode == 2 && !bright_flag) val_color -= 28;
    else if (mode == 3 && set == 0) mas_Value[channel] -= 6;
  }
  
  if (brightness > val_max) brightness = val_max;            // условия для яркости
  else if (brightness < val_min) brightness = val_min;

  if (rainbowDelay > rain_max) rainbowDelay = rain_max;      // условия для радуги
  else if (rainbowDelay < rain_min) rainbowDelay = rain_min;

  if (white_bright > val_max) white_bright = val_max;        // условия для белого
  else if (white_bright < val_min) white_bright = val_min;

  if (val_color > 1791) val_color = 255;                     // условия для выбора цвета
  else if (val_color < 255) val_color = 1791;

  if (channel > 7) channel = 0;                              // условия для ручного режима
  else if (channel < 0) channel = 7;

  if (mas_Value[channel] > 255) mas_Value[channel] = 0;      // условия для ручного режима
  else if (mas_Value[channel] < 0) mas_Value[channel] = 255;

  // Выставление общей яркости
  if (bright_flag && !main_flag) {
    dmx_write(1, brightness);
    oled.setCursor(70, 3);
    oled.print(brightness); oled.print("<  ");
  }
  else if (!bright_flag && !main_flag) {
    oled.setCursor(70, 3);
    oled.print(brightness); oled.print("  ");
  }

  //Управление скоростью радуги
  if (mode == 0 && !bright_flag) {
    dmx_write(7, rainbowDelay);
    oled.setCursor(70, 6);
    oled.print(rainbowDelay - rain_min); oled.print("< ");
  }
  else if (mode == 0 && bright_flag) {
    oled.setCursor(70, 6);
    oled.print(rainbowDelay - rain_min); oled.print("  ");
  }

  //Чисто белый
  if (mode == 1 && !bright_flag) {
    dmx_white(2, white_bright);
    oled.setCursor(70, 6);
    oled.print(white_bright); oled.print("< ");
  }
  else if (mode == 1 && bright_flag) {
    oled.setCursor(70, 6);
    oled.print(white_bright); oled.print("  ");
  }

  //Выбор цвета
  if (mode == 2 && !bright_flag) {
    oled.setCursor(70, 6);
    oled.print(val_color); oled.print("<  ");
    if (val_color == 255) dmx_write(2, val_color); //красный

    else if (val_color >= 256 && val_color <= 511) dmx_write(3, map(val_color, 256, 511, 0, 255)); //красный+зеленый=желтый

    else if (val_color >= 512 && val_color <= 767) dmx_write(2, map(val_color, 512, 767, 255, 0)); //зеленый

    else if (val_color >= 768 && val_color <= 1023) dmx_write(4, map(val_color, 768, 1023, 0, 255)); //зеленый+синий=голубой

    else if (val_color >= 1024 && val_color <= 1279) dmx_write(3, map(val_color, 1024, 1279, 255, 0)); //синий

    else if (val_color >= 1280 && val_color <= 1535) dmx_write(2, map(val_color, 1280, 1535, 0, 255)); //синий+красный=розовый

    else if (val_color >= 1536 && val_color <= 1791) dmx_write(4, map(val_color, 1536, 1791, 255, 0)); //красный
  }
  else if (mode == 2 && bright_flag) {
    oled.setCursor(70, 6);
    oled.print(val_color); oled.print("   ");
  }

  //Ручной режим
  if (mode == 3) {
    if (set == 1) {
      oled.setCursor(75, 3); oled.print(channel+1);
      oled.print("< ");
      oled.setCursor(75, 6); oled.print(mas_Value[channel]);
      oled.print("  ");
    }

    else if (set == 0) {
      dmx_write(channel+1, mas_Value[channel]);
      oled.setCursor(75, 3); oled.print(channel+1);
      oled.print(" ");
      oled.setCursor(75, 6); oled.print(mas_Value[channel]);
      oled.print("<  ");
    }
  }
}
