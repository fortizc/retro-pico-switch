#include <string.h>
#include "pico/stdlib.h"
#include "n64.hpp"
#include "gamecube.hpp"
#include "switch_descriptors.hpp"
#include "tusb.h"

#ifdef CONTROLLER_TYPE
#define controllerType CONTROLLER_TYPE
#else
#define controllerType "N64"
#endif

int main()
{
    stdio_init_all();
    Controller *controller;
    if (strcmp(controllerType, "N64") == 0) {
        controller = new N64Controller(3);
    } else if (strcmp(controllerType, "GC") == 0) {
        controller = new GamecubeController(3);
    }

    SwitchDescriptors switchController;
    controller->init();
    tusb_init();
    while (true) {
        try {
            controller->updateState();
            tud_task();
            if (tud_suspended()) {
                tud_remote_wakeup();
            }
            if (tud_hid_ready()) {
                tud_hid_report(0, controller->getSwitchReport(),
                               sizeof(SwitchReport));
            }
        } catch (int e) {
            tud_task();
            if (tud_suspended()) {
                tud_remote_wakeup();
            }
        }
    }
}
