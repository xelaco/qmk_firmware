// cd && cd qmk_firmware/ && qmk flash -kb xelaco/niu_mini -km xelaco
// 2022 09 20

#include QMK_KEYBOARD_H

#define LAYER_BASE 0
#define LAYER_LEFT 1
#define LAYER_RIGHT 2

#define XC_ENT RSFT_T(KC_ENT)
#define XC_LSFT LSFT_T(KC_TAB)
#define XC_QUOT RCTL_T(KC_QUOT)
#define LAYLEFT MO(LAYER_LEFT)
#define LAYRGHT MO(LAYER_RIGHT)

uint8_t mod_state;
int is_ctrl_fn_activated = 0;
int is_xshiftx_activated = 0;
int xshiftx_state = 0;

enum custom_keycodes
{
  CTRL_FN = SAFE_RANGE, XRGBTOG, X_COUL1, X_COUL2, RGBMONO, RGBRNBW, ACCENT0, ACCENT1, ACCENT2, ACCENT3, XSHIFTX, XCONTENT, XNICE
}; // 0 à | 1 ê | 2 é | 3 è

#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunknown-attributes"
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_ortho_4x12(
      KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      CTRL_FN, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, XC_QUOT,
      XC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XC_ENT,
      KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, LAYLEFT, KC_RALT, KC_SPC,  LAYRGHT, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
      ),
  [LAYER_LEFT] = LAYOUT_ortho_4x12(
      KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,
      _______, ACCENT0, ACCENT1, KC_DEL,  ACCENT2, KC_CIRC, KC_TILD, ACCENT3, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
      _______, _______, _______, XCONTENT,_______, _______, XNICE,   _______, _______, _______, _______, _______,
      _______, XRGBTOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
      ),
  [LAYER_RIGHT] = LAYOUT_ortho_4x12(
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
      _______, X_COUL1, X_COUL2, RGBMONO, RGBRNBW, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_PGUP,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN,
      _______, XSHIFTX, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT
      )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch(keycode)
  {
    case KC_A ... KC_Z:
      if(is_xshiftx_activated && record->event.pressed)
      {
        xshiftx_state = !xshiftx_state;
        if(xshiftx_state)
        {
          tap_code16(S(keycode));
          return false;
        }
      }
      return true;
      break;
    case XC_ENT:
      if(is_xshiftx_activated && record->event.pressed)
        xshiftx_state = 0;
      return true;
      break;
    case KC_BSPC:
      if(is_xshiftx_activated && record->event.pressed)
        xshiftx_state = !xshiftx_state;
      return true;
      break;
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
    case XCONTENT:
      if(record->event.pressed)
        SEND_STRING(":content:\n");
      break;
    case XNICE:
      if(record->event.pressed)
        SEND_STRING(":nice:\n");
      break;
    case XRGBTOG:
      if(record->event.pressed)
        rgblight_toggle();
      break;
    case X_COUL1:
      if(record->event.pressed)
        rgblight_decrease_hue();
      break;
    case X_COUL2:
      if(record->event.pressed)
        rgblight_increase_hue();
      break;
    case RGBMONO:
      if(record->event.pressed)
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case RGBRNBW:
      if(record->event.pressed)
        rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL + 1);
      break;
    case XSHIFTX:
      if(record->event.pressed)
      {
        is_xshiftx_activated = !is_xshiftx_activated;
        if(is_xshiftx_activated)
        {
          rgblight_enable_noeeprom();
          rgblight_mode_noeeprom(RGBLIGHT_MODE_KNIGHT);
          xshiftx_state = 0;
        }
        else
        {
          rgblight_disable_noeeprom();
        }
        return false;
      }
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

