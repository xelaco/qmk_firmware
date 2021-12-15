// cd && cd qmk_firmware/ && qmk flash -kb xelaco/niu_mini -km xelaco
// 2021 04 13

#include QMK_KEYBOARD_H

#define LAYER_BASE 0
#define LAYER_LEFT 1
#define LAYER_RIGHT 2

#define XC_ENT RSFT_T(KC_ENT)
#define XC_LSFT LSFT_T(KC_TAB)
#define XC_QUOT LCTL_T(KC_QUOT)
#define XC_SPAC RALT_T( KC_SPC)
#define LAYLEFT MO(LAYER_LEFT)
#define LAYRGHT MO(LAYER_RIGHT)

int is_ctrl_fn_activated = 0;

enum custom_keycodes{
  CTRL_FN = SAFE_RANGE, XRGBTOG, X_COUL1, X_COUL2, RGBMONO, RGBRNBW
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_BASE] = LAYOUT_ortho_4x12(
        KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        CTRL_FN, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, XC_QUOT,
        XC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XC_ENT,
        KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, LAYLEFT, KC_SPC,  XC_SPAC, LAYRGHT, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),
    [LAYER_LEFT] = LAYOUT_ortho_4x12(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,
        _______, _______, _______, _______, _______, KC_CIRC, KC_TILD, _______, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_UP,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [LAYER_RIGHT] = LAYOUT_ortho_4x12(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, X_COUL1, X_COUL2, RGBMONO, RGBRNBW, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_PGUP,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,  KC_PGDN,
        _______, XRGBTOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET
    )
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case XC_SPAC:
            return TAPPING_TERM + 5800;
        default:
            return TAPPING_TERM;
    }
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case XC_SPAC:
            return true;
        default:
            return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record){
    switch(keycode){
        case XRGBTOG:
            if(record->event.pressed){
                rgblight_toggle_noeeprom();
            }
            break;
        case X_COUL1:
            if(record->event.pressed){
                rgblight_decrease_hue_noeeprom();
            }
            break;
        case X_COUL2:
            if(record->event.pressed){
                rgblight_increase_hue_noeeprom();
            }
            break;
        case RGBMONO:
            if(record->event.pressed){
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            break;
        case RGBRNBW:
            if(record->event.pressed){
                rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL + 1);
            }
            break;
        case CTRL_FN:
            if(record->event.pressed){
                register_code(KC_LCTL);
                is_ctrl_fn_activated = 1;
            }
            else{
                unregister_code(KC_LCTL);
                is_ctrl_fn_activated = 0;
            }
            break;
        case KC_BSPC:
            if(record->event.pressed){
                if(is_ctrl_fn_activated){
                    unregister_code(KC_LCTL);
                    register_code(KC_DEL);
                }
                else{
                    register_code(KC_BSPC);
                }
            }
            else{
                unregister_code(KC_BSPC);
                unregister_code(KC_DEL);
            }
            return false;
            break;
        case KC_SPC:
            if(record->event.pressed){
                if(is_ctrl_fn_activated){
                    unregister_code(KC_LCTL);
                    register_code(KC_ENT);
                }
                else{
                    register_code(KC_SPC);
                }
            }
            else{
                unregister_code(KC_SPC);
                unregister_code(KC_ENT);
            }
            return false;
            break;
        case LAYLEFT:
            if(record->event.pressed){
                if(is_ctrl_fn_activated){
                    unregister_code(KC_LCTL);
                    register_code(KC_BSPC);
                    return false;
                }
                else{
                    return true;
                }
            }
            else{
                unregister_code(KC_BSPC);
            }
            break;
        default:
            return true;
    }
    return true;
};

