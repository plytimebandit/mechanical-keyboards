/* The Honey and Milk layout for the Dactyl Manuform 5x6 Keyboard */

// https://docs.qmk.fm/#/keycodes_basic?id=basic-keycodes
// https://github.com/qmk/qmk_firmware/blob/master/quantum/keymap_extras/keymap_german.h
// https://docs.qmk.fm/#/ref_functions?id=software-timers

#include QMK_KEYBOARD_H
#include "keymap_german.h"


#define _QWERTZ 0
#define _RGB 1
#define _LOWER 2
#define _RAISE 3

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define RGB   MO(_RGB)

// custom keycodes
enum my_keycodes {
    MY_RGB_RST = SAFE_RANGE
};

// tap dance
enum {
    TD1
};
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for 1st, twice for 2nd
    [TD1] = ACTION_TAP_DANCE_DOUBLE(DE_TILD, DE_BSLS)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTZ] = LAYOUT_5x6(
     KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                             KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,DE_SS,
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                             DE_Z  , KC_U  , KC_I  , KC_O  , KC_P  ,DE_UDIA,
     KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                             KC_H  , KC_J  , KC_K  , KC_L  ,DE_ODIA,DE_ADIA,
     KC_LCTL, DE_Y  , KC_X  , KC_C  , KC_V  , KC_B  ,                             KC_N  , KC_M  ,KC_COMM,KC_DOT ,DE_MINS,KC_RSFT,
                     KC_NUBS,DE_ACUT,                                                            DE_PLUS,DE_HASH,
                                      RAISE ,KC_SPC ,                            KC_ENT , LOWER ,
                                     LALT(KC_TAB), RGB   ,               KC_BSPC,TD(TD1),
                                     KC_LALT,LCA(KC_RGHT),               KC_LGUI,KC_RALT
  ),

  [_RGB] = LAYOUT_5x6(
     _______,_______,_______,_______,_______,_______,                            _______,_______,_______,_______,_______,_______,
     _______,_______,_______,_______,RGB_HUI,RGB_TOG,                            _______,_______,_______,_______,_______,_______,
     _______,_______,_______,_______,RGB_SAI,RGB_MOD,                            _______,_______,_______,_______,_______,_______,
     _______,_______,_______,_______,RGB_VAI,MY_RGB_RST,                         _______,_______,_______,_______,_______,_______,
                     _______,_______,                                                            _______,_______,
                                     _______,_______,                            _______,_______,
                                             _______,_______,            _______,_______,
                                             _______,_______,            _______,_______

  ),

  [_LOWER] = LAYOUT_5x6(
     KC_GRV ,KC_EXLM, KC_AT ,KC_HASH,KC_DLR ,KC_PERC,                            KC_CIRC,DE_LCBR,DE_LBRC,DE_RBRC,DE_RCBR,DE_BSLS,
     _______,_______,_______,_______,_______,_______,                            _______,_______, KC_UP ,_______,_______,_______,
     _______,KC_HOME,KC_PGUP,KC_PGDN,KC_END ,_______,                            _______,KC_LEFT,KC_DOWN,KC_RGHT,_______,_______,
     _______,_______,_______,_______,_______,_______,                            _______,_______,_______,_______,_______,_______,
                     _______,_______,                                                            _______,_______,
                                     _______,_______,                            _______,_______,
                                             _______,_______,            _______,_______,
                                             _______,_______,            _______,_______

  ),

  [_RAISE] = LAYOUT_5x6(
     KC_F12 , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 ,                             KC_F6 , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,
     KC_ENT ,_______,_______,_______,_______,DE_LCBR,                            DE_RCBR,KC_NUM ,KC_INS ,_______,KC_PSCR,KC_MUTE,
     _______,KC_LEFT, KC_UP ,KC_DOWN,KC_RGHT,DE_LBRC,                            DE_RBRC,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_VOLU,
     _______,_______,_______,_______,_______,_______,                            _______,_______,_______,_______,_______,KC_VOLD,
                     _______,_______,                                                            KC_EQL ,_______,
                                     _______,_______,                            _______,_______,
                                             _______,_______,            KC_DEL ,_______,
                                             _______,_______,            _______,_______
  ),
};

const uint8_t userEffectModes[] = {
    RGBLIGHT_MODE_STATIC_LIGHT,
    RGBLIGHT_MODE_RAINBOW_SWIRL + 5,
    RGBLIGHT_MODE_SNAKE,
    RGBLIGHT_MODE_TWINKLE + 2,
    RGBLIGHT_MODE_TWINKLE + 4
};
int8_t userEffectModeIndex = 0;
const uint8_t numberOfUserEffectModes = sizeof(userEffectModes) / sizeof(userEffectModes[0]);
uint8_t static_colors[3] = {0, 2, 3};
const uint8_t number_of_static_colors = sizeof(static_colors) / sizeof(static_colors[0]);
bool is_standby_effect_on = true;

