/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

#include "quantum.h"

enum my_keycodes {
  MY_BACK = SAFE_RANGE,
  MY_FORWARD,
  MY_COPY,
  MY_PASTE,
  MY_CUT
};

enum {
  TD_BRACKETS,
  TD_PARENTHESES,
};

tap_dance_action_t tap_dance_actions[] = {
  [TD_BRACKETS] = ACTION_TAP_DANCE_DOUTBLE(JP_LBRC, JP_RBRC),
  [TD_PARENTHESES] = ACTION_TAP_DANCE_DOUTBLE(JP_LPRN, JP_RPRN),
};

#define CTL_LPRN MT(MOD_LCTL, TD_PARENTHESES)
#define SFT_INT5 MT(MOD_LSFT, KC_INT5)
#define ALT_RIGHT MT(MOD_LALT, TD_BRACKETS)
#define LY3_INT4 LT(3, KC_INT4)
#define LY1_SPC LT(1, KC_SPACE)
#define LY2_ENT LT(2, KC_ENT)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_L     , KC_U     , KC_COMM  , KC_DOT    ,                                       KC_F     , KC_W     , KC_R     , KC_Y     , KC_P     , JP_EQL   ,
    KC_TAB   , KC_E     , KC_I     , KC_A     , KC_O     , JP_MINS   ,                                       KC_K     , KC_T     , KC_N     , KC_S     , KC_H     , JP_QUOT  ,
    CTL_LPRN , KC_Z     , KC_X     , KC_C     , KC_V     , LY1_SPC   ,                                       KC_G     , KC_D     , KC_M     , KC_J     , KC_B     , KC_SLASH ,
               KC_LGUI  , ALT_RIGHT,                   SFT_INT5 , KC_BTN1, LY3_INT4,            KC_BSPC  , LY2_ENT    , KC_0     , KC_PSCR  , JP_SCLN
  ),

  [1] = LAYOUT_universal(
    SSNP_FRE ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   KC_DEL   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    _______  , _______   , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______ , _______ , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______ ,                                        _______  , MY_BACK  , MY_FORWARD  , _______ , _______  , _______  ,
    _______ , _______  , _______  , _______  , _______  , _______ ,                                        MY_CUT  ,  MY_COPY  , MY_PASTE  , _______  , _______  , _______ ,
                  _______  , _______  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case JP_MINS:
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          tap_code16(JP_UNDS);
          return false;
        }
      }
    case JP_EQL:
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          tap_code16(JP_PLUS);
          return false;
        }
      }
      return true;
    case JP_QUOT:
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          tap_code16(JP_DQUO);
          return false;
        }
      }
      return true;
    case JP_SCLN:
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          unregister_mods(MOD_MASK_SHIFT);
          tap_code(JP_COLN);
          register_mods(MOD_MASK_SHIFT);
          return false;
        }
      }
    case MY_BACK:
      if (record->event.pressed) {
        tap_code16(A(KC_LEFT));
      }
      return false;
    case MY_FORWARD:
      if (record->event.pressed) {
        tap_code16(A(KC_RGHT));
      }
      return false;
    case MY_COPY:
      if (record->event.pressed) {
        tap_code16(C(KC_C));
      }
      return false;
    case MY_CUT:
      if (record->event.pressed) {
        tap_code16(C(KC_X));
      }
      return false;
    case MY_PASTE:
      if (record->event.pressed) {
        tap_code16(C(KC_V));
      }
      return false;
    case KC_BTN1:
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          tap_code(KC_BTN2);
          return false;
        }
      }
  }
  return true;
}

// clang-format on
layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
