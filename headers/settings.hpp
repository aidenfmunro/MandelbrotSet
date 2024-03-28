#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#ifdef GRAPHICS
    #define GRAPHICS_ON(...) \
        __VA_ARGS__
#else
    #define GRAPHICS_ON(...)
#endif

#endif