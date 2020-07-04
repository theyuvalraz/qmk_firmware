
#include QMK_KEYBOARD_H

enum layers {
  _BASE = 0,
  _FN1,
  _FN2,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QMKBEST = SAFE_RANGE,
  QMKURL
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

        [_BASE] = LAYOUT_ortho_4x4(
            KC_GESC, KC_GRV, KC_BSLS, KC_BSPC, \
            KC_DEL, KC_LPRN, KC_RPRN, LT(_FN1, KC_APP), \
            KC_LSFT, KC_UP, TG(_FN2), KC_TAB, \
            KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT \
            ),

        [_FN1] = LAYOUT_ortho_4x4(
            KC_NO, KC_NO, RGB_MOD, RGB_TOG, \
            KC_NO, KC_LCBR, KC_RCBR, KC_NO, \
            KC_NO, KC_BRIU, KC_TRNS, KC_MUTE, \
            BL_DEC, KC_BRID, BL_INC, KC_NO \
            ),

        [_FN2] = LAYOUT_ortho_4x4(
            KC_WH_D, KC_MS_U, KC_WH_U, KC_BTN3, \
            KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN2, \
            KC_NO, KC_NO, KC_TRNS, KC_NO, \
            KC_BTN1, KC_NO, KC_NO, KC_NO \
            )
};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _BASE:
        rgblight_setrgb (0x00,  0x00, 0xFF);
        break;
    case _FN1:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case _FN2:
        rgblight_setrgb (0x00,  0xFF, 0x00);
        break;
    default: //  for any other layers, or the default layer
        rgblight_setrgb (0x00,  0xFF, 0xFF);
        break;
    }
  return state;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QMKBEST:
      if (record->event.pressed) {
        // when keycode QMKBEST is pressed
        SEND_STRING("QMK is the best thing ever!");
      } else {
        // when keycode QMKBEST is released
      }
      break;
    case QMKURL:
      if (record->event.pressed) {
        // when keycode QMKURL is pressed
        SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
      } else {
        // when keycode QMKURL is released
      }
      break;
  }
  return true;
}
