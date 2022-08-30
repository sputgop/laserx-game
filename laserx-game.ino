/*
LaserX game
Leif Bjornstad 2022
*/

// Include extra file
#include "laserx-game-variables.h"
//#include "pitches.h"

// Using IRLremote library manager as I only need to read IR
// This library tested more accurately for me and less overhead
// https://github.com/NicoHood/IRLremote
#include "IRLremote.h"
CHashIR IRLremote; // Hash (unknown) mode

// HL1606 driven LED strip
// https://github.com/adafruit/HL1606-LED-Strip
#include "HL1606strip.h"
HL1606strip strip = HL1606strip(STRIP_D, STRIP_L, STRIP_C, numleds);

// Digit display library for TM1637 driven 4 digit display
// https://github.com/Seeed-Studio/Grove_4Digital_Display
#include "TM1637.h"
TM1637 digits(DIG_CLK, DIG_DIO);

void setup()
{
  blackout();

  //while (true) {rainbow_break(2,1);}  

  // sound on by default
  sound = true;

  // Start serial debug output
  //while (!Serial);
  //Serial.begin(115200);
  //Serial.println(F("Initializing..."));

  // Set LED/buzzer to output
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(buzzer, OUTPUT);
  pinMode(LED_EXTRA, OUTPUT);
  noTone(buzzer);

  // Set buttons to input pullup
  pinMode(button1, INPUT_PULLUP);

  // set up Digit display
  digits.init();
  digits.set(7); // 7 is brightest

  digits.displayStr(" HI ");

  rainbow_break(2, 2);

  // Start reading the remote. PinInterrupt or PinChangeInterrupt* will automatically be selected
  if (!IRLremote.begin(pinIR))
    Serial.println(F("You did not choose a valid pin."));
}


void loop()
{
  // CODE TO SELECT GAME
  game_type = get_number(SELECT_GAME);

  if (game_type == GAME_TEST) {
    testmode_play();
  }  
  /*else if (game_type == GAME_SOUND) {
    if (sound) {
      sound = false;
      digits.displayStr("0FF");
    } else {
      sound = true;
      digits.displayStr("0n");
    }
  }*/
  else if (game_type == GAME_CONTROL) {  
    // CODE TO SET LENGTH OF GAME
    minutes = get_number(SELECT_TIME);
    pre_game();
    //delay(PRE_GAME_DELAY);
    controlmode_play();
  }
  else if (game_type == GAME_DESTROY) {
    max_hp = get_number(SELECT_HP);
    owning_team = get_number(SELECT_TEAM);
    pre_game();
    //delay(PRE_GAME_DELAY);
    destroymode_play();
  }    
  
  colorWipe(WHITE, 5);
  delay(500);
}


// Just clear IR "memory" before game or when needed
void clear_IR(){
  while (IRLremote.available()) {
    auto data = IRLremote.read();
  }
}

// wait for input press then countdown
void pre_game() {
  strip_fill(GREEN);
  delay(1000);
  
  digits.displayStr("----");  

  while (digitalRead(button1) == HIGH) { delay(50); }

  beep();

  unsigned long now = millis();

  while (millis() - now < PRE_GAME_DELAY) {
    displaytime(PRE_GAME_DELAY - (millis() - now) + 1000);
    if ((((millis() - now) / 500) % 2) == 0) {
      strip_fill(GREEN);
    } else {
      blackout();
    }
    delay(10);
  }
  digits.clearDisplay();
  blackout();
}

// display an ouch
void ouch() {
  uint8_t ouchtype = random(1, 5);
  digits.point(false);
  switch (ouchtype) {
    case 1:
      digits.displayStr("OUCH");
      break;
    case 2:
      digits.displayStr("00F");
      break;
    case 3:
      digits.displayStr("n000");
      break;
/*    case 4:
      digits.displayStr("Ouch");
      break;
    case 5:
      digits.displayStr("bAH");
      break;*/        
    default:
      digits.displayStr("AAAH");  
  }
}


