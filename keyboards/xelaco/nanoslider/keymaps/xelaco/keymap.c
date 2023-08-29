// cd && cd qmk_firmware/ && qmk flash -kb xelaco/nanoslider -km xelaco
// 2021 12 15

#include QMK_KEYBOARD_H
#include "analog.h"
#include "qmk_midi.h"

#define LAYER_1 0
#define LAYER_2 1

bool alt_tab_state = false;
uint16_t alt_tab_timer = 0;
bool ctrl_tab_state = false;
uint16_t ctrl_tab_timer = 0;

int prev_pos = 0; // Previous position of the slider
uint16_t slider_timer = 0;
int xel_rgb_state = 0; // To change mode: RGB off, plain, swirl
uint16_t xel_rgb_timer = 0;

enum custom_keycodes
{
  ALT_TAB = SAFE_RANGE, CTRL_TAB, XEL_RGB
};

enum
{
  XEL_PLAY_NEXT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_1] = LAYOUT(
        TG(LAYER_2),
        CTRL_TAB,  XEL_RGB,   A(KC_S),
        ALT_TAB,   TD(XEL_PLAY_NEXT), C(S(KC_ESC)), A(KC_F1)
    ),
    [LAYER_2] = LAYOUT(
        TG(LAYER_2),
        C(KC_F13), C(KC_F18), C(KC_F17),
        C(KC_F14), C(KC_F15), C(KC_F16), QK_BOOT
    )
};

void keyboard_post_init_user(void)
{
  prev_pos = analogReadPin(SLIDER_PIN);
  rgblight_disable_noeeprom();
  xel_rgb_state = 0;
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
    case XEL_RGB:
      if(timer_elapsed(xel_rgb_timer) < 500)
        break;
      xel_rgb_timer = timer_read();
      switch(xel_rgb_state)
      {
        case 0:
          rgblight_enable_noeeprom();
          rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
          xel_rgb_state++;
          break;
        case 1:
          rgblight_enable_noeeprom();
          rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL + 1);
          xel_rgb_state++;
          break;
        default:
          rgblight_disable_noeeprom();
          xel_rgb_state = 0;
          break;
      }
      break;
  }
  return true;
}

void rgb_slider(void)
{
  if(timer_elapsed(slider_timer) < 1000)
    return;
  if(abs(prev_pos - analogReadPin(SLIDER_PIN)) > 16)
  {
    prev_pos = analogReadPin(SLIDER_PIN);
    rgblight_sethsv_noeeprom(prev_pos / 4, rgblight_get_sat(), rgblight_get_val());
    slider_timer = timer_read();
  }
}

void zoom_slider(void)
{
  if(timer_elapsed(slider_timer) < 1000)
    return;
  if(abs(prev_pos - analogReadPin(SLIDER_PIN)) > 100)
  {
    prev_pos = analogReadPin(SLIDER_PIN);
    //char str[10];
    //sprintf(str, "%d\n", (int)sqrt(prev_pos)*6/32);
    //send_string(str);
    SEND_STRING(SS_LCTL("0"));
    for(int i = 0; i < (int)sqrt(prev_pos) * 6 / 32; i++)
      SEND_STRING(SS_LCTL("+"));
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
  if(layer_state == 2)    // Layer 1
    rgb_slider();
  else
    zoom_slider();
}

void play_next_choice(tap_dance_state_t *state, void *user_data)
{
  switch(state->count)
  {
    case 1:
      tap_code(KC_MEDIA_PLAY_PAUSE);
      break;
    case 2:
      tap_code(KC_MEDIA_NEXT_TRACK);
      break;
    case 3:
      tap_code(KC_MEDIA_PREV_TRACK);
      break;
    default:
      tap_code(KC_MEDIA_PLAY_PAUSE);
  }
}

tap_dance_action_t tap_dance_actions[] = {
  [XEL_PLAY_NEXT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, play_next_choice, NULL),
};

