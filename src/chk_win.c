#include <chk_win.h>
#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <assert.h>
#include <stdio.h>

void chk_win_cb_on_error(int32_t code, const char* msg);
void chk_win_cb_on_close(GLFWwindow* impl);
void chk_win_cb_on_frame(GLFWwindow* impl);
void chk_win_cb_on_move(GLFWwindow* impl, int32_t x, int32_t y);
void chk_win_cb_on_size(GLFWwindow* impl, int32_t w, int32_t h);
void chk_win_cb_on_fb_size(GLFWwindow* impl, int32_t w, int32_t h);
void chk_win_cb_on_dpi(GLFWwindow* impl, float x, float y);
void chk_win_cb_on_minimize(GLFWwindow* impl, int32_t v);
void chk_win_cb_on_maximize(GLFWwindow* impl, int32_t v);
void chk_win_cb_on_focus(GLFWwindow* impl, int32_t v);
void chk_win_cb_on_active(GLFWwindow* impl, int32_t v);
void chk_win_cb_on_mouse_pos(GLFWwindow* impl, double x, double y);

static int32_t g_win_count = 0;

chk_win_config chk_win_default_config() {
    chk_win_config config = {0};

    config.w       = 1280;
    config.h       = 720;
    config.caption = "chk_win";

    return config;
}

bool chk_win_create(chk_win* win, chk_win_config config) {
    // Check that the window exists and the config is reasonable
    assert(win != NULL);
    assert(config.caption != NULL);
    assert(config.w > 0);
    assert(config.h > 0);

    // Initialize the backend
    if (!g_win_count) {
        glfwSetErrorCallback(chk_win_cb_on_error);
        if (!glfwInit()) {
            printf("Failed to initialize GLFW!\n");
            return false;
        }
    }

    // Destroy the current window if re-creating an existing one.
    if (win->_impl) {
        if (!chk_win_destroy(win)) {
            printf("Failed to destroy the existing window!\n");
            return false;
        }
    }
    g_win_count++;

    // Begin from square one.
    *win           = (chk_win){0};
    win->callbacks = config.callbacks;

    // Create the implementation
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    win->_impl =
        glfwCreateWindow(config.w, config.h, config.caption, NULL, NULL);
    if (!win->_impl) {
        printf("Failed to create the window!\n");
        return false;
    }

    // Initialize OpenGL
    glfwMakeContextCurrent(win->_impl);
    if (!gladLoadGL(glfwGetProcAddress)) { return false; }

    // Setup callbacks
    glfwSetWindowUserPointer(win->_impl, win);
    glfwSetWindowCloseCallback(win->_impl, chk_win_cb_on_close);
    glfwSetWindowRefreshCallback(win->_impl, chk_win_cb_on_frame);
    glfwSetWindowPosCallback(win->_impl, chk_win_cb_on_move);
    glfwSetWindowSizeCallback(win->_impl, chk_win_cb_on_size);
    glfwSetFramebufferSizeCallback(win->_impl, chk_win_cb_on_fb_size);
    glfwSetWindowContentScaleCallback(win->_impl, chk_win_cb_on_dpi);
    glfwSetWindowIconifyCallback(win->_impl, chk_win_cb_on_minimize);
    glfwSetWindowMaximizeCallback(win->_impl, chk_win_cb_on_maximize);
    glfwSetWindowFocusCallback(win->_impl, chk_win_cb_on_focus);
    glfwSetCursorEnterCallback(win->_impl, chk_win_cb_on_active);
    glfwSetCursorPosCallback(win->_impl, chk_win_cb_on_mouse_pos);

    // Get initial values
    glfwGetWindowPos(win->_impl, &win->x, &win->y);
    glfwGetWindowSize(win->_impl, &win->w, &win->h);
    glfwGetFramebufferSize(win->_impl, &win->fb_w, &win->fb_h);

    float tmp_af, tmp_bf;
    glfwGetWindowContentScale(win->_impl, &tmp_af, &tmp_bf);
    glfwGetCursorPos(win->_impl, &win->cur_x, &win->cur_y);

    win->dpi_x = tmp_af, win->dpi_y = tmp_bf;

    win->ct = win->lt = glfwGetTime();
    win->dt           = 0.0;

    // Done.
    glfwShowWindow(win->_impl);
    win->flags.is_running = true;
    return true;
}

