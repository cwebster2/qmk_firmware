/* Copyright 2020 Casey Webster <casey@e1337.dev>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "raw_hid.h"
#define RAW_EPSIZE 8
#include <stdio.h>

#include "cwebster2.h"

char wpm_str[10];
uint16_t wpm_graph_timer = 0;

#ifdef COMBO_ENABLE
enum combos {
    ZX_COPY,
    CV_PASTE
};

const uint16_t PROGMEM copy_combo[]  = { KC_Z, KC_X, COMBO_END };
const uint16_t PROGMEM paste_combo[] = { KC_C, KC_V, COMBO_END };

combo_t key_combos[COMBO_COUNT] = {
    [ZX_COPY]  = COMBO(copy_combo, LCTL_T(KC_C)),
    [CV_PASTE] = COMBO(paste_combo, LCTL_T(KC_V))
};
#endif

enum custom_keycodes {
    KC_LCCL = SAFE_RANGE
};

enum layers {
    _QWERTY = 0,
    _COLEMAK,
    _GAME,
    _FN,
    _SYMBOLS,
    _NUM,
    _NAV,
    _MOUSE,
    _MEDIA
};

// shortcuts for certain keys to use LAYOUT_kc()
#define KC_CTLBS CTL_T(KC_BSPC)
#define KC_ALTCL LALT_T(KC_CAPS)

#define LAYOUT_kyria_base( \
    L01, L02, L03, L04, L05, R05, R04, R03, R02, R01, \
    L11, L12, L13, L14, L15, R15, R14, R13, R12, R11, \
    L21, L22, L23, L24, L25, R25, R24, R23, R22, R21,  \
              L33, L34, L35, R35, R34, R33 \
    ) \
    LAYOUT_kyria_wrapper ( \
    KC_GRV,   L01, L02, L03, L04, L05,                                      R05, R04, R03, R02, R01, KC_BSLS, \
    KC_CTLBS, L11, L12, L13, L14, L15,                                      R15, R14, R13, R12, R11, KC_QUOT, \
    KC_EQL,   L21, L22, L23, L24, L25, KC_LCCL, KC_LGUI, KC_ALTCL, KC_LSFT, R25, R24, R23, R22, R21, KC_MINS, \
         SCMD_T(KC_LBRC), C_S_T(KC_MINS), L33, L34, L35, R35, R34, R33, TO(_GAME), KC_PSCR \
    )
#define LAYOUT_kyria_base_wrapper(...)       LAYOUT_kyria_base(__VA_ARGS__)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 */
    [_QWERTY] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______QWERTY_L1______,                 _______QWERTY_R1______,
      _______QWERTY_L2______,                 _______QWERTY_R2______,
      _______QWERTY_L3______,                 _______QWERTY_R3______,
              _______THUMBS_L_______,  _______THUMBS_R_______
 /*           `---------------------'  `---------------------' */
    ),
    [_COLEMAK] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______COLEMAK_L1_____,                 _______COLEMAK_R1_____,
      _______COLEMAK_L2_____,                 _______COLEMAK_R2_____,
      _______COLEMAK_L3_____,                 _______COLEMAK_R3_____,
              _______THUMBS_L_______,  _______THUMBS_R_______
 /*           `---------------------'  `---------------------' */
    ),
 // GAME layout -- qwerty without homerow mods
    [_GAME] = LAYOUT_kyria_wrapper(
 /* ,-------------------------------------------.                              ,-------------------------------------------. */
    KC_GRV,   _______QWERTY_L1______,                                     _______QWERTY_R1______, KC_BSLS,
    KC_CTLBS, KC_A, KC_S, KC_D, KC_F, KC_G,                               KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
    KC_EQL,   _______QWERTY_L3______, KC_LCCL, KC_LGUI,                   KC_ALTCL, KC_LSFT, _______QWERTY_R3______, KC_MINS,
       SCMD_T(KC_LBRC), C_S_T(KC_MINS), _______THUMBS_L_______,   _______THUMBS_R_______, TO(_QWERTY), KC_PSCR
 /*                        `----------------------------------'  `----------------------------------' */
    ),
    [_FN] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______FN_______L1____,                 _______INACTIVE_R1____,
      _______FN_______L2____,                 _______INACTIVE_R2____,
      _______FN_______L3____,                 _______INACTIVE_R3____,
              _______FN________T____,  _______INACTIVE__T____
 /*           `---------------------'  `---------------------' */
      ),
    [_SYMBOLS] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______SYM______L1____,                 _______INACTIVE_R1____,
      _______SYM______L2____,                 _______INACTIVE_R2____,
      _______SYM______L3____,                 _______INACTIVE_R3____,
              _______SYM_______T____,  _______INACTIVE__T____
 /*           `---------------------'  `---------------------' */
    ),
    [_NUM] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______NUM______L1____,                 _______INACTIVE_R1____,
      _______NUM______L2____,                 _______INACTIVE_R2____,
      _______NUM______L3____,                 _______INACTIVE_R3____,
              _______NUM_______T____,  _______INACTIVE__T____
 /*           `---------------------'  `---------------------' */
      ),
    [_NAV] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______INACTIVE_R1____,                 _______NAV______R1____,
      _______INACTIVE_R1____,                 _______NAV______R2____,
      _______INACTIVE_R1____,                 _______NAV______R3____,
              _______INACTIVE__T____,  _______INACTIVE__T____
 /*           `---------------------'  `---------------------' */
      ),
    [_MOUSE] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______INACTIVE_R1____,                 _______MOUSE____R1____,
      _______INACTIVE_R1____,                 _______MOUSE____R2____,
      _______INACTIVE_R1____,                 _______MOUSE____R3____,
              _______INACTIVE__T____,  _______MOUSE_____T____
 /*           `---------------------'  `---------------------' */
      ),
    [_MEDIA] = LAYOUT_kyria_base_wrapper(
 /* ,-----------------------.                 ,-----------------------. */
      _______INACTIVE_R1____,                 _______MEDIA____R1____,
      _______INACTIVE_R1____,                 _______MEDIA____R2____,
      _______INACTIVE_R1____,                 _______MEDIA____R3____,
              _______INACTIVE__T____,  _______MEDIA_____T____
 /*           `---------------------'  `---------------------' */
     ),
};

