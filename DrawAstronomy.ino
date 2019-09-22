
// draw moonphase and sunrise/set and moonrise/set
void drawAstronomy() {

  gfx.setFont(MoonPhases_Regular_36);
  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 275, String((char) (97 + (moonData.illumination * 26))));

  gfx.setColor(MINI_WHITE);
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(MINI_YELLOW);
  gfx.drawString(120, 250, MOON_PHASES[moonData.phase]);
  
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setColor(MINI_YELLOW);
  gfx.drawString(5, 250, SUN_MOON_TEXT[0]);
  gfx.setColor(MINI_WHITE);
  time_t time = currentWeather.sunrise + dstOffset;
  gfx.drawString(5, 276, SUN_MOON_TEXT[1] + ":");
  gfx.drawString(45, 276, getTime(&time));
  time = currentWeather.sunset + dstOffset;
  gfx.drawString(5, 291, SUN_MOON_TEXT[2] + ":");
  gfx.drawString(45, 291, getTime(&time));

  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  gfx.setColor(MINI_YELLOW);
  gfx.drawString(235, 250, SUN_MOON_TEXT[3]);
  gfx.setColor(MINI_WHITE);
  float lunarMonth = 29.53;
  // approximate moon age
  gfx.drawString(235, 276, String(moonData.phase <= 4 ? lunarMonth * moonData.illumination / 2.0 : lunarMonth - moonData.illumination * lunarMonth / 2.0, 1) + "d");
  gfx.drawString(235, 291, String(moonData.illumination * 100, 0) + "%");
  gfx.drawString(190, 276, SUN_MOON_TEXT[4] + ":");
  gfx.drawString(190, 291, SUN_MOON_TEXT[5] + ":");

}
