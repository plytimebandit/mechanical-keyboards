/*
Copyright 2023 plytimebandit <email@.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once


#define USE_SERIAL

// https://beta.docs.qmk.fm/using-qmk/hardware-features/feature_split_keyboard
#define MASTER_LEFT
// #define MASTER_RIGHT
//#define EE_HANDS
// Rows are doubled-up

#define TAPPING_TERM 200

// RGB Lighting
// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_rgblight.md
// https://www.diykeyboards.com/projects/add-underglow-to-qmk-boards
#ifdef RGB_DI_PIN
#undef RGB_DI_PIN
#endif
#define RGB_DI_PIN D3
#ifdef RGBLED_NUM
#undef RGBLED_NUM
#endif
#define RGBLED_NUM 30
#define RGBLED_SPLIT { 15, 15 }
#define RGBLIGHT_SPLIT
#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT
#define RGBLIGHT_DEFAULT_HUE 52
#define RGBLIGHT_DEFAULT_SAT 255
#define RGBLIGHT_DEFAULT_VAL 200

// Activate just a few animations
// #define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_TWINKLE
// tuning for RGB effects
#define RGBLIGHT_EFFECT_BREATHE_MAX 150
