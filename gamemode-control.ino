// Point control mode
// "Capture" the point by shooting it, once it is captured you get points every second
// Whichever team has highest points wins

void controlmode_play(){
  // prep game state
  blue_points = red_points = rene_points = 0;
  gameon = true;
  gamelength = 60000 * minutes;
  owning_team = attacker = NO_TEAM;
  hp = CONTROL_MAX_HP;
  frame = 0;
  idle_style = animation = ANIM_IDLE1;
  control_change = false;
  gamestarttime = millis();      
  prev_owner = NO_TEAM;
  button_hold = 0;

  // IR clear and turn on status LED
  clear_IR();
  digitalWrite(LED_EXTRA, HIGH);

  // Enter game loop
  controlmode_loop();

  // End game
  digitalWrite(LED_EXTRA, LOW);
  digits.point(false);
  digits.displayStr("End");  
  
  rainbow_break(2, 5);

  uint8_t winning_color = BLACK;
  short winning_points = blue_points;  

  while (digitalRead(button1) == HIGH) {
    if (blue_points == red_points && blue_points == rene_points) {
      digits.displayStr("----");
    }
    else if (blue_points >= red_points && blue_points >= rene_points) {
      digits.displayStr("bLUE");
      winning_color = BLUE;
    }
    else if (red_points >= blue_points && red_points >= rene_points) {
      digits.displayStr("rEd");
      winning_color = RED;
      winning_points = red_points;
    }    
    else if (rene_points >= red_points && rene_points >= blue_points) {
      digits.displayStr("grn");
      winning_color = GREEN;
      winning_points = rene_points;
    }

    colorWipe(winning_color, 20);
    colorWipe(WHITE, 20);    

    digits.displayNum(winning_points);

    colorWipe(winning_color, 20);
    colorWipe(WHITE, 20);
  }

  beep();

  rainbow_break(2, 5);
}

void controlmode_loop() {
  while (gameon)
  {              
    controlmode_frame();

    check_button();

    if ((millis() - gamestarttime) > gamelength) {
      // end game if timer past, but ensure points are recorded
      // Serial.println(gamestarttime);
      // Serial.println(millis());
      // Serial.println(gamelength);
      // Serial.println(((millis() - gamestarttime) < gamelength));
      
      control_change = true;
      gameon = false;
    }

    if (control_change) {        
      // 1 point for every 1 second of control time
      short points = (millis() - controltime) / 1000;

      switch (prev_owner) {
        case BLUE_TEAM:
          blue_points += points;
          break;
        case RED_TEAM:
          red_points += points;
          break;
        case RENE_TEAM:
          rene_points += points;
          break;
        // no points for NO_TEAM prev owner
      }
      

      // establish ownership
      prev_owner = owning_team;
      controltime = millis();
      control_change = false;
    }
  }  
}

void controlmode_frame(){
  unsigned long now = millis();

  // Check if new IR protocol data is available
  if (IRLremote.available() && animation != ANIM_TAKEOVER)
  {
    // New method: check and return laser team (if exists)
    controlmode_attack(get_laser_team());
  }

  // increase animation
  frame++;

  animate();

  if (animation == idle_style) {
    displaytime(gamelength - (millis() - gamestarttime));
  }

  // 10 ms frame? 100 FPS ?
  while (millis() - now < 10) {
      // idk how accurate it is
    }
}


void controlmode_attack(uint8_t attacking_team) {
  if (attacking_team == NO_TEAM) { return; }

  // ensure control point not already owned
  if (owning_team != attacking_team) {
    // set damage
    attacker = attacking_team;
    hp--;

    //Serial.print(F("HP: "));
    //Serial.print(hp);
    //Serial.print(F(" Owner: "));
    //Serial.print(owning_team);
    //Serial.print(F(" Attacker: "));
    //Serial.print(attacker);

    if (hp <= 0) {
      /*
      // Ownership goes to neutral in between (2x HP)
      if (owning_team == NO_TEAM) {
        owning_team = attacking_team;}
      else {owning_team = NO_TEAM;}
      */

      // Ownership immediately changes to attacking team
      owning_team = attacking_team;      
      control_change = true;
      animation = ANIM_TAKEOVER;
      frame = 0;
      digitalWrite(LED_EXTRA, LOW);
      digits.displayStr("C00L");
      blackout();

      // reset hp to full
      hp = CONTROL_MAX_HP;
    }
    else {
      // Set new attack animation and frame
      animation = ANIM_ATTACK1;
      frame = 0;

      ouch();
    }

    // clear buzzer just in case
    //noTone(buzzer);
  }
}
