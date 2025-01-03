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

#define CTL_PRNS MT(MOD_LCTL, KC_8)
#define SFT_GRV MT(MOD_LSFT, KC_GRV)
#define ALT_BRCS MT(MOD_LALT, KC_LBRC)
#define LY2_SPC LT(2, KC_SPACE)
#define LY3_ENT LT(3, KC_ENT)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_L     , KC_U     , KC_COMM  , KC_DOT    ,                                       KC_F     , KC_W     , KC_R     , KC_Y     , KC_P     , JP_EQL   ,
    KC_TAB   , KC_E     , KC_I     , KC_A     , KC_O     , JP_MINS   ,                                       KC_K     , KC_T     , KC_N     , KC_S     , KC_H     , JP_QUOT  ,
    CTL_PRNS , KC_Z     , KC_X     , KC_C     , KC_V     , JP_SCLN   ,                                       KC_G     , KC_D     , KC_M     , KC_J     , KC_B     , KC_SLASH ,
               KC_LGUI  , ALT_BRCS,                   LY2_SPC , KC_BTN1, SFT_GRV,               KC_BSPC  , LY3_ENT    , KC_0     , KC_PSCR  , TG(1)
  ),

  [1] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T   ,                                       KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , JP_EQL   ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G   ,                                       KC_H     , KC_J     , KC_K     , KC_L     , JP_SCLN  , JP_QUOT  ,
    CTL_PRNS , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B   ,                                       KC_N     , KC_M     , KC_COMM  , KC_DOT   , JP_MINS  , KC_SLASH ,
               KC_LGUI  , ALT_BRCS,                   LY2_SPC , KC_BTN1, SFT_GRV,                 KC_BSPC  , LY3_ENT    , KC_0     , KC_PSCR  , TG(1)
  ),

  [2] = LAYOUT_universal(
    KC_F1    ,  KC_F2   , KC_F3    , KC_F4   , KC_F5    , KC_F6    ,                                         C(KC_Y)  , _______  , RCS(KC_C), RCS(KC_V), RGUI(KC_LEFT),RGUI(KC_RGHT),
    KC_F7    ,  KC_F8   , KC_F9    , KC_F10  , KC_F11   , KC_F12   ,                                         KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , C(KC_S)  , C(KC_A)  , 
    _______  ,  _______ , KC_LEFT  , KC_UP   , KC_DOWN  , KC_RGHT  ,                                         C(KC_Z)  , C(KC_X)  , C(KC_C)  , C(KC_V)  , KC_PGUP  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                 A(KC_LEFT),  A(KC_RGHT)  , _______       , _______  , KC_PGDN
  ),

  [3] = LAYOUT_universal(
    _______  ,KC_1      , KC_2     , KC_3    , KC_4     , KC_5     ,                                         S(KC_1)  , S(KC_3)  , S(KC_4)  , S(KC_5)  , S(KC_6)  , S(KC_8)  ,
    _______  ,KC_6      , KC_7     , KC_8    , KC_9     , KC_0     ,                                         JP_AT    , JP_CIRC  , JP_TILD  , JP_LBRC  , JP_RBRC  , S(KC_9)  ,
    _______  ,S(KC_MINS), _______  , _______ , _______  , _______  ,                                         JP_CAPS  , JP_ASTR  , JP_YEN   , JP_PIPE  , JP_GRV   , KC_DEL   ,
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   _______   , _______  , _______       , _______  , _______
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
        uint8_t mods = get_mods();
        if (mods & MOD_MASK_SHIFT) {
          unregister_mods(MOD_MASK_SHIFT);
          tap_code(JP_COLN);
          set_mods(mods);
          return false;
        }
      }
    case CTL_PRNS:
      if (record->tap.count && record->event.pressed) {
        register_mods(MOD_LSFT);
        tap_code(KC_8);
        tap_code(KC_9);
        unregister_mods(MOD_LSFT);
        tap_code(KC_LEFT);
        return false;
      }
      return true;
    case ALT_BRCS:
      if (record->tap.count && record->event.pressed) {
        tap_code(JP_LBRC);
        tap_code(JP_RBRC);
        tap_code(KC_LEFT);
        return false;
      }
    case KC_BTN1:
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          tap_code(KC_BTN2);
          return false;
        }
      }
    case TG(1):
      if (record->event.pressed) {
        if (IS_LAYER_ON(0)) {
          tap_code(KC_INT5); //無変換
        } else {
          tap_code(KC_INT4); //変換
        }
      }
  }
  return true;
}

// clang-format on
layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 1
    keyball_set_scroll_mode(get_highest_layer(state) == 2);
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
