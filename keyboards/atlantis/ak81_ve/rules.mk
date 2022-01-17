# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Keyboard RGB underglow
RGB_MATRIX_ENABLE = yes		# Enable RGB matrix effects
RGB_MATRIX_DRIVER = WS2812
AUDIO_ENABLE = no           # Audio output
DYNAMIC_MACRO_ENABLE = yes
ENCODER_ENABLE = yes