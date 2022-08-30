// ========= PINS =========
// IR
#define pinIR 2
// HL1606 LED STRIP
#define STRIP_D 9
#define STRIP_C 10
#define STRIP_L 11
#define numleds 32
// GROVE TM1637 DIGIT DISPLAY
#define DIG_CLK 6
#define DIG_DIO 7
// Buzzer and button and LED pins
#define buzzer 12
#define button1 4
#define LED_EXTRA 14

// Game parameter defines
#define MIN_GAME_TIME 3
#define MAX_GAME_TIME 10
#define CONTROL_MAX_HP 8
#define DESTROY_MIN_HP 8
#define DESTROY_MAX_HP 128
#define PRE_GAME_DELAY 15000
#define LONG_PRESS_MS 600

// LASERX IR CODES:
#define IR_RED 0xB40074AA // Red team
#define IR_BLUE 0x42F90701 // Blue team
#define IR_RENE 0xB40074A9 // Renegade team

// Game team defines
#define NO_TEAM 0
#define RED_TEAM 1
#define BLUE_TEAM 2
#define RENE_TEAM 3

// animation defines
#define ANIM_IDLE1 0
#define ANIM_IDLE2 1
#define ANIM_IDLE3 2
#define ANIM_ATTACK1 3
#define ANIM_ATTACK2 4
#define ANIM_TAKEOVER 5
#define ANIM_RAINBOW 6

// game name defines
#define GAME_CONTROL 1
#define GAME_DESTROY 2
#define GAME_TEST 3
//#define GAME_SOUND 4
#define NUMBER_OF_GAMES 3

// selector method defines
#define SELECT_TIME 1
#define SELECT_HP 2
#define SELECT_GAME 3
#define SELECT_TEAM 4

// global game variables
bool gameon;
bool sound;
uint8_t game_type;
uint8_t animation;
uint8_t idle_style;
uint8_t frame;
uint8_t owning_team;
uint8_t prev_owner;
uint8_t hp;
uint8_t max_hp;
uint8_t attacker;
uint8_t button_hold;
bool control_change;
short blue_points;
short red_points;
short rene_points;
uint8_t minutes;
unsigned long gamestarttime;
unsigned long controltime;
unsigned long gamelength;