bool chk_win_destroy(chk_win* win) {
    assert(win != NULL);
    if (win->_impl != NULL) {
        glfwDestroyWindow(win->_impl);
        win->_impl = NULL;
    }
    g_win_count--;
    if (!g_win_count) { glfwTerminate(); }
    return true;
}

bool chk_win_run(chk_win* win) {
    assert(win != NULL);

    if (win->callbacks.on_start) {
        win->callbacks.on_start(win, win->callbacks.user_ptr);
    }

    bool result = false;
    while (win->flags.is_running) { result = chk_win_step(win, true); }

    if (win->callbacks.on_close) {
        win->callbacks.on_close(win, win->callbacks.user_ptr);
    }

    return result;
}

bool chk_win_step(chk_win* win, bool process_events) {
    assert(win != NULL);

    win->lt = win->ct;
    win->ct = glfwGetTime();
    win->dt = win->ct - win->lt;

    if (win->callbacks.on_frame) {
        win->callbacks.on_frame(win, win->callbacks.user_ptr);
    }

    glfwSwapBuffers(win->_impl);

    if (process_events) { glfwPollEvents(); }
    win->state = (chk_win_state){0};
    return true;
}

/*
    Implementation of impl callbacks
*/

#define CHK_GET_WIN                                                            \
    chk_win* win = glfwGetWindowUserPointer(impl);                             \
    do {                                                                       \
        if (!win) { return; }                                                  \
    } while (0)

void chk_win_cb_on_error(int32_t code, const char* msg) {
    printf("GLFW error(%d): %s\n", code, msg);
}

void chk_win_cb_on_close(GLFWwindow* impl) {
    CHK_GET_WIN;
    win->flags.is_running = false;
}

void chk_win_cb_on_frame(GLFWwindow* impl) {
    CHK_GET_WIN;
    chk_win_step(win, false);
}

void chk_win_cb_on_move(GLFWwindow* impl, int32_t x, int32_t y) {
    CHK_GET_WIN;
    win->x = x, win->y = y;
    win->state.pos_changed = true;
}

void chk_win_cb_on_size(GLFWwindow* impl, int32_t w, int32_t h) {
    CHK_GET_WIN;
    win->w = w, win->h = h;
    win->state.size_changed = true;
}

void chk_win_cb_on_fb_size(GLFWwindow* impl, int32_t w, int32_t h) {
    CHK_GET_WIN;
    win->fb_w = w, win->fb_h = h;
    win->state.fb_changed = true;
}

void chk_win_cb_on_dpi(GLFWwindow* impl, float x, float y) {
    CHK_GET_WIN;
    win->dpi_x = (double)x, win->dpi_y = (double)y;
    win->state.dpi_changed = true;
}

void chk_win_cb_on_minimize(GLFWwindow* impl, int32_t v) {
    CHK_GET_WIN;
    win->flags.is_minimized      = (bool)v;
    win->state.minimized_changed = true;
}

void chk_win_cb_on_maximize(GLFWwindow* impl, int32_t v) {
    CHK_GET_WIN;
    win->flags.is_maximized      = (bool)v;
    win->state.maximized_changed = true;
}

void chk_win_cb_on_focus(GLFWwindow* impl, int32_t v) {
    CHK_GET_WIN;
    win->flags.is_focused    = (bool)v;
    win->state.focus_changed = true;
}

void chk_win_cb_on_active(GLFWwindow* impl, int32_t v) {
    CHK_GET_WIN;
    win->flags.is_active      = (bool)v;
    win->state.active_changed = true;
}

void chk_win_cb_on_mouse_pos(GLFWwindow* impl, double x, double y) {
    CHK_GET_WIN;
    win->cur_x = x, win->cur_y = y;
    win->state.cursor_pos_changed = true;
}

#undef CHK_GET_WIN