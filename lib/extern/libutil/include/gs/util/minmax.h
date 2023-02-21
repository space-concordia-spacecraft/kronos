#ifndef GS_UTIL_MINMAX_H
#define GS_UTIL_MINMAX_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Min/max utilities.
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
   Return minimum value.
   @param[in] x value
   @param[in] y value
   @return the lowest value of the input parameters.
*/
#define gs_min(x,y) ({                          \
            __typeof__ (x) _x = (x);            \
            __typeof__ (y) _y = (y);            \
            _x < _y ? _x : _y; })

/**
   Return maximum value.
   @param[in] x value
   @param[in] y value
   @return the maximum value of the input parameters.
*/
#define gs_max(x,y) ({                          \
            __typeof__ (x) _x = (x);            \
            __typeof__ (y) _y = (y);            \
            _x > _y ? _x : _y; })

/**
   Return minimum value.
   @param[in] x value
   @param[in] y value
   @param[in] z value
   @return the lowest value of the input parameters.
*/
#define gs_min3(x,y,z) ({                                \
            __typeof__ (x) _h = gs_min((x),(y));         \
            gs_min(_h, (z)); })

/**
   Return maximum value.
   @param[in] x value
   @param[in] y value
   @param[in] z value
   @return the maximum value of the input parameters.
*/
#define gs_max3(x,y,z) ({                                \
            __typeof__ (x) _h = gs_max((x),(y));         \
            gs_max(_h, (z)); })

/**
   Clamp value within min/max.
   @param[in] x value
   @param[in] _max max value
   @param[in] _min min value
   @return value between min and max.
*/
#define gs_clamp(x, _min, _max) ({                      \
            __typeof__ (x) _h = gs_max((x),(_min));     \
            gs_min(_h,(_max)); })

#ifdef __cplusplus
}
#endif
#endif
