// This returns color byte for LED strip, based on team integer
uint8_t get_team_color(uint8_t team)
{
  switch (team) {
    case NO_TEAM:
      return WHITE;
      break;
    case RED_TEAM:
      return RED;
      break;
    case BLUE_TEAM:
      return BLUE;
      break;
    case RENE_TEAM:
      return GREEN;
      break;
  }
}

// Simple animation wipe color
void colorWipe(uint8_t color, uint8_t wait) {
  uint8_t i;
  
  for (i=0; i < numleds; i++) {
      strip.setLEDcolor(i, color);      
      strip.writeStrip();   
      delay(wait);
  }
}

// Make LED strip all black
void blackout() {
  uint8_t i;
  
  for (i=0; i < numleds; i++) {
      strip.setLEDcolor(i, BLACK);           
  }

  strip.writeStrip();
}


void strip_flash_hp (uint8_t color1, uint8_t color2) {
  uint8_t i;

  for (i=0; i < numleds; i++) {
      if ((frame % 8) <= 4) {
        if (i % CONTROL_MAX_HP < hp) {
          strip.setLEDcolor(i, BLACK);
        }
        else
        {
          strip.setLEDcolor(i, color1);
        }
      }
      else {
        if (i % CONTROL_MAX_HP < hp) {
          strip.setLEDcolor(i, color2);
        }
        else {
          strip.setLEDcolor(i, BLACK);
        }
      }
      
  }

  strip.writeStrip();
}

void strip_flash (uint8_t color1, uint8_t color2) {
  uint8_t i;

  for (i=0; i < numleds; i++) {
      if ((frame % 8) <= 4) {
        strip.setLEDcolor(i, color1);
      }
      else {
        strip.setLEDcolor(i, color2);        
      }      
  }

  strip.writeStrip();
}


void strip_fill (uint8_t color) {
  uint8_t i;

  for (i=0; i < numleds; i++) {
    strip.setLEDcolor(i, color);      
  }

  strip.writeStrip();
}

/*
void strip_pong_hp (uint8_t color) {
  uint8_t actualframe = (frame % 128) / 2;

  uint8_t i, j, k;

  for (i=0; i < numleds; i++) {
    strip.setLEDcolor(i, BLACK);      
  }

  for (j = actualframe; j <= hp; j++) {

  }


  strip.writeStrip();
} */

void strip_loop(uint8_t color, uint8_t leds) {
  uint8_t actualframe = (frame % 128) / 4;
  uint8_t i;

  for (i=0; i < numleds; i++) {
    strip.setLEDcolor(i, BLACK);      
  }

  for (i=actualframe; i < actualframe + leds; i++) {
    if (i < numleds) {
      strip.setLEDcolor(i, color);
    }
    else {
      strip.setLEDcolor(i - numleds, color);
    }
  }

  strip.writeStrip();
}

//void strip_rainbow(uint8_t anim_frame) {
//void strip_rainbow(volatile uint8_t anim_frame) {

void rainbow_break(uint8_t ms, uint8_t loops) {
  frame = 1;  

  for (uint8_t i = 0; i < loops; i++) {
    frame = 1;
    while (frame < 100) {
      strip_rainbow();
      frame++;
      delay(ms);
    }
  }
}

void strip_rainbow() {
  uint8_t i;
  float col;
  uint8_t intcol;

  for (i=0; i < numleds; i++) {
    //col = ((float(i + 1) / 32) + (float(anim_frame) / 100));
    col = ((float(i + 1) / 32) + (float(frame) / 100));

    if (col > 1) { col = col - 1;}
    col = col * 12;

    intcol = round(col);

    switch (intcol) {
      case 1:
        strip.setLEDcolor(i, RED);
        break;
      case 2:
        strip.setLEDcolor(i, YELLOW);
        break;
      case 3:
        strip.setLEDcolor(i, GREEN);
        break;
      case 4:
        strip.setLEDcolor(i, TEAL);
        break;
      case 5:
        strip.setLEDcolor(i, BLUE);
        break;
      case 6:
        strip.setLEDcolor(i, VIOLET);
        break;
      case 7:
        strip.setLEDcolor(i, RED);
        break;
      case 8:
        strip.setLEDcolor(i, VIOLET);
        break;
      case 9:
        strip.setLEDcolor(i, BLUE);
        break;
      case 10:
        strip.setLEDcolor(i, TEAL);
        break;
      case 11:
        strip.setLEDcolor(i, GREEN);
        break;
      case 12:
        strip.setLEDcolor(i, YELLOW);
        break;
//      default:
//        strip.setLEDcolor(i, BLACK);   
    }
  }

  strip.writeStrip();
}

void strip_slow_fill(uint8_t color) {
  // 100 frame animation  
  uint8_t i, j;
  uint8_t actual_frame = frame;

  while (actual_frame > 100) {
    actual_frame = actual_frame - 100;
  }

  for (i = 0; i < ((actual_frame * 16) / 100); i++) {
    j = 4 + i - 1;
    strip.setLEDcolor(j, color);
    j = 36 - i;
    if (j >= 32) { j = j - 32;}
    strip.setLEDcolor(j, color);
  }

  strip.writeStrip();

}

void strip_spread(uint8_t color, uint8_t frame_count) {  
  uint8_t i, j;
  uint8_t actual_frame = frame;

  while (actual_frame > frame_count) {
    actual_frame = actual_frame - frame_count;
  }

  for (i = 0; i <= ((actual_frame * 16) / frame_count); i++) {
    j = 4 + i - 1;
    strip.setLEDcolor(j, color);
    j = 36 - i;
    if (j >= 32) { j = j - 32;}
    strip.setLEDcolor(j, color);
  }

  strip.writeStrip();

}

void strip_num(uint8_t color, uint8_t leds) {
  uint8_t i;

  for (i=0; i < numleds; i++) {
    if (i < leds) {
      strip.setLEDcolor(i, color);
    }
    else {
      strip.setLEDcolor(i, BLACK);      
    }
  }

  strip.writeStrip();
}

void strip_random(uint8_t color) {  
  uint8_t i;

  for (uint8_t j = 0; j < 4; j++) {
    i = random(0, numleds);
    strip.setLEDcolor(i, color);
  }
  for (uint8_t j = 0; j < 4; j++) {
    i = random(0, numleds);
    strip.setLEDcolor(i, BLACK);
  }
  strip.writeStrip();
}