// cd && cd qmk_firmware/ && qmk flash -kb xelaco/pegasushoof -km xelaco
// 2021 12 15

#include QMK_KEYBOARD_H

#define KM_XELQWERTY 0
#define KM_XELBATTLE 1
#define KM_XELFN 2

#define XC_ALT A(KC_TAB)
#define XC_ENT LCTL_T(KC_ENT)

void init_game(void);
void init_battle_grid(char gr[][3]);
void generate_boat(char gr[][3]);
void show_grid(void);
void atk_player(int square_x, int square_y);
void atk_computer(void);

char player[3][3] = {{'0','0','0'},{'0','0','0'},{'0','0','0'}}, computer[3][3] = {{'0','0','0'},{'0','0','0'},{'0','0','0'}};
int seed = 0;
char score_player = 0, score_computer = 0;
int ai_ordi[2] = {-1, 0};

enum custom_keycodes
{
    XELINIT = SAFE_RANGE, BATTLE1, BATTLE2, BATTLE3, BATTLE4, BATTLE5, BATTLE6, BATTLE7, BATTLE8, BATTLE9, XELSHOW
    /* Battleships
     * BATTLE1 | BATTLE2 | BATTLE3
     * BATTLE4 | BATTLE5 | BATTLE6
     * BATTLE7 | BATTLE8 | BATTLE9
     */
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [KM_XELQWERTY] = LAYOUT(                                                                                                                                    \
    KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SLCK, KC_PAUS, \
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP, \
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN, \
    MO(2),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          XC_ENT,                                \
    KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,            \
    KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, KC_RGUI, TG(1),   KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [KM_XELBATTLE] = LAYOUT(                                                                                                                                    \
    XELINIT,          BATTLE1, BATTLE2, BATTLE3, BATTLE4, BATTLE5, BATTLE6, BATTLE7, BATTLE8, BATTLE9, _______, _______, XELSHOW,    KC_WH_D, KC_BTN3, KC_WH_U, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    KC_BTN1, KC_MS_U, KC_BTN2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    KC_MS_L, KC_MS_D, KC_MS_R, \
    MO(2),   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                               \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             _______,          \
    _______, _______, _______,                            _______,                            _______, _______, TG(1),   _______,    _______, _______, _______  \
    ),
    [KM_XELFN] = LAYOUT(                                                                                                                                        \
    RESET,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, KC_CLCK, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,     _______, _______, _______, \
    _______, C(KC_Q), C(KC_W), C(KC_E), C(KC_R), C(KC_T), C(KC_Y), C(KC_U), C(KC_O), C(KC_P), _______, _______, _______, _______,    _______, _______, _______, \
    KC_TRNS, C(KC_A), C(KC_S), C(KC_D), C(KC_F), C(KC_G), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, KC_GRV,           _______,                               \
    _______, _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), C(KC_B), C(KC_N), C(KC_M), _______, _______, _______,          _______,             _______,          \
    _______, _______, KC_BSPC,                            KC_ENT,                             _______, _______, _______, _______,    _______, _______, _______  \
    )
};

uint32_t layer_state_set_user(uint32_t state)
{
    switch (biton32(state))
    {
        case KM_XELBATTLE:
            ph_sclk_led_on();
            break;
        default:
            ph_sclk_led_off();
            break;
    }
    return state;
}

void led_set_user(uint8_t usb_led)
{
    if(usb_led & (1 << USB_LED_CAPS_LOCK))
    {
        ph_caps_led_on();
    }
    else
    {
        ph_caps_led_off();
    }
    return;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode)
    {
    case XELINIT:
        if(record->event.pressed)
            init_game();
        break;
    case XELSHOW:
        if(record->event.pressed)
            show_grid();
        break;
    case BATTLE1:
        if(record->event.pressed)
          atk_player(0, 0);
        break;
    case BATTLE2:
        if(record->event.pressed)
          atk_player(0, 1);
        break;
    case BATTLE3:
        if(record->event.pressed)
          atk_player(0, 2);
        break;
    case BATTLE4:
        if(record->event.pressed)
          atk_player(1, 0);
        break;
    case BATTLE5:
        if(record->event.pressed)
          atk_player(1, 1);
        break;
    case BATTLE6:
        if(record->event.pressed)
          atk_player(1, 2);
        break;
    case BATTLE7:
        if(record->event.pressed)
          atk_player(2, 0);
        break;
    case BATTLE8:
        if(record->event.pressed)
          atk_player(2, 1);
        break;
    case BATTLE9:
        if(record->event.pressed)
          atk_player(2, 2);
        break;
    }
    return true;
}

void init_game(void)
{
    if(seed == 0)
    {
        srandom(timer_read());
        seed = 1;
    }
    score_player = 0;
    score_computer = 0;
    ai_ordi[0] = -1;
    ai_ordi[1] = 0;
    init_battle_grid(player);
    init_battle_grid(computer);
    SEND_STRING(SS_LCTRL("a"));
    SEND_STRING(SS_TAP(X_BSPACE));
    SEND_STRING("You can play! F1-F9 to fire! F12 to show grid!\n");
    return;
}

