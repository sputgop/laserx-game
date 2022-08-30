// "Destroy" game mode
// Shoot the point to destroy the base

void destroymode_play() {
  // prep game state  
  gameon = true;    
  hp = max_hp;
  frame = 0;
  animation = idle_style = ANIM_IDLE3;
  gamestarttime = millis();
  button_hold = 0;
  
  // IR clear and turn on status LED
  clear_IR();
  digitalWrite(LED_EXTRA, HIGH);

  // START PLAYING !!!
  destroymode_loop();

  // End game
  digitalWrite(LED_EXTRA, LOW);
  noTone(buzzer);
  //digits.displayStr("End");
  gamelength = millis() - gamestarttime;  

  digits.point(true);
  displaytime(gamelength);

  owning_team = NO_TEAM;
  animation = ANIM_IDLE3;
  frame = 0;

  while (digitalRead(button1) == HIGH) {   
    frame++;
    animate();
    delay(5);
  }

  beep();

  digits.point(false);
}

void destroymode_loop() {
  while (gameon)
  {              
    destroymode_frame();

    check_button();

    if (hp <= 0) {
      // End game if 0 HP

      // Serial.println(gamestarttime);
      // Serial.println(millis());
      // Serial.println(gamelength);
      // Serial.println(((millis() - gamestarttime) < gamelength));

      for (int i = 0; i < 250; i++) {        
        strip_random(get_team_color(owning_team));

        if (sound == true) {tone(buzzer, 100 + random(350), 10);}

        delay(10);
      }  

      noTone(buzzer);
      

      gameon = false;
    }

  }  
}

void destroymode_frame(){
  unsigned long now = millis();

  // Check if new IR protocol data is available
  if (IRLremote.available() && animation != ANIM_TAKEOVER)
  {    
    destroymode_attack(get_laser_team());
  }

  // set digit display on first idle frame
  if (animation == idle_style && frame == 0) {
    digits.displayNum(hp);
  }

  // increase animation
  frame++;

  animate();  
  
  if (animation != idle_style) {
    while (millis() - now < 10) {
        // normal 100 FPS on non-idle
      }
  }
  else {
    uint8_t delay_time = ((hp * 15) / max_hp);
    while (millis() - now < delay_time) {
      // faster idle FPS the lower HP there is
      // from 15ms delay to 1ms
    }
  }  
}


void destroymode_attack(uint8_t attacking_team) {
  if (attacking_team == NO_TEAM) { return; }

  attacker = attacking_team;

  // ensure control point not already owned
  if (owning_team != attacking_team) {
    // set damage
    //attacker = attacking_team;
    hp--;

    //if (hp % 2 == 0) { digits.displayStr("0000"); }
    //else { digits.displayStr("----"); }

    digits.displayNum(hp);

    if (hp > 0) {
      // Set new attack animation and frame
      animation = ANIM_ATTACK2;      
      frame = 0;
    }
    
  }
}