static void send_layer_via_hid(int layer) {
    uint8_t data[RAW_EPSIZE];
    data[0] = 1;
    data[1] = layer;
    raw_hid_send((uint8_t*)data, sizeof(data));
    return;
}

/* RGB LIGHT LAYERS
const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 10, HSV_AZURE}
);

const rgblight_segment_t PROGMEM my_raise_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 10, HSV_RED}
);

const rgblight_segment_t PROGMEM my_lower_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 10, HSV_TURQUOISE}
);

const rgblight_segment_t PROGMEM my_adjust_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 10, HSV_PURPLE}
);
*/
void keyboard_post_init_rgb(void);

void keyboard_post_init_user(void) {
    rgblight_sethsv_noeeprom(HSV_BLUE);
    keyboard_post_init_rgb();
}


static const uint8_t PROGMEM layer_colors[][3] = {
    [_QWERTY] = {HSV_BLUE},
    [_COLEMAK] = {HSV_AZURE},
    [_GAME] = {HSV_RED},
    [_FN] = {HSV_PINK},
    [_SYMBOLS] = {HSV_GREEN},
    [_NUM] = {HSV_CORAL},
    [_NAV] = {HSV_GOLDENROD},
    [_MOUSE] = {HSV_TURQUOISE},
    [_MEDIA] = {HSV_MAGENTA}
};

void keyboard_post_init_rgb(void) {
/*#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_STARTUP_ANIMATION)*/
    /*if (userspace_config.rgb_layer_change) { rgblight_enable_noeeprom(); }*/
    /*if (rgblight_config.enable) {*/
        /*layer_state_set_user(layer_state);*/
        /*uint16_t old_hue = rgblight_config.hue;*/
        uint16_t old_hue = 170;
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        for (uint16_t i = 255; i > 0; i--) {
            rgblight_sethsv_noeeprom( ( i + old_hue) % 255, 255, 255);
            matrix_scan();
            wait_ms(10);
        }
    /*}*/
/*#endif*/
    /*layer_state_set_user(layer_state);*/
}

