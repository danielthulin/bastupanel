#include <MCUFRIEND_kbv.h>
#include <UTFTGLUE.h>
#include <TouchScreen.h>
#include <FastLED.h>

UTFTGLUE myGLCD(0, A2, A1, A3, A4, A0);

const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
//const int TS_LEFT=907,TS_RT=136,TS_TOP=942,TS_BOT=139;
const int TS_LEFT = 926, TS_RT = 141, TS_TOP = 166, TS_BOT = 864;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint p;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define LED_PIN 31
#define NUM_LEDS 5

CRGB leds[NUM_LEDS];

int BAR_MINY = 280;
int BAR_MAXY = 100;
int BAR_WIDTH = 30;
int BAR_SPACE = 30;
int padding = 3;

int REDBAR_MINX = 0 + BAR_SPACE;
int REDBAR_MAXX = REDBAR_MINX + BAR_WIDTH;
int GREENBAR_MINX = REDBAR_MAXX + BAR_SPACE;
int GREENBAR_MAXX = GREENBAR_MINX + BAR_WIDTH;
int BLUEBAR_MINX = GREENBAR_MAXX + BAR_SPACE;
int BLUEBAR_MAXX = BLUEBAR_MINX + BAR_WIDTH;
int DIMBAR_MINX = BLUEBAR_MAXX + BAR_SPACE;
int DIMBAR_MAXX = DIMBAR_MINX + BAR_WIDTH;

// warm wite button
int BUTTON_MINX = DIMBAR_MAXX + 60;
int BUTTON_MAXX = BUTTON_MINX + 150;
int BUTTON_HEIGHT = 40;
int BUTTON_SPACE = 30;
int WW_BUTTON_MINY = 100 + BUTTON_HEIGHT;
int WW_BUTTON_MAXY = WW_BUTTON_MINY - BUTTON_HEIGHT;

int SPD_BUTTON_MINY = WW_BUTTON_MINY + BUTTON_HEIGHT + BUTTON_SPACE;
int SPD_BUTTON_MAXY = SPD_BUTTON_MINY - BUTTON_HEIGHT;


int redval = 239;
int greenval = 235;
int blueval = 216;
int dimval = 5;

int redpos = BAR_MINY;
int greenpos = BAR_MINY;
int bluepos = BAR_MINY;
int dimpos = BAR_MINY;

int oldrpos = redpos;
int oldgpos = greenpos;
int oldbpos = bluepos;
int olddpos = dimpos;

int currentPage;
int x, y;
int prev_x, prev_y;
int until_cond = 0;


// ---------------------------------------------


