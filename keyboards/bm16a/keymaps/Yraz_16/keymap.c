
#include QMK_KEYBOARD_H

typedef struct {
    bool is_press_action;
    int state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    TRIPLE_TAP,
    TRIPLE_HOLD
};

enum layers {
  _BASE = 0,
  _FN1,
  _FN2,
};

enum {
    TD_ESC_CAPS,
    TD_COL_PIPE
};

int cur_dance(qk_tap_dance_state_t *state);
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);

int cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }

    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    } else return 8;
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
    .is_press_action = true,
    .state = 0
};

void x_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: register_code16(KC_SCOLON); break;
        case SINGLE_HOLD: register_code16(KC_COLON); break;
        case DOUBLE_TAP: register_code16(KC_PIPE); break;
        case DOUBLE_HOLD: register_code16(KC_NO); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_SCOLON); register_code16(KC_SCOLON);
    }
}

void x_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case SINGLE_TAP: unregister_code16(KC_SCOLON); break;
        case SINGLE_HOLD: unregister_code16(KC_COLON); break;
        case DOUBLE_TAP: unregister_code16(KC_PIPE); break;
        case DOUBLE_HOLD: unregister_code16(KC_NO);
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_SCOLON);
    }
    xtap_state.state = 0;
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
    [TD_COL_PIPE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset)

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
            KC_NO, KC_NO, KC_TRNS, TD(TD_COL_PIPE), \
            KC_BTN1, KC_NO, KC_NO, TD(TD_ESC_CAPS) \
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
