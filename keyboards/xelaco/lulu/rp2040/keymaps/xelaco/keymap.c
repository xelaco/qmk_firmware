// cd && cd qmk_firmware/ && qmk flash -kb xelaco/lulu/rp2040 -km xelaco
// 2022 09 25

#include QMK_KEYBOARD_H

#define LAYER_BASE 0
#define LAYER_LEFT 1
#define LAYER_RIGHT 2

#define XC_ENT RSFT_T(KC_ENT)
#define LAYLEFT MO(LAYER_LEFT)
#define LAYRGHT MO(LAYER_RIGHT)

uint8_t layer_sword[3] = {0};
uint8_t mod_state;
int is_ctrl_fn_activated = 0;

enum custom_keycodes
{
  CTRL_FN = SAFE_RANGE, ACCENT0, ACCENT1, ACCENT2, ACCENT3
}; // 0 à | 1 ê | 2 é | 3 è

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
      KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_GRV,
      KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC, KC_RBRC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XC_ENT,
                        KC_LALT, LAYLEFT, KC_LGUI, KC_ENT,  KC_SPC,  KC_BSLS, LAYRGHT, KC_RALT
      ),
  [LAYER_LEFT] = LAYOUT(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
      KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,
      _______, ACCENT0, ACCENT1, KC_DEL,  ACCENT2, KC_CIRC, KC_TILD, ACCENT3, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                        _______, _______,  _______, _______, _______,  _______, _______, _______
      ),
  [LAYER_RIGHT] = LAYOUT(
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
      _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, _______, _______, _______, _______, _______,
      _______, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_PGUP,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN,
                        _______, _______, _______, _______, _______,  _______, _______, QK_BOOT
      )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch(keycode)
  {
    case ACCENT0:  //à
      if(record->event.pressed)
      {
        mod_state = get_mods();
        if (mod_state & MOD_MASK_SHIFT)
        {
          del_mods(MOD_MASK_SHIFT);
          SEND_STRING(SS_TAP(X_GRV)"A");
          set_mods(mod_state);
        }
        else
          SEND_STRING(SS_TAP(X_GRV)"a");
      }
      return false;
      break;
    case ACCENT1:  //ê
      if(record->event.pressed)
        SEND_STRING(SS_LSFT(SS_TAP(X_6))"e");
      return false;
      break;
    case ACCENT2:  //é
      if(record->event.pressed)
      {
        if (get_mods() & MOD_MASK_SHIFT)
        {
          del_mods(MOD_MASK_SHIFT);
          SEND_STRING(SS_TAP(X_QUOT)"E");
          set_mods(mod_state);
        }
        else
          SEND_STRING(SS_TAP(X_QUOT)"e");
      }
      return false;
      break;
    case ACCENT3:  //è
      if(record->event.pressed)
      {
        if (get_mods() & MOD_MASK_SHIFT)
        {
          del_mods(MOD_MASK_SHIFT);
          SEND_STRING(SS_TAP(X_GRV)"E");
          set_mods(mod_state);
        }
        else
          SEND_STRING(SS_TAP(X_GRV)"e");
      }
      return false;
      break;
    case CTRL_FN:
      if(record->event.pressed)
      {
        register_code(KC_LCTL);
        is_ctrl_fn_activated = 1;
      }
      else
      {
        unregister_code(KC_LCTL);
        is_ctrl_fn_activated = 0;
      }
      break;
    case KC_RALT:
      if(record->event.pressed)
      {
        if(is_ctrl_fn_activated)
        {
          unregister_code(KC_LCTL);
          register_code(KC_ENT);
        }
        else
        {
          register_code(KC_RALT);
        }
      }
      else{
        unregister_code(KC_RALT);
        unregister_code(KC_ENT);
      }
      return false;
      break;
    case LAYLEFT:
      if(record->event.pressed)
      {
        if(is_ctrl_fn_activated)
        {
          unregister_code(KC_LCTL);
          register_code(KC_BSPC);
          return false;
        }
        else
        {
          return true;
        }
      }
      else
      {
        unregister_code(KC_BSPC);
      }
      break;
    default:
      return true;
  }
  return true;
};

void keyboard_post_init_user(void) {
  layer_sword[LAYER_BASE] = 0;
  layer_sword[LAYER_LEFT] = 1;
  layer_sword[LAYER_RIGHT] = 2;
}

