simpleDSTadjust dstAdjusted(StartRule, EndRule);

// draws the clock
void drawTime() {

  char time_str[11];
  char *dstAbbrev;
  time_t now = dstAdjusted.time(&dstAbbrev);
  struct tm * timeinfo = localtime (&now);

  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setColor(MINI_WHITE);
  String date = WDAY_NAMES[timeinfo->tm_wday] + " " + MONTH_NAMES[timeinfo->tm_mon] + " " + String(timeinfo->tm_mday) + " " + String(1900 + timeinfo->tm_year);
  gfx.drawString(120, 6, date);

  gfx.setFont(ArialRoundedMTBold_36);

  if (IS_STYLE_12HR) {
    int hour = (timeinfo->tm_hour+11)%12+1;  // take care of noon and midnight
    sprintf(time_str, "%2d:%02d:%02d\n",hour, timeinfo->tm_min, timeinfo->tm_sec);
  } else {
    sprintf(time_str, "%02d:%02d:%02d\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  }
  gfx.drawString(120, 20, time_str);

  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setFont(ArialMT_Plain_10);
  gfx.setColor(MINI_BLUE);
  if (IS_STYLE_12HR) {
    sprintf(time_str, "%s\n%s", dstAbbrev, timeinfo->tm_hour>=12?"PM":"AM");
  } else {
    sprintf(time_str, "%s", dstAbbrev);
  }
  gfx.drawString(195, 27, time_str);  // Known bug: Cuts off 4th character of timezone abbreviation
}

time_t updateTimeFromNTP() {
  time_t now;
  configTime(UTC_OFFSET * 3600, 0, NTP_SERVERS);
  while((now = time(nullptr)) < NTP_MIN_VALID_EPOCH) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.printf("Current time: %d\n", now);
  // calculate for time calculation how much the dst class adds.
  dstOffset = UTC_OFFSET * 3600 + dstAdjusted.time(nullptr) - now;
  Serial.printf("Time difference for DST: %d\n", dstOffset);
  return now;
}
