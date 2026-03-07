#include QMK_KEYBOARD_H

enum layers {
  LAYER_BASE,
  LAYER_LEFT,
  LAYER_RIGHT
};

#define LAYLEFT MO(LAYER_LEFT)
#define LAYRGHT MO(LAYER_RIGHT)
#define XC_LSFT LSFT_T(KC_TAB)
#define XC_QUOT RCTL_T(KC_QUOT)

uint8_t mod_state;
float xel_music_cipher[][2] = SONG(XEL_MUSIC_CIPHER);
float xel_music_cl[][2] = SONG(XEL_MUSIC_CL);
float xel_music_startup[][2] = SONG(XEL_MUSIC_STARTUP);

enum custom_keycodes
{
  CTRL_FN = SAFE_RANGE, ACCENT0, ACCENT1, ACCENT2, ACCENT3, XMS_CIP, XMS_CL, XMS_STU
}; // 0 à | 1 ê | 2 é | 3 è

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_ortho_4x12(
      KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, XC_QUOT,
      XC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
      KC_LALT, KC_CAPS, KC_LGUI, KC_RALT, LAYLEFT, KC_ENT,  KC_SPC,  LAYRGHT, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
      ),
  [LAYER_LEFT] = LAYOUT_ortho_4x12(
      KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,
      _______, ACCENT0, ACCENT1, KC_DEL,  ACCENT2, KC_CIRC, KC_TILD, ACCENT3, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
      ),
  [LAYER_RIGHT] = LAYOUT_ortho_4x12(
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
      _______, XMS_CIP, XMS_CL,  XMS_STU, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_PGUP,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT
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
        mod_state = get_mods();
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
        mod_state = get_mods();
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
    case XMS_CIP:
      if(record->event.pressed)
      {
        PLAY_SONG(xel_music_cipher);
      }
      return false;
      break;
    case XMS_CL:
      if(record->event.pressed)
      {
        PLAY_SONG(xel_music_cl);
      }
      return false;
      break;
    case XMS_STU:
      if(record->event.pressed)
      {
        PLAY_SONG(xel_music_startup);
      }
      return false;
      break;
    default:
      return true;
  }
  return true;
};

bool led_update_user(led_t led_state) {
  if(led_state.caps_lock) {
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  } else {
    rgb_matrix_disable_noeeprom();
  }
  return true;
}