// Function for player to input something
// game type or whatnot
uint8_t get_number(uint8_t select_type) {
  uint8_t button1state;
  uint8_t min_num, max_num;
  uint8_t idle_col;

  if (select_type == SELECT_TIME) {
    min_num = MIN_GAME_TIME;
    max_num = MAX_GAME_TIME;
  }
  else if (select_type == SELECT_HP) {
    min_num = DESTROY_MIN_HP;
    max_num = DESTROY_MAX_HP;
  }
  else if (select_type == SELECT_GAME) {
    min_num = 1;
    max_num = NUMBER_OF_GAMES;
  }
  else if (select_type == SELECT_TEAM) {
    min_num = 1;
    max_num = 2;
  }

  uint8_t temp_number = min_num;
  bool selected = false;
  unsigned long presstime = 0;  

  // Initial display number
  //digits.point(true);
  //displaystrdigits(str, temp_number);
  //strip_num(WHITE, number);

  if (select_type == SELECT_TIME) {
    digits.displayStr("LEn");
    idle_col = BLUE;    
  }
  else if (select_type == SELECT_HP) {
    digits.displayStr(" HP");
    idle_col = RED;
  }

  if (select_type != SELECT_GAME && select_type != SELECT_TEAM) {
    colorWipe(idle_col, 10);
    delay(1000);
    digits.displayNum(temp_number);
  }  

  frame = 1;

  // Loop until a long press locks in selection
  while (selected == false) {
    button1state = digitalRead(button1);

    if (select_type == SELECT_GAME) {
      if (temp_number == GAME_CONTROL) {
        idle_col = TEAL;
        digits.displayStr("CAP");
      }
      else if (temp_number == GAME_DESTROY) {
        idle_col = RED;
        digits.displayStr("bASE");
      }
      else if (temp_number == GAME_TEST) {
        idle_col = VIOLET;
        digits.displayStr("tESt");
      }
      /*
      else if (temp_number == GAME_SOUND) {
        idle_col = YELLOW;
        digits.displayStr("Snd");
      }*/
    } else if (select_type == SELECT_TEAM) {
      if (temp_number == RED_TEAM) {
        idle_col = RED;
        digits.displayStr("rEd");
      }
      else if (temp_number == BLUE_TEAM) {
        idle_col = BLUE;
        digits.displayStr("bLUE");
      }
    }

    // button pressed
    if (button1state == LOW) {      
      strip_fill(WHITE);

      // set timer for long press
      if (presstime == 0) {
        beep();
        presstime = millis();
        }

      // determine long press, then exit loop if so
      if (millis() - presstime > LONG_PRESS_MS) {
        selected = true;
        strip_fill(GREEN);
      }

      // to prevent bouncing
      delay(50);
    }
    // button released AND not after a long press
    else if (selected == false) {
      //Serial.print(F("RELEASED "));
      
      strip_loop(idle_col, 8);
      frame++;
      if (frame > 128) {frame = 1;}

      // if press time is > 0 we know we're coming off a hot press
      if (presstime > 0) {        
        // INCREMENT/ROLLOVER NUMBER
        if (select_type == SELECT_HP) {
          // HP doubles otherwise just +1
          temp_number = temp_number * 2;
        } else { temp_number++; }        

        if (temp_number > max_num) { temp_number = min_num; }
        if (temp_number < min_num) { temp_number = min_num; }
        presstime = 0;
        //Serial.print(F("NUMBER: "));
        //Serial.println(temp_number);

        // DISPLAY NUMBER
        digits.displayNum(temp_number);
      }
    }

    // Delay for 5ms for animate
    delay(5);
  }

  // delay after selection
  delay(1000);
  blackout();

  //digits.point(false);
  digits.clearDisplay();
  return temp_number;
}


