
struct tm timerStart;
struct tm timerOnTime;
typedef void (*ClickCallback)();
ClickCallback callbacks[] = {increaseStartTimeHour, increaseStartTimeMinute, decreaseStartTimeHour, decreaseStartTimeMinute, increaseOnTimeHour, increaseOnTimeMinute, decreaseOnTimeHour, decreaseOnTimeMinute};

void initialiseMotorTimer() { 
  timerStart.tm_sec = 0;
  timerStart.tm_min = 0;
  timerStart.tm_hour = 7;

  timerOnTime.tm_sec = 0;
  timerOnTime.tm_min = 0;
  timerOnTime.tm_hour = 2;

  loadTimerConfig();
}

void loadTimerConfig() {
  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  Serial.print("SPIFFS opened: ");
  Serial.println(result ? "OK" : "Failed");

  // this opens the file in read-mode
  File f = SPIFFS.open("/timer_config.txt", "r");

  if (!f) {
    Serial.println("No timer_config.txt file.");
    return;
  }
  
  //Lets read line by line from the file
  String startHour = f.readStringUntil('\n');
  String startMinute = f.readStringUntil('\n');
  String onHour = f.readStringUntil('\n');
  String onMinute = f.readStringUntil('\n');
  
  timerStart.tm_hour = startHour.toInt();
  timerStart.tm_min = startMinute.toInt();

  timerOnTime.tm_hour = onHour.toInt();
  timerOnTime.tm_min = onMinute.toInt();
  
  f.close();
}

void saveTimerConfig() {
  bool result = SPIFFS.begin();

  // open the file in write mode
  File f = SPIFFS.open("/timer_config.txt", "w");
  
  if (!f) {
    Serial.println("Failed to create timer_config.txt.");
    return;
  }
  
  // now write two lines in key/value style with  end-of-line characters
  f.println(timerStart.tm_hour);
  f.println(timerStart.tm_min);
  f.println(timerOnTime.tm_hour);
  f.println(timerOnTime.tm_min);

  f.close();
}

void increaseStartTimeHour() {
  timerStart.tm_hour++;
  if(timerStart.tm_hour>23) timerOnTime.tm_hour = 23;
}

void decreaseStartTimeHour() {
  timerStart.tm_hour--;
  if(timerStart.tm_hour<0) timerOnTime.tm_hour = 0;
}

void increaseStartTimeMinute() {
  timerStart.tm_min += 5;
  if(timerStart.tm_min>55) timerStart.tm_min = 55;
}

void decreaseStartTimeMinute() {
  timerStart.tm_min -= 5;
  if(timerStart.tm_min<0) timerStart.tm_min = 0;
}

void increaseOnTimeHour() {
  timerOnTime.tm_hour++;
  if(timerOnTime.tm_hour>8) timerOnTime.tm_hour = 8;
}

void decreaseOnTimeHour() {
  timerOnTime.tm_hour--;
  if(timerOnTime.tm_hour<0) timerOnTime.tm_hour = 0;
}

void increaseOnTimeMinute() {
  timerOnTime.tm_min += 15;
  if(timerOnTime.tm_min>45) timerOnTime.tm_min = 45;
}

void decreaseOnTimeMinute() {
  timerOnTime.tm_min -= 15;
  if(timerOnTime.tm_min<0) timerOnTime.tm_min = 0;
}

int clickAreas[][2][2] = {
  {{0,0},{0,0}},
  {{0,0},{0,0}},
  {{0,0},{0,0}},
  {{0,0},{0,0}},
  {{0,0},{0,0}},
  {{0,0},{0,0}},
  {{0,0},{0,0}},
  {{0,0},{0,0}}
};
bool clickedArea[] = {false, false, false, false, false,false, false, false};
int clickAreaIndex = 0;
int clearClickCounter = 0;
void motorTimerHandleClick(int x, int y) {
  Serial.print("Examine click: "); Serial.print(x); Serial.print(","); Serial.println(y);
  clearClickCounter = 0;
  for(int i=0; i<8; i++) {
    if((x>=clickAreas[i][0][0] && x<=clickAreas[i][1][0]) &&
       (y>=clickAreas[i][0][1] && y<=clickAreas[i][1][1]) ) {
      Serial.print("Hit: "); Serial.println(i);
      clickedArea[i] = true;
      callbacks[i]();
    } else {
      Serial.print("No hit: ("); Serial.print(clickAreas[i][0][0]); Serial.print(","); Serial.print(clickAreas[i][0][1]); Serial.print(") ,("); Serial.print(clickAreas[i][1][0]); Serial.print(","); Serial.print(clickAreas[i][1][1]); Serial.println(")");
      clickedArea[i] = false;
    }
  }
  saveTimerConfig();
}

int drawUpArrow(int x, int y) {
  if(clickedArea[clickAreaIndex]) {
      gfx.setColor(MINI_BLUE);
  } else {
      gfx.setColor(MINI_WHITE);
  }
  gfx.fillTriangle(x,y+8, x+8,y, x+16,y+8);
  clickAreas[clickAreaIndex][0][0] = x-16;
  clickAreas[clickAreaIndex][0][1] = y-8;
  clickAreas[clickAreaIndex][1][0] = x+16+16;
  clickAreas[clickAreaIndex][1][1] = y+18;
  // gfx.drawRect(clickAreas[clickAreaIndex][0][0], clickAreas[clickAreaIndex][0][1], 48, 26); //debug frame for the clickarea
  clickAreaIndex++;
  return y+8;
}

