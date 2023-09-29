#include <stdint.h>
#include <stdio.h>
#include <wayland-client.h>
#include "kiosk-shell-dpms-client-protocol.h"
#include <string.h>

struct kiosk_shell_dpms_manager *dpms_manager = NULL;

static void handle_change(void *data, struct kiosk_shell_dpms_manager *kiosk_shell_dpms_manager, uint32_t mode) {
    printf("Current mode: %d\n", mode);
}

static const struct kiosk_shell_dpms_manager_listener event_listener = {
    handle_change
};

static void registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    if (strcmp(interface, kiosk_shell_dpms_manager_interface.name) == 0) {
        dpms_manager = wl_registry_bind(registry, name, &kiosk_shell_dpms_manager_interface, 1);
        kiosk_shell_dpms_manager_add_listener(dpms_manager, &event_listener, NULL);
    }
}

static void registry_handle_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    // This space deliberately left blank
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = registry_handle_global_remove,
};

int main() {

    struct wl_display *display = wl_display_connect(NULL);

    if(!display) {
        printf("Could not connect to display\n");
        return -1;
    }


    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);

    if (!dpms_manager) {
        printf("Could not initialize DPMS\n");
        return -1;
    }

    kiosk_shell_dpms_manager_set_mode(dpms_manager, 1);
    wl_display_roundtrip(display);
    kiosk_shell_dpms_manager_destroy(dpms_manager);

    wl_display_disconnect(display);
    return 0;
}