void oled_sword(uint8_t index) {
  static const char PROGMEM swords[][512] = {
    // Desperate
    {
      0x00, 0x00, 0xfc, 0x04, 0x04, 0x04, 0x8c, 0xf8, 0x00, 0x00, 0xe0, 0x50, 0x50, 0x50, 0x60, 0x00,
      0x30, 0x50, 0x50, 0xd0, 0x00, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x00, 0xe0, 0x50, 0x50, 0x50, 0x60,
      0x00, 0xf0, 0x10, 0x10, 0x00, 0x80, 0x50, 0x50, 0x50, 0xe0, 0x10, 0xfc, 0x10, 0x10, 0x00, 0xe0,
      0x50, 0x50, 0x50, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,
      0x01, 0x01, 0x01, 0x01, 0x00, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
      0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
      0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
      0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xe0, 0xf8, 0xfc, 0x84, 0x84,
      0x84, 0x8c, 0x88, 0xc8, 0x88, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x30, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00,
      0x02, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
      0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
      0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
      0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
      0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0f, 0x0f, 0x0f,
      0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0xff, 0xff, 0x3f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x1f, 0x0f, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
      0x07, 0x0f, 0x07, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0d, 0x07, 0x02,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    // Sandalphon
    {
      0x00, 0x00, 0x98, 0x24, 0x24, 0x24, 0xc8, 0x00, 0x80, 0x50, 0x50, 0x50, 0xe0, 0x00, 0xf0, 0x10,
      0x10, 0x10, 0xe0, 0x00, 0xe0, 0x10, 0x10, 0x10, 0xfe, 0x00, 0x80, 0x50, 0x50, 0x50, 0xe0, 0x00,
      0xfe, 0x00, 0x00, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x00, 0xfe, 0x10, 0x10, 0x10, 0xe0, 0x00, 0xe0,
      0x10, 0x10, 0x10, 0xe0, 0x00, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x81, 0x81, 0xc1, 0xc0, 0xc0, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1, 0xc0,
      0xc0, 0xc0, 0xc1, 0xc0, 0xc0, 0xc1, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe0,
      0xe1, 0xe0, 0xe0, 0xe7, 0xe1, 0xe1, 0xe1, 0xe0, 0xe0, 0xe1, 0xe0, 0xa0, 0xa0, 0xa1, 0xa0, 0xa0,
      0xa1, 0xb1, 0xb1, 0xb0, 0xb0, 0xb1, 0xb0, 0xb0, 0xb0, 0x71, 0x70, 0xf0, 0xf0, 0xb0, 0x70, 0x70,
      0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
      0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x38, 0xd8, 0xe8, 0x6f, 0x7f, 0x7f,
      0xff, 0xcc, 0xd8, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00,
      0x02, 0x07, 0x07, 0x0f, 0x0a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
      0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a,
      0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
      0x2d, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x6f, 0x77, 0x77, 0x7a, 0x7d, 0x6f, 0x77, 0x77,
      0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7,
      0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xe7, 0xdf, 0xb8, 0xb7, 0xf7, 0xf7,
      0xf8, 0x9f, 0xdf, 0x1f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
      0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0f, 0x0f, 0x02, 0x07, 0x02,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1f, 0x3f,
      0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    // Excalibur
    {
      0x00, 0x00, 0xfc, 0x24, 0x24, 0x24, 0x24, 0x00, 0x10, 0xa0, 0x40, 0xa0, 0x10, 0x00, 0xe0, 0x10,
      0x10, 0x10, 0x00, 0x80, 0x50, 0x50, 0x50, 0xe0, 0x00, 0xfe, 0x00, 0x00, 0xf2, 0x00, 0x00, 0xfe,
      0x10, 0x10, 0x10, 0xe0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00, 0xf0, 0x10, 0x10, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0,
      0xb0, 0x70, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x01, 0x01, 0x81, 0x81, 0x81, 0x80, 0xc1, 0xc0, 0xc0, 0xc0, 0xc1, 0xc0, 0xc0, 0xc1,
      0xc1, 0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1, 0xc0, 0xc0, 0xc1, 0xc0, 0xc0, 0xc1,
      0xc1, 0xc1, 0xc1, 0xc0, 0xc0, 0xc0, 0xc1, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1, 0xc0, 0xc0, 0xc0, 0xc0,
      0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
      0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
      0xc0, 0xc0, 0xc0, 0xc0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xc0, 0xc0, 0x40, 0x40, 0x40, 0xc1,
      0xe7, 0xff, 0x7c, 0x83, 0xfe, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x02, 0x07, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
      0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
      0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
      0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
      0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
      0x1f, 0x1f, 0x1d, 0x18, 0x10, 0x12, 0x12, 0x12, 0x12, 0x12, 0x1a, 0x1f, 0x12, 0x12, 0x17, 0x1d,
      0x3d, 0xff, 0xf7, 0x0f, 0xff, 0x1f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
      0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x02,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3c,
      0x6f, 0x77, 0x79, 0x1e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }};
    oled_write_raw_P(swords[index], 512 * sizeof(char));
}

void oled_logo(void) {
  static const char PROGMEM logo[] = {
    0x00, 0xc0, 0x64, 0x22, 0xfe, 0x88, 0x04, 0x62, 0xfc, 0x94, 0x8a, 0x46, 0x00, 0x80, 0xfc, 0x87,
    0x40, 0x40, 0xf8, 0x84, 0x42, 0xf2, 0x4e, 0x00, 0x60, 0xfc, 0x84, 0x82, 0x46, 0x60, 0xf8, 0x84,
    0x82, 0x62, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0xff, 0xff, 0x00, 0x08, 0xf8, 0xc0,
    0x88, 0xf8, 0xf8, 0x80, 0x81, 0xff, 0xff, 0x00, 0x08, 0xf8, 0xc0, 0x88, 0xf8, 0xf8, 0x80, 0x00,
    0xff, 0xff, 0xff, 0x3f, 0x0f, 0x0f, 0x07, 0x07, 0x00, 0x00, 0x07, 0x00, 0x00, 0x07, 0x07, 0x00,
    0x00, 0x07, 0x07, 0x00, 0x00, 0x07, 0x00, 0x00, 0x07, 0x07, 0x0f, 0x0f, 0x3f, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x00, 0x00, 0x80, 0x40, 0x38, 0xf0, 0xc0,
    0x00, 0x00, 0x08, 0xf8, 0xf8, 0x00, 0x00, 0xd8, 0xc8, 0x00, 0x00, 0xc0, 0x40, 0x40, 0xc0, 0x00,
    0x00, 0x40, 0xc0, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x78, 0xc8, 0xc8, 0xc8, 0xb8, 0x00, 0x00,
    0xd8, 0xc8, 0x00, 0x08, 0xf8, 0xf8, 0x00, 0x00, 0xc0, 0x40, 0x40, 0xc0, 0x00, 0x40, 0xc0, 0x80,
    0x40, 0xc0, 0xc0, 0x00, 0x40, 0xe0, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x60, 0x10, 0x10, 0xd0, 0x30,
    0x00, 0xc0, 0xf0, 0x10, 0x10, 0xe0, 0x00, 0xc0, 0xf0, 0x38, 0x08, 0x08, 0x88, 0xb8, 0x80, 0x00,
    0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x64, 0x64,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x07, 0x05, 0x01, 0x05, 0x07,
    0x06, 0x04, 0x04, 0x07, 0x07, 0x00, 0x04, 0x07, 0x07, 0x00, 0xc2, 0x46, 0x45, 0x45, 0xc7, 0x04,
    0x04, 0x06, 0x07, 0xc5, 0x86, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04, 0x84, 0xc3, 0x40, 0x44,
    0x47, 0xc7, 0x00, 0x04, 0x07, 0x07, 0x00, 0x01, 0x07, 0x05, 0x05, 0x05, 0x40, 0xc4, 0xc7, 0x07,
    0x00, 0x07, 0x07, 0x04, 0x00, 0x07, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00,
    0x00, 0x01, 0x07, 0x04, 0x04, 0x03, 0x00, 0x00, 0x03, 0x06, 0x04, 0x44, 0xc4, 0xc7, 0x00, 0x00,
    0x26, 0x26, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0e, 0x0c, 0x08, 0x18, 0xff,
    0xff, 0x18, 0x08, 0x0c, 0x0e, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x26, 0x26,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x13, 0x26, 0x26, 0x26, 0x1d, 0x00,
    0x20, 0x3c, 0x2a, 0x09, 0x2f, 0x3e, 0x30, 0x20, 0x00, 0x00, 0x00, 0x06, 0x1f, 0x31, 0x20, 0x20,
    0x24, 0x3d, 0x04, 0x18, 0x3e, 0x22, 0x22, 0x3e, 0x1c, 0x18, 0x3e, 0x22, 0x22, 0x3f, 0x3f, 0x20,
    0x34, 0x2e, 0x2a, 0x3a, 0x10, 0x82, 0xfe, 0xfe, 0x22, 0x36, 0x1c, 0x00, 0x08, 0x3e, 0x2a, 0x2a,
    0x2e, 0x00, 0x08, 0x3e, 0x2a, 0x2a, 0x2e, 0x00, 0x18, 0x3e, 0x22, 0x22, 0x3f, 0x3f, 0x20, 0x00,
    0xff, 0xff, 0xff, 0xfc, 0xf0, 0xf0, 0xe0, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0xe0, 0x00,
    0x00, 0xe0, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0xe0, 0xf0, 0xf0, 0xfc, 0xff, 0xff, 0xff
  };
    oled_write_raw_P(logo, 512 * sizeof(char));
}

void oled_layer(void) {
  uint8_t highest_layer = get_highest_layer(layer_state);
  switch (highest_layer) {
    case LAYER_BASE:
    case LAYER_LEFT:
    case LAYER_RIGHT:
      oled_sword(layer_sword[highest_layer]);
      break;
    default:
      oled_logo();
  }
}

bool oled_task_user(void)
{
  if (is_keyboard_master())
    oled_layer();
  else
    oled_logo();
  return false;
}

