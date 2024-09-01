#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct chk_win;
#define CHK_WIN_CALLBACK(Name) void Name(struct chk_win* win, void* user_ptr)
typedef CHK_WIN_CALLBACK(chk_win_callback);

typedef struct chk_win_flags {
    uint32_t is_running    : 1;
    uint32_t is_fullscreen : 1;
    uint32_t is_minimized  : 1;
    uint32_t is_maximized  : 1;
    uint32_t is_focused    : 1;
    uint32_t is_active     : 1;
} chk_win_flags;

typedef struct chk_win_state {
    uint32_t pos_changed        : 1;
    uint32_t size_changed       : 1;
    uint32_t fb_changed         : 1;
    uint32_t dpi_changed        : 1;
    uint32_t cursor_pos_changed : 1;

    uint32_t fullscreen_changed : 1;
    uint32_t minimized_changed  : 1;
    uint32_t maximized_changed  : 1;
    uint32_t focus_changed      : 1;
    uint32_t active_changed     : 1;
} chk_win_state;

typedef struct chk_win_callbacks {
    void*             user_ptr;
    chk_win_callback* on_start;
    chk_win_callback* on_close;
    chk_win_callback* on_frame;
    chk_win_callback* on_resize;
} chk_win_callbacks;

typedef struct chk_win {
    chk_win_flags     flags;
    chk_win_state     state;
    chk_win_callbacks callbacks;

    int32_t x, y;
    int32_t w, h;
    int32_t fb_w, fb_h;
    double  dpi_x, dpi_y;
    double  cur_x, cur_y;
    double  dt, ct, lt;

    void* _impl;
} chk_win;

typedef struct chk_win_config {
    int32_t     x, y;
    int32_t     w, h;
    const char* caption;

    chk_win_callbacks callbacks;
} chk_win_config;

chk_win_config chk_win_default_config();

bool chk_win_create(chk_win* win, chk_win_config config);
bool chk_win_destroy(chk_win* win);
bool chk_win_run(chk_win* win);
bool chk_win_step(chk_win* win, bool process_events);