uint16_t press_hold_timer;
uint16_t keycode_uml;
bool is_keycode_uml_pending = false;

uint32_t standby_timer;
bool isStandbyActive = false;
uint8_t currentRgbMode;

bool is_left_shift_pressed(void) {
    return (get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT);
}

bool is_right_shift_pressed(void) {
    return (get_mods() & MOD_BIT(KC_RSFT)) == MOD_BIT(KC_RSFT);
}

bool is_shift_pressed(void) {
    return is_left_shift_pressed() || is_right_shift_pressed();
}

void press_hold_key(uint16_t keycode, bool isShiftPressed, uint16_t shiftedKeycode) {
	if (isShiftPressed) {
		bool isLeftShiftPressed  = is_left_shift_pressed();
		bool isRightShiftPressed = is_right_shift_pressed();

		if (isLeftShiftPressed)  unregister_code(KC_LSFT);
		if (isRightShiftPressed) unregister_code(KC_RSFT);

		register_code16(shiftedKeycode);
		unregister_code16(shiftedKeycode);

		if (isLeftShiftPressed)  register_code(KC_LSFT);
		if (isRightShiftPressed) register_code(KC_RSFT);

	} else {
		register_code16(keycode);
		unregister_code16(keycode);
	}
}

// only activate standby for effects with choosen color (ignore on colorful effects)
void set_standby_effect(void) {
	for (uint8_t i = 0; i < number_of_static_colors; i++) {
		if (static_colors[i] == userEffectModeIndex) {
			is_standby_effect_on = true;
			return;
		}
	}
	is_standby_effect_on = false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	standby_timer = timer_read32();
	if (isStandbyActive) {
		// deactivate RGB standby animation, get back to settings before
		isStandbyActive = false;
		rgblight_mode_noeeprom(currentRgbMode);
	}

	switch (keycode) {
		// Cycle through preselected RGB effects
		case RGB_MOD:
			if (rgblight_is_enabled() && record->event.pressed) {
				if (is_shift_pressed()) {
					userEffectModeIndex--;
				} else {
					userEffectModeIndex++;
				}
				if (userEffectModeIndex >= numberOfUserEffectModes) {
					userEffectModeIndex = 0;
				} else if (userEffectModeIndex < 0) {
					userEffectModeIndex = numberOfUserEffectModes - 1;
				}

				rgblight_mode(userEffectModes[userEffectModeIndex]);
			}

            set_standby_effect();

			return false; // Do not let QMK process the keycode further

        // Set RGB effects to default
        case MY_RGB_RST:
            if (record->event.pressed) {
                rgblight_mode(RGBLIGHT_DEFAULT_MODE);
                rgblight_sethsv(RGBLIGHT_DEFAULT_HUE, RGBLIGHT_DEFAULT_SAT, RGBLIGHT_DEFAULT_VAL);
                userEffectModeIndex = 0;
            }
            set_standby_effect();
            return false;

		// Hold ü/ä/ö to get [ or ] or | OR shifted { or } or |
		case DE_UDIA:
		case DE_ADIA:
		case DE_ODIA:
			if (record->event.pressed) {
				// Don't print uml, just remember it. Print it when key is released or another key has been pressed.
				press_hold_timer = timer_read();
				unregister_code16(keycode);
				keycode_uml = keycode;
				is_keycode_uml_pending = true;
			} else if (is_keycode_uml_pending) {
				// if another key, different than uml, has been pressed the uml keycode has already been printed.
				is_keycode_uml_pending = false;
				if (timer_elapsed(press_hold_timer) > 200) {
					unregister_code16(keycode);
					if (keycode == DE_UDIA) {
						press_hold_key(DE_LBRC, is_shift_pressed(), DE_LCBR);
					} else if (keycode == DE_ADIA) {
						press_hold_key(DE_RBRC, is_shift_pressed(), DE_RCBR);
					} else { // keycode == DE_ODIA
						press_hold_key(DE_PIPE, false, 0);
					}
				} else {
					register_code(keycode);
					unregister_code16(keycode);
				}
			}
			return false;

		default:
			// If there is an uml keycode pending print it first and consume it (don't print twice when released).
			if (is_keycode_uml_pending) {
				is_keycode_uml_pending = false;
				register_code(keycode_uml);
				unregister_code16(keycode_uml);
			}
			return true; // Process all other keycodes normally
    }
    return true;
}

void matrix_scan_user(void) {
	// activate RGB standby animation after 5 minutes.
	if (!isStandbyActive && timer_elapsed32(standby_timer) > 300000 && is_standby_effect_on) {
		isStandbyActive = true;
		currentRgbMode = rgblight_get_mode();
		rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
	}
}
