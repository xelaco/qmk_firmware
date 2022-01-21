#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID 0x03A8
#define PRODUCT_ID 0x0000
#define DEVICE_VER 0x0001
#define MANUFACTURER keebwerk.
#define PRODUCT nano. slider
#define DESCRIPTION Powered by qmk

/* key matrix size */
#define MATRIX_ROWS 2
#define MATRIX_COLS 4

/* key matrix pins */
#define MATRIX_ROW_PINS { F0, F1 }
#define MATRIX_COL_PINS { B0, B1, B2, B3 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* number of backlight levels */

#ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 0
#endif

/* Set 0 if debouncing isn't needed */
#define DEBOUNCING_DELAY 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* prevent stuck modifiers */
#define PREVENT_STUCK_MODIFIERS

#define RGB_DI_PIN F6
#ifdef RGB_DI_PIN
//#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLED_NUM 4
#define RGBLIGHT_HUE_STEP 16
#define RGBLIGHT_SAT_STEP 32
#define RGBLIGHT_VAL_STEP 32
#endif

#define MIDI_ADVANCED

#define SLIDER_PIN D4

#define TAPPING_TERM 200

#endif