int drawDownArrow(int x, int y) {
    if(clickedArea[clickAreaIndex]) {
      gfx.setColor(MINI_BLUE);
  } else {
      gfx.setColor(MINI_WHITE);
  }
  gfx.fillTriangle(x,y, x+8,y+8, x+16,y);
  clickAreas[clickAreaIndex][0][0] = x-16;
  clickAreas[clickAreaIndex][0][1] = y-10;
  clickAreas[clickAreaIndex][1][0] = x+16+16;
  clickAreas[clickAreaIndex][1][1] = y+16;
  // gfx.drawRect(clickAreas[clickAreaIndex][0][0], clickAreas[clickAreaIndex][0][1], 48, 26); //debug frame for the clickarea
  clickAreaIndex++;
  return y+8;
}

int drawTimerStartTime(int startHeight) {
  char time_str[11];
  
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setColor(MINI_YELLOW);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(0, startHeight-3, "Start\ntime");

  drawUpArrow(50, startHeight); //above hours
  startHeight = drawUpArrow(101, startHeight); //above minutes
  
  if (IS_STYLE_12HR) {
    int hour = (timerStart.tm_hour+11)%12+1;  // take care of noon and midnight
    sprintf(time_str, "%2d:%02d",hour, timerStart.tm_min);
  } else {
    sprintf(time_str, "%02d:%02d",timerStart.tm_hour, timerStart.tm_min);
  }
  
  gfx.setFont(ArialRoundedMTBold_36);
  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(35, startHeight-2, time_str);

  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setFont(ArialMT_Plain_10);
  gfx.setColor(MINI_BLUE);
  if (IS_STYLE_12HR) {
    sprintf(time_str, "%s", timerStart.tm_hour>=12?"PM":"AM");
    gfx.drawString(195, startHeight, time_str); 
  }
  
  startHeight += 36 + 2;

  drawDownArrow(50, startHeight);
  startHeight = drawDownArrow(101, startHeight);
}

int drawTimerOnTime(int startHeight) {
  char time_str[11];

  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setColor(MINI_YELLOW);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(0, startHeight-3, "On\ntime");

  drawUpArrow(50, startHeight); //above hours
  startHeight = drawUpArrow(113, startHeight); //above minutes

  sprintf(time_str, "%02dh%02dm",timerOnTime.tm_hour, timerOnTime.tm_min);
  gfx.setFont(ArialRoundedMTBold_36);
  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(35, startHeight-2, time_str);
  startHeight += 36 + 2;

  drawDownArrow(50, startHeight);
  startHeight = drawDownArrow(113, startHeight);
  return startHeight;
}

int drawTimerState(int startHeight) {
  char time_str[11];
  time_t left;
  gfx.setFont(ArialRoundedMTBold_36);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    
  if(isTimerOn()) {
    left = secondsUntilOff();
    gfx.setColor(MINI_YELLOW);
    gfx.drawString(0, startHeight, "On");
  } else {
    left = secondsUntilOn();
    gfx.setColor(MINI_BLUE);
    gfx.drawString(0, startHeight, "Off");
  }
  
  gfx.setFont(ArialRoundedMTBold_36);
  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  sprintf(time_str, "%02d:%02d:%02d", left/3600, left/60%60, left%60);
  gfx.drawString(SCREEN_WIDTH, startHeight, time_str);
    
  startHeight += 36;
  return startHeight;
}

time_t getStartTime() {
  time_t startTime = timerStart.tm_hour * 3600 + timerStart.tm_min * 60;
  time_t endTime = timerOnTime.tm_hour * 3600 + timerOnTime.tm_min * 60;
  
  time_t nowTime = dstAdjusted.time(NULL);
  struct tm * midnight = localtime(&nowTime);
  
  if(startTime+endTime >= 24*3600) {
    time_t timeIntoNextDay = startTime+endTime - 24*3600;
    if(midnight->tm_hour*3600 + midnight->tm_min*60 + midnight->tm_sec < timeIntoNextDay) {
      nowTime -=  24*3600;
      midnight = localtime (&nowTime);
      nowTime +=  24*3600;;
    }
  }
  
  midnight->tm_sec = 0;
  midnight->tm_min = 0;
  midnight->tm_hour = 0;
  return mktime(midnight) + startTime;
}

bool isTimerOn() {
  time_t startTime = getStartTime();
  time_t endTime = startTime + timerOnTime.tm_hour * 3600 + timerOnTime.tm_min * 60;
  time_t nowTime = dstAdjusted.time(NULL);
  return (nowTime>=startTime && nowTime<=endTime);
}

time_t secondsUntilOff() {
  time_t startTime = getStartTime();
  time_t endTime = startTime + timerOnTime.tm_hour * 3600 + timerOnTime.tm_min * 60;
  time_t nowTime = dstAdjusted.time(NULL);
  return endTime-nowTime;
}

time_t secondsUntilOn() {
  time_t startTime = getStartTime();
  time_t nowTime = dstAdjusted.time(NULL);
  if(startTime<nowTime) startTime += 24 * 3600;
  return startTime-nowTime;
}

void drawMotorTimer(int y) {
  clickAreaIndex = 0;
  gfx.setTransparentColor(MINI_BLACK);
  gfx.setColor(MINI_WHITE);
  gfx.drawLine(0,y,SCREEN_WIDTH,y);

  y += 8;
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setColor(MINI_BLUE);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(160, y-3, "MOTOR\nHEATER\nPLUG");

  y = drawTimerStartTime(y);
  y += 16;
  y = drawTimerOnTime(y);
  y += 8;
  y = drawTimerState(y);
  
  if(clearClickCounter == 5) {
    clearClickCounter++;
    for(int i=0; i<8; i++) {
      clickedArea[i] = false;
    }
  } else if(clearClickCounter < 5) {
    clearClickCounter++;
  }
}
