# chk_win

Simple OpenGL enabled window for C projects.

## Description

Sometimes all you really need is a simple OpenGL window with a couple callbacks.
This library takes care of all the backend stuff, leaving you with a simple interface.

Usage:

```c
    #include <chk_win.h>

    CHK_WIN_CALLBACK(called_every_frame) {
        int* my_data = user_ptr;

        // Increased every frame
        *my_data++;
    }

    int main() {
        // User data
        int useful_data = 42;

        // The window
        chk_win the_window = {0}; // Zero initialize for best results.
        chk_win_config config = chk_win_default_config();

        // The config
        // there are also: on_start, on_close and on_resize.
        config.callbacks.on_frame = called_every_frame;
        config.callbacks.user_ptr = &useful_data;

        // Run!
        if (!chk_win_create(&the_win, config)) { return 1; }
        chk_win_run(&the_win);

        // Closed, perform cleanup!
        chk_win_destroy(&the_win);
        return 0;
    }
```

## Getting Started

### Dependencies

Because we use Glad2 for OpenGL initialization, you need to have Python3 available, and the jinja2 library.
Note: In macOS, jinja2 is not available on brew, so you'll need to run

```
    python3 -m pip install jinja2 --break-system-packages
```

because pip is setup in managed mode.

In linux you might have to install GLfW's dependencies from: [here](https://www.glfw.org/docs/latest/compile.html#compile_deps)

### Building

- As any CMake project.

If using visual studio code, you can install the cmake extension, and press Build in the status bar.
Everything should just work.

## Authors

Jorge Botarro. [@chikinrasshu](https://x.com/chikinrasshu)

## License

This project is licensed under the MIT License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.

- [glfw](https://github.com/glfw/glfw)
- [glad](https://github.com/Dav1dde/glad)