void init_battle_grid(char gr[][3])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            gr[i][j] = '-';
        }
    }
    generate_boat(gr);
    generate_boat(gr);
}

void generate_boat(char gr[][3])
{
    int error = 1, column = 0, row = 0;
    do
    {
        error = 0;
        row = random() % 3;
        column = random() % 3;
        if(random() % 2)
        {
            if(row + 2 > 3)
            {
                error = 1;
            }
            else
            {
                if(gr[row][column] != '-' || gr[row+1][column] != '-')
                {
                    error = 1;
                }
                if(error == 0)
                {
                    gr[row][column] = 'B';
                    gr[row+1][column] = 'B';
                }
            }
        }
        else
        {
            if(column + 2 > 3)
            {
                error = 1;
            }
            else
            {
                if(gr[row][column] != '-' || gr[row][column+1] != '-')
                {
                    error = 1;
                }
                if(error == 0)
                {
                    gr[row][column] = 'B';
                    gr[row][column+1] = 'B';
                }
            }
        }
    }while(error);
    return;
}

void show_grid(void)
{
    SEND_STRING(SS_LCTRL("a"));
    SEND_STRING(SS_TAP(X_BSPACE));
    if(player[0][0] == '0')
    {
        SEND_STRING("Error: ESC to init!\n");
        return;
    }
    SEND_STRING("Battleships! (by xelaco *** March - 2019 ***)\n");
    SEND_STRING("  =============== \n");
    for(int x = 0; x < 3; x++)
    {
        SEND_STRING(" [ ");
        for(int y = 0; y < 3; y++)
        {
            switch(player[x][y])
            {
                case 'B':
                    SEND_STRING("B ");
                    break;
                case '-':
                    SEND_STRING("- ");
                    break;
                case '!':
                    SEND_STRING("! ");
                    break;
                case '@':
                    SEND_STRING("@ ");
                    break;
            }
        }
        SEND_STRING("| ");
        for(int y = 0; y < 3; y++)
        {
            switch(computer[x][y])
            {
                case 'B':
                case '-':
                    SEND_STRING("? ");
                    break;
                case '!':
                    SEND_STRING("! ");
                    break;
                case '@':
                    SEND_STRING("@ ");
                    break;
            }
        }
        SEND_STRING("] \n");
    }
    SEND_STRING("  =============== \n");
    if(score_player == 4 && score_computer == 4)
    {
        SEND_STRING("### Draw! ###\n");
    }
    else if(score_player == 4)
    {
        SEND_STRING("### Player has won! ###\n");
    }
    else if(score_computer == 4)
    {
        SEND_STRING("### Computer has won! ###\n");
    }
    return;
}

void atk_player(int square_x, int square_y)
{
    if(player[0][0] == '0')
    {
        SEND_STRING("Error: ESC to init!\n");
        return;
    }
    if(score_player == 4 || score_computer == 4)
    {
        show_grid();
    }
    else if(computer[square_x][square_y] == 'B')
    {
        computer[square_x][square_y] = '!';
        score_player++;
        atk_computer();
        show_grid();
    }
    else if(computer[square_x][square_y] == '-')
    {
        computer[square_x][square_y] = '@';
        atk_computer();
        show_grid();
    }
    return;
}

void atk_computer(void)
{
    int error = 0, row = -1, column = -1;
    do
    {
        if(ai_ordi[0] > -1)
        {
            switch(error)
            {
                case 0:
                    column = ai_ordi[0] % 10;
                    row = (ai_ordi[0] - column) / 10 - 1;
                    break;
                case 1:
                    column = ai_ordi[0] % 10 + 1;
                    row = (ai_ordi[0] - column) / 10 + 1;
                    if(column > 2)
                    {
                        column = -1;
                    }
                    break;
                case 2:
                    column = ai_ordi[0] % 10;
                    row = (ai_ordi[0] - column) / 10 + 1;
                    if(row > 2)
                    {
                        row = -1;
                    }
                    break;
                case 3:
                    column = ai_ordi[0] % 10 - 1;
                    row = (ai_ordi[0] - column + 1) / 10;
                    break;
                default:
                    ai_ordi[0] = -1;
                    row = -1;
                    column = -1;
            }
        }
        else
        {
            row = random() % 3;
            column = random() % 3;
        }
        if(row < 0 || row > 2 || column < 0 || column > 2)
        {
            error++;
        }
        else if(player[row][column] == '!' || player[row][column] == '@')
        {
            error++;
        }
        else
        {
            error = 0;
        }
    }while(error);
    switch(player[row][column])
    {
        case 'B':
        player[row][column] = '!';
        score_computer++;
        ai_ordi[0] = row * 10 + column;
        ai_ordi[1] = 1;
        break;
        default:
        player[row][column] = '@';
        ai_ordi[1]--;
        if(ai_ordi[1] == -3)
        {
            ai_ordi[0] = -1;
        }
        break;
    }
    return;
}

