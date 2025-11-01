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
#define SFT_SPC MT(MOD_LSFT, KC_SPC)
#define OS_BRCS MT(MOD_LGUI, KC_LBRC)
#define LY2_ENT LT(2, KC_ENT)
#define LY3_INT4 LT(3, KC_INT4)

// タップダンスの定義
enum {
    TD_MUHENKAN_HENKAN = 0,
    TD_ALT_TG1,
    TD_LPRN_RPRN,
    TD_LBRC_RBRC,
};

// タップダンス機能の実装
void dance_muhenkan_henkan(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code(KC_INT5);  // 1回タップ: 無変換
    } else if (state->count == 2) {
        tap_code(KC_INT4);  // 2回タップ: 変換
    }
}

// Alt / TG(1) タップダンス（長押しでAlt modifier）
typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
};

uint8_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        return DOUBLE_TAP;
    }
    return 0;
}

static tap alt_tg1_tap_state = {
    .is_press_action = true,
    .state = 0
};

void alt_tg1_finished(tap_dance_state_t *state, void *user_data) {
    alt_tg1_tap_state.state = cur_dance(state);
    switch (alt_tg1_tap_state.state) {
        case SINGLE_TAP:
            tap_code(KC_LALT);
            break;
        case SINGLE_HOLD:
            register_code(KC_LALT);
            break;
        case DOUBLE_TAP:
            layer_invert(1);  // TG(1)
            break;
    }
}

void alt_tg1_reset(tap_dance_state_t *state, void *user_data) {
    switch (alt_tg1_tap_state.state) {
        case SINGLE_HOLD:
            unregister_code(KC_LALT);
            break;
    }
    alt_tg1_tap_state.state = 0;
}

// ( / ) タップダンス
void dance_lprn_rprn(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code16(S(KC_8));  // 1回タップ: (
    } else if (state->count == 2) {
        tap_code16(S(KC_9));  // 2回タップ: )
    }
}

// [ / ] タップダンス
void dance_lbrc_rbrc(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code(JP_LBRC);  // 1回タップ: [
    } else if (state->count == 2) {
        tap_code(JP_RBRC);  // 2回タップ: ]
    }
}

// タップダンスアクション
tap_dance_action_t tap_dance_actions[] = {
    [TD_MUHENKAN_HENKAN] = ACTION_TAP_DANCE_FN(dance_muhenkan_henkan),
    [TD_ALT_TG1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_tg1_finished, alt_tg1_reset),
    [TD_LPRN_RPRN] = ACTION_TAP_DANCE_FN(dance_lprn_rprn),
    [TD_LBRC_RBRC] = ACTION_TAP_DANCE_FN(dance_lbrc_rbrc),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T   ,                                       KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_BSPC  ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G   ,                                       KC_H     , KC_J     , KC_K     , KC_L     , JP_SCLN  , JP_QUOT  ,
    CTL_PRNS , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B   ,                                       KC_N     , KC_M     , KC_COMM  , KC_DOT   , JP_MINS  , KC_SLASH ,
               OS_BRCS  , TD(TD_ALT_TG1) ,     LY2_ENT, KC_BTN1, TD(TD_MUHENKAN_HENKAN),              LY3_INT4    , SFT_SPC , KC_0     , KC_PSCR  , JP_EQL
  ),

  [1] = LAYOUT_universal(
    _______   , KC_Q     , KC_L     , KC_U     , KC_COMM  , KC_DOT    ,                                       KC_F     , KC_W     , KC_R     , KC_Y     , KC_P     , _______  ,
    _______   , KC_E     , KC_I     , KC_A     , KC_O     , JP_MINS   ,                                       KC_K     , KC_T     , KC_N     , KC_S     , KC_H     , _______  ,
    _______   , KC_Z     , KC_X     , KC_C     , KC_V     , JP_SCLN   ,                                       KC_G     , KC_D     , KC_M     , KC_J     , KC_B     , _______ ,
               _______   , _______ ,                   _______, _______, _______,              _______  , _______      , _______  , _______  , _______
  ),

  [2] = LAYOUT_universal(
    KC_F1    ,  KC_F2   , KC_F3    , KC_F4   , KC_F5    , KC_F6    ,                                         S(KC_1)  , S(KC_3)  , S(KC_4)  , S(KC_5)  , S(KC_6)  , KC_BSPC  ,
    KC_F7    ,  KC_F8   , KC_F9    , KC_F10  , KC_F11   , KC_F12   ,                                         KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , JP_AT    , JP_CIRC  , 
    SSNP_VRT ,  SSNP_HOR, KC_LEFT  , KC_UP   , KC_DOWN  , KC_RGHT  ,                                         TD(TD_LPRN_RPRN), TD(TD_LBRC_RBRC), JP_ASTR  , JP_PIPE  , JP_TILD  ,  JP_YEN ,
                  SSNP_FRE , _______ , _______  ,         _______  , _______  ,                 _______,  _______, _______       , _______  , JP_GRV
  ),

  [3] = LAYOUT_universal(
    _______  , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                         _______  , _______  , _______  , _______  , _______  , KC_BSPC  ,
    _______  , KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,                                         RGUI(KC_LEFT), RGUI(KC_DOWN)  , RGUI(KC_UP)  , RGUI(KC_RIGHT)  , _______    , KC_DEL  ,
    _______  , C(KC_Z)  , C(KC_X)  , C(KC_C)  , C(KC_V)  , C(KC_A)  ,                                         A(KC_LEFT)  , A(KC_RIGHT)  , _______   , _______  , KC_PGDN  , KC_PGUP  ,
                  RCS(KC_C)  , RCS(KC_V)  , _______  ,         KC_BTN2  , _______  ,                   _______   , _______  , _______       , _______  , JP_CAPS
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
    case OS_BRCS:
      if (record->tap.count && record->event.pressed) {
        uint8_t mods = get_mods();
        tap_code(JP_LBRC);
        tap_code(JP_RBRC);
        unregister_mods(MOD_MASK_SHIFT);
        tap_code(KC_LEFT);
        set_mods(mods);
        return false;
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
