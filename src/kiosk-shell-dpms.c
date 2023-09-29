#include <libweston/libweston.h>
#include <libweston/zalloc.h>
#include <stdint.h>
#include "kiosk-shell-dpms-server-protocol.h"

struct dpms {
    uint32_t state;
    struct wl_listener idle_listner;
    struct wl_listener wake_listner;
    struct wl_listener destroy_listner;
    struct weston_compositor *compositor;
};

// Global variable
struct dpms *display;

// Forward Declarations
WL_EXPORT int wet_module_init(struct weston_compositor *, int *, char **);
static void weston_compositor_destroy_listener(struct wl_listener *, void *);
static void weston_compositor_idle_listener(struct wl_listener *, void *);
static void weston_compositor_wake_listener(struct wl_listener *, void *);
void set_power_state(struct dpms *, uint32_t);
static void bind_dpms(struct wl_client *, void *, uint32_t, uint32_t);
static void unbind_dpms(struct wl_resource *);
static void set_dpms_mode(struct wl_client *, struct wl_resource *, uint32_t);
void update_display_state();

static const struct kiosk_shell_dpms_manager_interface dpms_implementation = {
    .set_mode = set_dpms_mode
};


WL_EXPORT int
wet_module_init(struct weston_compositor *compositor, int *argc, char **argv) {

    display = zalloc(sizeof *display);

    if (display == NULL) {
        return -1;
    }

    display->compositor = compositor;

    update_display_state();

    if(!weston_compositor_add_destroy_listener_once(compositor, &display->destroy_listner, weston_compositor_destroy_listener)){
        free(display);
        return 0;
    }

    display->wake_listner.notify = weston_compositor_wake_listener;
    wl_signal_add(&compositor->wake_signal, &display->wake_listner);

    display->idle_listner.notify = weston_compositor_idle_listener;
    wl_signal_add(&compositor->idle_signal, &display->idle_listner);

    if(!wl_global_create(compositor->wl_display, &kiosk_shell_dpms_manager_interface, 1, NULL, bind_dpms)) {
        wl_list_remove(&display->destroy_listner.link);
        free(display);
        return -1;
    }

    weston_log("Kiosk DPMS initiated\n");

    return 0;
}

static void weston_compositor_destroy_listener(struct wl_listener *listener, void *data) {
    wl_list_remove(&display->destroy_listner.link);
    wl_list_remove(&display->idle_listner.link);
    wl_list_remove(&display->wake_listner.link);
    free(display);
    #ifdef DEBUG
    weston_log("Kiosk DPMS destroyed\n");
    #endif
}

static void weston_compositor_idle_listener(struct wl_listener *listener, void *data) {
    set_power_state(display, KIOSK_SHELL_DPMS_MANAGER_MODE_OFF);
}

static void weston_compositor_wake_listener(struct wl_listener *listener, void *data) {
    set_power_state(display, KIOSK_SHELL_DPMS_MANAGER_MODE_ON);
}

void set_power_state(struct dpms *display, uint32_t mode) {

    if(display->state == mode) {
        return;
    }

    if(mode) {
        weston_compositor_wake(display->compositor);
        weston_log("Compositor set to active\n");
    } else {
        weston_compositor_sleep(display->compositor);
        weston_log("Compositor set to sleep\n");
    }

    update_display_state();

}

static void bind_dpms(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
    struct wl_resource *resource;
    resource = wl_resource_create(client, &kiosk_shell_dpms_manager_interface, 1, id);
    wl_resource_set_implementation(resource, &dpms_implementation, NULL, unbind_dpms);
    kiosk_shell_dpms_manager_send_mode(resource, display->state);
    weston_log("Kiosk DPMS bound\n");
}

static void unbind_dpms(struct wl_resource *resource) {
    // Nothing to do here
}

static void set_dpms_mode(struct wl_client *client, struct wl_resource *resource, uint32_t mode) {
    set_power_state(display, mode);
    kiosk_shell_dpms_manager_send_mode(resource, display->state);
}

void update_display_state() {
    if(display->compositor->state == 2 || display->compositor->state == 3) {
        display->state = 0;
    } else {
        display->state = 1;
    }
}