void show_bar(String color, int MINX, int BAR_MINY, int MAXX, int pos)
{
  if (color == "red") {
    myGLCD.setColor(255, 0, 0);
  } else if (color == "green") {
    myGLCD.setColor(0, 255, 0);
  } else if (color == "blue") {
    myGLCD.setColor(0, 0, 255);
  } else if (color == "black") {
    myGLCD.setColor(0, 0, 0);
  } else if (color == "white") {
    myGLCD.setColor(255, 255, 255);
  }
  myGLCD.fillRect(MINX, BAR_MINY, MAXX, pos);

}
void set_rgbd_val_numbers(int redval, int greenval, int blueval, int dimval) {
  myGLCD.setColor(255, 255, 255);
  myGLCD.printNumI(redval, REDBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
  myGLCD.printNumI(greenval, GREENBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
  myGLCD.printNumI(blueval, BLUEBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
  myGLCD.printNumI(dimval, DIMBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
}
void set_rgb_val_numbers(int redval, int greenval, int blueval) {
  myGLCD.setColor(255, 255, 255);
  myGLCD.printNumI(redval, REDBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
  myGLCD.printNumI(greenval, GREENBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
  myGLCD.printNumI(blueval, BLUEBAR_MINX - 15, BAR_MINY + 10, 3, ' ');
}
void set_bars(int redval, int greenval, int blueval, int dimval) {
  redpos = map(redval, 0, 255, BAR_MINY, BAR_MAXY);
  greenpos = map(greenval, 0, 255, BAR_MINY, BAR_MAXY);
  bluepos = map(blueval, 0, 255, BAR_MINY, BAR_MAXY);
  dimpos = map(dimval, 0, 255, BAR_MINY, BAR_MAXY);

  show_bar("black", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, oldrpos);
  show_bar("red", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, redpos);
  show_bar("black", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, oldgpos);
  show_bar("green", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, greenpos);
  show_bar("black", BLUEBAR_MINX + padding, BAR_MINY,  BLUEBAR_MAXX - padding, oldbpos);
  show_bar("blue", BLUEBAR_MINX + padding, BAR_MINY, BLUEBAR_MAXX - padding, bluepos);
  show_bar("black", DIMBAR_MINX + padding, BAR_MINY,  DIMBAR_MAXX - padding, olddpos);
  show_bar("white", DIMBAR_MINX + padding, BAR_MINY, DIMBAR_MAXX - padding, dimpos);

}
void set_color_bars(int redval, int greenval, int blueval) {
  redpos = map(redval, 0, 255, BAR_MINY, BAR_MAXY);
  greenpos = map(greenval, 0, 255, BAR_MINY, BAR_MAXY);
  bluepos = map(blueval, 0, 255, BAR_MINY, BAR_MAXY);

  show_bar("black", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, oldrpos);
  show_bar("red", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, redpos);
  show_bar("black", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, oldgpos);
  show_bar("green", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, greenpos);
  show_bar("black", BLUEBAR_MINX + padding, BAR_MINY,  BLUEBAR_MAXX - padding, oldbpos);
  show_bar("blue", BLUEBAR_MINX + padding, BAR_MINY, BLUEBAR_MAXX - padding, bluepos);

}
void set_color_output(int redval, int greenval, int blueval) {
  // SET AND SHOW LED OUTPUT
  myGLCD.setColor(redval, greenval, blueval);
  myGLCD.fillRoundRect(REDBAR_MINX, 50 , BLUEBAR_MAXX, 80);
}
void show_rgbScreen(void)
{

  myGLCD.setFont(BigFont);

  // BARS
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(REDBAR_MINX, BAR_MINY, REDBAR_MAXX, BAR_MAXY);
  myGLCD.drawRect(GREENBAR_MINX, BAR_MINY, GREENBAR_MAXX, BAR_MAXY);
  myGLCD.drawRect(BLUEBAR_MINX, BAR_MINY, BLUEBAR_MAXX, BAR_MAXY);
  myGLCD.drawRect(DIMBAR_MINX, BAR_MINY, DIMBAR_MAXX, BAR_MAXY);
  myGLCD.setColor(255, 0, 0);

  set_rgbd_val_numbers(redval, greenval, blueval, dimval);

  // SET AND SHOW LED OUTPUT
  myGLCD.setColor(redval, greenval, blueval);
  myGLCD.fillRoundRect(REDBAR_MINX, 50 , BLUEBAR_MAXX, 80);

  redpos = map(redval, 0, 255, BAR_MINY, BAR_MAXY);
  greenpos = map(greenval, 0, 255, BAR_MINY, BAR_MAXY);
  bluepos = map(blueval, 0, 255, BAR_MINY, BAR_MAXY);
  dimpos = map(dimval, 0, 255, BAR_MINY, BAR_MAXY);

  show_bar("red", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, redpos);
  show_bar("green", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, greenpos);
  show_bar("blue", BLUEBAR_MINX + padding, BAR_MINY, BLUEBAR_MAXX - padding, bluepos);
  show_bar("white", DIMBAR_MINX + padding, BAR_MINY, DIMBAR_MAXX - padding, dimpos);

  // BUTTONS
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(BUTTON_MINX, WW_BUTTON_MINY, BUTTON_MAXX, WW_BUTTON_MAXY);
  myGLCD.print("Varmvitt", BUTTON_MINX, WW_BUTTON_MINY + (WW_BUTTON_MAXY - WW_BUTTON_MINY) / 2);
  myGLCD.drawRect(BUTTON_MINX, SPD_BUTTON_MINY, BUTTON_MAXX, SPD_BUTTON_MAXY);
  myGLCD.print("Speedy", BUTTON_MINX, SPD_BUTTON_MINY + (SPD_BUTTON_MAXY - SPD_BUTTON_MINY) / 2);

  FastLED.setBrightness(dimval);
  for (int i = 0; i <= NUM_LEDS - 1; i++) {
    leds[i] = CRGB (redval, greenval, blueval);
    if ( i > 0) {
      leds[i - 1] = CRGB(0, 0, 0);
    }
    FastLED.show();
    delay(100);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB (redval, greenval, blueval);
    FastLED.show();
    delay(100);

  }



}

// ------------------------------------------------------
void setup() {
  myGLCD.InitLCD();
  myGLCD.clrScr();
  FastLED.setMaxPowerInMilliWatts(75000);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,15000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(dimval );

  show_rgbScreen();
  currentPage = 0;
}

void loop() {
  p = ts.getPoint();

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    x = map(p.y, TS_LEFT, TS_RT, 0, myGLCD.getDisplayXSize());
    y = map(p.x, TS_TOP, TS_BOT, 0, myGLCD.getDisplayYSize());

   /// SET AND SHOW LED OUTPUT
    myGLCD.setColor(redval, greenval, blueval);
    myGLCD.fillRoundRect(REDBAR_MINX, 50 , BLUEBAR_MAXX, 80);

    FastLED.showColor(CRGB(redval,greenval,blueval),dimval);

    // SHOW R, G AND D
    set_rgbd_val_numbers(redval, greenval, blueval, dimval);

    // BARS
    if (y >= BAR_MAXY && y <= BAR_MINY) {
      // SLIDER FOR RED
      if (x >= REDBAR_MINX && x <= REDBAR_MAXX) {
        redval = map(y, BAR_MINY, BAR_MAXY, 0, 255);
        oldrpos = redpos;
        show_bar("black", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, oldrpos);
        redpos = y;
        show_bar("red", REDBAR_MINX + padding, BAR_MINY, REDBAR_MAXX - padding, redpos);
      }
      // SLIDER FOR GREEN
      if (x >= GREENBAR_MINX && x <= GREENBAR_MAXX) {
        greenval = map(y, BAR_MINY, BAR_MAXY, 0, 255);
        oldgpos = greenpos;
        show_bar("black", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, oldgpos);
        greenpos = y;
        show_bar("green", GREENBAR_MINX + padding, BAR_MINY, GREENBAR_MAXX - padding, greenpos);
      }
      //SLIDER FOR BLUE
      if (x >= BLUEBAR_MINX && x <= BLUEBAR_MAXX) {
        blueval = map(y, BAR_MINY, BAR_MAXY, 0, 255);
        oldbpos = bluepos;
        show_bar("black", BLUEBAR_MINX + padding, BAR_MINY,  BLUEBAR_MAXX - padding, oldbpos);
        bluepos = y;
        show_bar("blue", BLUEBAR_MINX + padding, BAR_MINY, BLUEBAR_MAXX - padding, bluepos);
      }
      //SLIDER FOR DIM
      if (x >= DIMBAR_MINX && x <= DIMBAR_MAXX) {
        dimval = map(y, BAR_MINY, BAR_MAXY, 0, 255);
        olddpos = dimpos;
        show_bar("black", DIMBAR_MINX + padding, BAR_MINY,  DIMBAR_MAXX - padding, olddpos);
        dimpos = y;
        show_bar("white", DIMBAR_MINX + padding, BAR_MINY, DIMBAR_MAXX - padding, dimpos);
      }
    }

    // BUTTONS
    if (x >= BUTTON_MINX && x <= BUTTON_MAXX) {
      //Warm white
      if (y <= WW_BUTTON_MINY && y >= WW_BUTTON_MAXY) {
        redval = 239;
        greenval = 235;
        blueval = 216;

        FastLED.showColor(CRGB(redval,greenval,blueval),dimval);

        set_color_bars(redval, greenval, blueval);
        set_rgb_val_numbers(redval, greenval, blueval);
        set_color_output(redval, greenval, blueval);
        
      }
      // SPD button
      if (y <= SPD_BUTTON_MINY && y >= SPD_BUTTON_MAXY) {
        until_cond = 1;
        while (until_cond == 1) {
          for (int i = 0; i <= NUM_LEDS - 1; i++) {
            leds[i] = CRGB (0, 0 + i * 10, 255 - i * 10);
            FastLED.show();
            delay(100);
          }
          for (int i = NUM_LEDS - 1; i >= 0; i--) {
            leds[i] = CRGB (0 + i * 10, 255 - i * 10, 0);
            FastLED.show(100);
          }
          for (int i = 0; i <= NUM_LEDS - 1; i++) {
            leds[i] = CRGB (255 - i * 10, 0, 0 + i * 10);
            FastLED.show();
            delay(100);
          }
          for (int i = NUM_LEDS - 1; i >= 0; i--) {
            leds[i] = CRGB (0 + i * 10, 0, 0 + i * 10);
            FastLED.show();
            delay(100);
          }
          for (int i = NUM_LEDS - 1; i >= 0; i--) {
            leds[i] = CRGB (255, 255, 255);
            FastLED.show();
            delay(100);
          }
          p = ts.getPoint();
          if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
            until_cond = 0;
          }
        }
      }
    }
  }
}
