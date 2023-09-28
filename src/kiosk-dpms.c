#include <libweston/libweston.h>
#include <libweston/zalloc.h>
#include <stdint.h>

struct dpms {
    int state;
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
void set_power_state(struct dpms *, uint32_t mode);


WL_EXPORT int
wet_module_init(struct weston_compositor *compositor, int *argc, char **argv) {

    display = zalloc(sizeof *display);

    if (display == NULL) {
        return -1;
    }

    display->compositor = compositor;
    display->state = 1;

    if(!weston_compositor_add_destroy_listener_once(compositor, &display->destroy_listner, weston_compositor_destroy_listener)){
        free(display);
        return 0;
    }

    display->wake_listner.notify = weston_compositor_wake_listener;
    wl_signal_add(&compositor->wake_signal, &display->wake_listner);

    display->idle_listner.notify = weston_compositor_idle_listener;
    wl_signal_add(&compositor->idle_signal, &display->idle_listner);

    weston_log("Kiosk DPMS initiated\n");

    return 0;
}

static void weston_compositor_destroy_listener(struct wl_listener *listener, void *data) {
    wl_list_remove(&display->destroy_listner.link);
    wl_list_remove(&display->idle_listner.link);
    wl_list_remove(&display->wake_listner.link);
    display->compositor = NULL;
    free(display);
    #ifdef DEBUG
    weston_log("Kiosk DPMS destroyed\n");
    #endif
}

static void weston_compositor_idle_listener(struct wl_listener *listener, void *data) {
    set_power_state(display, 0);
}

static void weston_compositor_wake_listener(struct wl_listener *listener, void *data) {
    set_power_state(display, 1);
}


void set_power_state(struct dpms *display, uint32_t mode) {
    if(mode) {
        weston_compositor_wake(display->compositor);
        display->state = 1;
        #ifdef DEBUG
        weston_log("Compositor set to active\n");
        #endif
    } else {
        weston_compositor_sleep(display->compositor);
        display->state = 0;
        #ifdef DEBUG
        weston_log("Compositor set to sleep\n");
        #endif
    }
}