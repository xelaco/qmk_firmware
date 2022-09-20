// cd && cd qmk_firmware/ && qmk flash -kb xelaco/lulu/rp2040 -km xelaco
// 2022 09 17

#include QMK_KEYBOARD_H

#define LAYER_BASE 0
#define LAYER_LEFT 1
#define LAYER_RIGHT 2

#define LAYLEFT MO(LAYER_LEFT)
#define LAYRGHT MO(LAYER_RIGHT)

#define RGBPLAIN RGB_MATRIX_SOLID_COLOR
#define RGBBEAN RGB_MATRIX_JELLYBEAN_RAINDROPS
#define RGBRAIN RGB_MATRIX_DIGITAL_RAIN
#define RGBFLOW RGB_MATRIX_PIXEL_FLOW

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
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC, KC_RBRC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                        KC_LALT, LAYLEFT, KC_LGUI, KC_ENT,  KC_SPC,  KC_BSLS, LAYRGHT, KC_RALT
      ),
  [LAYER_LEFT] = LAYOUT(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
      KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,
      _______, ACCENT0, ACCENT1, _______, ACCENT2, KC_CIRC, KC_TILD, ACCENT3, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                        _______, _______,  _______, _______, _______,  _______, _______, _______
      ),
  [LAYER_RIGHT] = LAYOUT(
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
      _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, _______, _______, _______, _______, _______,
      _______, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_PGUP,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN, _______, _______,
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

