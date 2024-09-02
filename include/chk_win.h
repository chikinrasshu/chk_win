#pragma once

#include <chk_common.h>

typedef struct chk_win chk_win;
#define CHK_WIN_CALLBACK(Name) void Name(chk_win* win, void* user_ptr)
typedef CHK_WIN_CALLBACK(chk_win_callback);

typedef struct chk_win_flags {
    u32 is_running    : 1;
    u32 is_fullscreen : 1;
    u32 is_minimized  : 1;
    u32 is_maximized  : 1;
    u32 is_focused    : 1;
    u32 is_active     : 1;
} chk_win_flags;

typedef struct chk_win_state {
    u32 pos_changed        : 1;
    u32 size_changed       : 1;
    u32 fb_changed         : 1;
    u32 dpi_changed        : 1;
    u32 cursor_pos_changed : 1;

    u32 fullscreen_changed : 1;
    u32 minimized_changed  : 1;
    u32 maximized_changed  : 1;
    u32 focus_changed      : 1;
    u32 active_changed     : 1;
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

    s32 x, y;
    s32 w, h;
    s32 fb_w, fb_h;
    f64 dpi_x, dpi_y;
    f64 cur_x, cur_y;
    f64 dt, ct, lt;

    void* _impl;
} chk_win;

typedef struct chk_win_config {
    s32         x, y;
    s32         w, h;
    const char* caption;

    chk_win_callbacks callbacks;
} chk_win_config;

chk_win_config chk_win_default_config();

bool chk_win_create(chk_win* win, chk_win_config config);
bool chk_win_destroy(chk_win* win);
bool chk_win_run(chk_win* win);
bool chk_win_step(chk_win* win, bool process_events);
