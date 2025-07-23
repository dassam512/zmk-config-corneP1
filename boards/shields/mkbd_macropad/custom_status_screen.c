#include <zephyr/kernel.h>
#include <lvgl.h>
#include <zmk/battery.h>
#include <zmk/usb.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/battery_state_changed.h>

static lv_obj_t *main_label;
static lv_timer_t *fade_timer;

struct status_state {
    uint8_t battery_level;
    bool usb_connected;
};

static struct status_state get_status_state(const zmk_event_t *eh) {
    return (struct status_state){
        .battery_level = zmk_battery_state_of_charge(),
        .usb_connected = zmk_usb_is_powered(),
    };
}

static void update_status_display(struct status_state state) {
    if (main_label == NULL) return;
    
    char status_text[32];
    if (state.usb_connected) {
        snprintf(status_text, sizeof(status_text), "Charging...");
    } else {
        snprintf(status_text, sizeof(status_text), "Battery: %d%%", state.battery_level);
    }
    
    lv_label_set_text(main_label, status_text);
}

static void fade_to_battery_status(lv_timer_t *timer) {
    struct status_state state = get_status_state(NULL);
    update_status_display(state);
    lv_timer_del(fade_timer);
    fade_timer = NULL;
}

ZMK_DISPLAY_WIDGET_LISTENER(status_screen, struct status_state, 
                            update_status_display, get_status_state)

ZMK_SUBSCRIPTION(status_screen, zmk_usb_conn_state_changed);
ZMK_SUBSCRIPTION(status_screen, zmk_battery_state_changed);

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    main_label = lv_label_create(screen);
    lv_label_set_text(main_label, "Hello P1!");
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);
    
    status_screen_init();
    
    fade_timer = lv_timer_create(fade_to_battery_status, 5000, NULL);
    lv_timer_set_repeat_count(fade_timer, 1);
    
    return screen;
}