#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

void draw(void) {
  u8g.setFont(u8g_font_profont12);
  u8g.setPrintPos(0, 10);
  u8g.print("What's our project");
  u8g.setPrintPos(0, 25);
  u8g.print("");
  u8g.setPrintPos(0, 40);
  u8g.print("How is it going");
}

void draw2(void) {
  u8g.setFont(u8g_font_profont15);
  u8g.setPrintPos(0, 10);
  u8g.print("Self Balancing Bot");
  u8g.setPrintPos(0, 25);
  u8g.print("(SBR)");
  u8g.setPrintPos(0, 40);
  u8g.setFont(u8g_font_profont15);
  u8g.print("Project is fcuked");
}

void setup(void) {
}

void loop(void) {
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage() );
  delay(3500);
  u8g.firstPage();
  do {
    draw2();
  } while (u8g.nextPage());
  delay(3500);
}
