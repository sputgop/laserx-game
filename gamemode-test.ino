// test mode is just for testing out shooting
void testmode_play() {
  gameon = true;
  frame = 0;
  hp = 4;
  blue_points = 0;
  animation = idle_style = ANIM_RAINBOW;
  owning_team = NO_TEAM;

  // IR clear and turn on status LED
  clear_IR();
  digitalWrite(LED_EXTRA, HIGH);

  // game loop
  testmode_loop();

  // End game
  digitalWrite(LED_EXTRA, LOW);
  noTone(buzzer);
}

void testmode_loop() {
  while (gameon == true) {
    testmode_frame();

    if (digitalRead(button1) == LOW) {
      beep();
      gameon = false;
    }
  }
}

void testmode_frame() {
  unsigned long now = millis();

  // Check if new IR protocol data is available
  if (IRLremote.available())
  {
    // Get the new data from the remote
    testmode_attack(get_laser_team());
  }

  frame++;  
  animate();

  if (animation == idle_style) {
    if (frame > 100) {frame = 0;}
    digits.displayNum(blue_points);
  }

  while (millis() - now < 10) {
    // do nothing!
  }
}

void testmode_attack(uint8_t attacking_team) {
  if (attacking_team == NO_TEAM) { return; }

  attacker = attacking_team;
  animation = ANIM_ATTACK2;
  frame = 0;
  blue_points++;
  ouch();
}