layer_state_t layer_state_set_user(layer_state_t state) {
    /*uint8_t layer = get_highest_layer(state);*/
    /*switch (get_highest_layer(state))*/
    /*{*/
    /*    case _QWERTY ... _MEDIA:*/
    /*        rgblight_sethsv_noeeprom(layer_colors[layer][0],layer_colors[layer][1],layer_colors[layer][2]);*/
    /*        send_layer_via_hid(layer);*/
    /*        break;*/
    /*}*/
    send_layer_via_hid(state);
    switch (get_highest_layer(state)) {
        case _QWERTY:
            rgblight_sethsv_noeeprom(HSV_BLUE);
            break;
        case _SYMBOLS:
            rgblight_sethsv_noeeprom(HSV_GREEN);
            break;
        case _NUM:
            rgblight_sethsv_noeeprom(HSV_CORAL);
            break;
        case _NAV:
            rgblight_sethsv_noeeprom(HSV_GOLDENROD);
            break;
        case _FN:
            rgblight_sethsv_noeeprom(HSV_PINK);
            break;
        case _MEDIA:
            rgblight_sethsv_noeeprom(HSV_MAGENTA);
            break;
        case _MOUSE:
            rgblight_sethsv_noeeprom(HSV_TURQUOISE);
            break;
        case _GAME:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
    }
    return state;
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_logo(void) {
    static const char PROGMEM logo[] = {
        // canvas is 128x64.  need 16 padding
        // 80x32
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x60, 0xe0, 0xf0, 0xfe, 0xfe, 0xf0, 0x60, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x7c, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x7c, 0xf8, 0xe0, 0x60, 0x20, 0x10, 0x10, 0x0c, 0x0f, 0x09, 0x08, 0x00, 0x09, 0x9b, 0x5c, 0x50, 0x10, 0x20, 0x40, 0xe0, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x78, 0x38, 0x38, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x17, 0x15, 0x3d, 0xef, 0xaf, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x10, 0x00, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x40, 0xa0, 0xbf, 0xff, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    oled_write_raw_P(logo, sizeof(logo));

    oled_advance_page(false);
    oled_advance_page(false);
    oled_advance_page(false);
    oled_advance_page(false);

#ifdef COMBO_ENABLE
    oled_write_P(PSTR("Combos enabled: "), false);
    if (is_combo_enabled()) {
        oled_write_P(PSTR("Yes\n"), false);
    } else {
        oled_write_P(PSTR("No\n"), false);
    }
#endif

    uint8_t mods = get_mods() | get_weak_mods();
    oled_write_P((mods & MOD_MASK_GUI) ? PSTR("GUI ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_ALT) ? PSTR("ALT ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_CTRL) ? PSTR("CTRL ") : PSTR("     "), false);
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHFT ") : PSTR("     "), false);
    oled_write_P(PSTR("\n"), false);

#ifdef WPM_ENABLE
    // Write WPM
    sprintf(wpm_str, "WPM: %03d", get_current_wpm());
    //oled_write_P(PSTR("\n"), false);
    oled_write(wpm_str, false);
#endif
}

static void render_qmk_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("Kyria rev1.3\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _SYMBOLS:
            oled_write_P(PSTR("Shifted Sym\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Numbers\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("Navigation\n"), false);
            break;
        case _FN:
            oled_write_P(PSTR("Fn\n"), false);
            break;
        case _GAME:
            oled_write_P(PSTR("Game\n"), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("Media keys\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("Mouse keys\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    uint8_t mods = get_mods() | get_weak_mods();
    oled_write_P((mods & MOD_MASK_GUI) ? PSTR("GUI ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_ALT) ? PSTR("ALT ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_CTRL) ? PSTR("CTRL ") : PSTR("     "), false);
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHFT ") : PSTR("     "), false);
    oled_write_P(PSTR("\n"), false);

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_logo();
    }
}
#endif


#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    // messages from host
    // add ability to set color
    raw_hid_send(data, length);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t my_colon_timer;

    switch (keycode) {
        case KC_LCCL:
            if (record->event.pressed) {
                my_colon_timer = timer_read();
                register_code(KC_LCTL);
            } else {
                unregister_code(KC_LCTL);
                if (timer_elapsed(my_colon_timer) < TAPPING_TERM) {
                    SEND_STRING(":");
                }
            }
            return false;
    }
    return true;
}