void displaytime(unsigned long ms) {
//void displaytimeleft() {
  //unsigned long ms;
  float time;

  // calculate milliseconds left
  //ms = gamelength - (millis() - gamestarttime);

  if (ms == 0) {
    digits.displayNum(0);
    return;
  }

  // seconds are mod 60 (divide 100 for decimal point)
  // minutes are divide 60
  time = (float((ms / 1000) % 60) / 100) + float((ms / 1000) / 60);
  
  digits.displayNum(time, 2);

  //Serial.println(time);
}


void animate() {
  switch (animation) {
    case ANIM_IDLE1:
      strip_loop(get_team_color(owning_team), hp * 2);
      break;
    case ANIM_IDLE2:
      strip_loop(get_team_color(owning_team), 8);
      break;
    case ANIM_IDLE3:      
      if (frame <= 64) {
        strip_spread(get_team_color(owning_team), 64);
      } else {
        strip_spread(BLACK, 64);
      }
      if (frame > 128) { frame = 0; }
      break;
    case ANIM_ATTACK1:
      attack_tone();
      strip_flash_hp(get_team_color(attacker), get_team_color(owning_team));      

      if (frame >= 48) {
        // End animation
        noTone(buzzer);
        animation = idle_style;
        frame = 0;
        blackout();
      }
      break;
    case ANIM_ATTACK2:
      attack_tone();
      strip_flash(get_team_color(attacker), get_team_color(owning_team));

      if (frame >= 48) {
        // End animation
        noTone(buzzer);
        animation = idle_style;
        frame = 0;
        blackout();
      }
      break;
    case ANIM_RAINBOW:
      strip_rainbow();
      break;
    case ANIM_TAKEOVER:
      if (frame <= 100) {
        strip_slow_fill(get_team_color(owning_team));
        if ((frame / 10) % 2 == 1) {tone(buzzer, 523, 100);}
        else (noTone(buzzer));
      }
      if (frame > 100) {
        strip_flash(BLACK, get_team_color(owning_team));
        tone(buzzer, 523 + ((frame - 100) * 2));
      }
      if (frame >= 200) {
        // End animation
        noTone(buzzer);
        animation = idle_style;
        frame = 0;
        digitalWrite(LED_EXTRA, HIGH);
        blackout();
      }
  }
}


// Set tone based on frame and attacking team
void attack_tone() {
  if (sound) {
    switch (attacker) {
      case RED_TEAM:
        tone(buzzer, (frame * 25) + 100 + random(0, 100), 25);
        break;
      case BLUE_TEAM:
        tone(buzzer, 1300 - (frame * 25) + random(0, 100), 25);
        break;
      case RENE_TEAM:
        tone(buzzer, random(600- (frame * 12), 600 + (frame * 12)), 25);
        break;
    }
  }
}


// convert IR laser data to a team integer
uint8_t get_laser_team() {
  uint8_t team;

  // Light Led
  digitalWrite(LED_BUILTIN, HIGH);

  // Get data  
  auto data = IRLremote.read();

  Serial.println(data.command);

  switch(data.command) {
    case IR_RED:
      Serial.println(F("RED"));
      team = RED_TEAM;
      break;
    case IR_BLUE:
      Serial.println(F("BLUE"));
      team = BLUE_TEAM;
      break;
    case IR_RENE:
      Serial.println(F("RENEGADE"));
      team = RENE_TEAM;
      break;
    default:
      team = NO_TEAM;
  }

  // Turn Led off after printing the data
  digitalWrite(LED_BUILTIN, LOW);

  return team;
}

void beep() {
  if (sound == true) {tone(buzzer, 523, 50);}
}

void check_button() {
    if (digitalRead(button1) == LOW) {
      button_hold += 1;
    } else {
      button_hold = 0;
    }

    if (button_hold == 1) {beep();}

    // holding button for 200 frame end game
    if (button_hold > 200) {gameon = false;}
}

// Reset function
// void(* resetFunc) (void) = 0;