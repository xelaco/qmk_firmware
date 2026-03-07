#include QMK_KEYBOARD_H
#include "analog.h"
#include "qmk_midi.h"

#define LAYER 0

bool alt_tab_state = false;
uint16_t alt_tab_timer = 0;
bool ctrl_tab_state = false;
uint16_t ctrl_tab_timer = 0;

int slider_prev_position = 0;
uint16_t slider_timer = 0;

enum custom_keycodes
{
  ALT_TAB = SAFE_RANGE, CTRL_TAB
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER] = LAYOUT(
      QK_BOOT,
      KC_CAPS, C(S(KC_ESC)), CTRL_TAB,
      KC_MPRV, KC_MPLY,      KC_MNXT, ALT_TAB
      )
};

void keyboard_post_init_user(void)
{
  slider_prev_position = analogReadPin(SLIDER_PIN);
  rgblight_disable_noeeprom();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch(keycode)
  {
    case ALT_TAB:
      if(record->event.pressed)
      {
        if(!alt_tab_state)
        {
          alt_tab_state = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      }
      else
        unregister_code(KC_TAB);
      break;
    case CTRL_TAB:
      if(record->event.pressed)
      {
        if(!ctrl_tab_state)
        {
          ctrl_tab_state = true;
          register_code(KC_LCTL);
        }
        ctrl_tab_timer = timer_read();
        register_code(KC_TAB);
      }
      else
        unregister_code(KC_TAB);
      break;
  }
  return true;
}

void rgb_slider(void)
{
  if(timer_elapsed(slider_timer) < 1000)
    return;
  if(abs(slider_prev_position - analogReadPin(SLIDER_PIN)) > 16)
  {
    slider_prev_position = analogReadPin(SLIDER_PIN);
    rgblight_sethsv_noeeprom(slider_prev_position / 4, rgblight_get_sat(), rgblight_get_val());
    slider_timer = timer_read();
  }
}

void matrix_scan_user(void)
{
  if(alt_tab_state)
  {
    if(timer_elapsed(alt_tab_timer) > 1000)
    {
      unregister_code(KC_LALT);
      alt_tab_state = false;
    }
  }
  if(ctrl_tab_state)
  {
    if(timer_elapsed(ctrl_tab_timer) > 1000)
    {
      unregister_code(KC_LCTL);
      ctrl_tab_state = false;
    }
  }
  rgb_slider();
}

bool led_update_user(led_t led_state) {
  if(led_state.caps_lock) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  } else {
    rgblight_disable_noeeprom();
  }
  return true;
}
