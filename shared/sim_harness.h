#ifndef SIM_HARNESS_H
#define SIM_HARNESS_H

#include "lvgl.h"

/* Display defaults — override per-target via CMake compile definitions. */
#ifndef DISP_HOR_RES
#define DISP_HOR_RES 160
#endif

#ifndef DISP_VER_RES
#define DISP_VER_RES 80
#endif

#ifndef SCALE_FACTOR
#define SCALE_FACTOR 4
#endif

/**
 * Implement this in your app.  Called once after LVGL and the display
 * driver are fully initialised.
 */
extern void app_create_ui(void);

#endif /* SIM_HARNESS_H */
