/*
Switch handling functions: short and long press detection
(c) 2012-2013 jenkie and Thomas Jarosch / pedelecforum.de

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "Display/defines.h"
#include "config.h"
#include "switches.h"
#include "display.h"
#include "display_backlight.h"
#include "menu.h"
#include "globals.h"

#if (DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
#include "Display/DisplayController.h"
#endif

int switch_analog_values[_SWITCHES_COUNT] = {1010, 909, 995, 967, 1002, 0000};
int switches_range = 5;


struct switch_state
{
    unsigned long first_press_time;    // time when switch was pressed down (to decide if long or short press)
    bool previous_state;               // state of the switch in the previous loop run
    bool action_enabled;               // disable switch if long-press action has been done until switch is released
    bool trigger_every_time;           // should the action be triggered every loop run during long press (f.e. fake starting aid)
    sw_action action_short_press;      // action to execute on short press
    sw_action action_long_press;       // action to execute on long press
} switch_states[_SWITCHES_COUNT];

// Generic switch handler. Returns a 'switch_result'
enum button_state { BUTTON_ON=0, BUTTON_OFF=1 };

// Forward declarations
static enum switch_result _read_switch(switch_state *state, boolean switch_current);
static void _handle_menu_switch(const enum switch_name sw, const enum switch_result res);
static void action_set_profile(const boolean new_profile);


switch_name get_switch(int value){
    switch_name result = _SWITCHES_COUNT;
    for (int i=0; i<_SWITCHES_COUNT; ++i) {
        if (value >= switch_analog_values[i] - switches_range && value <= switch_analog_values[i] + switches_range)
            return static_cast<switch_name>(i);
    }
    return result;
}

void init_switches()
{
    memset(&switch_states, 0, sizeof(struct switch_state) * _SWITCHES_COUNT);

    // Store switch actions in the right place
    switch_states[SWITCH_THROTTLE].action_short_press = SW_THROTTLE_SHORT_PRESS;
    switch_states[SWITCH_THROTTLE].action_long_press = SW_THROTTLE_LONG_PRESS;

    switch_states[SWITCH_UP].action_short_press = SW_UP_SHORT_PRESS;
    switch_states[SWITCH_UP].action_long_press = SW_UP_LONG_PRESS;

    switch_states[SWITCH_DOWN].action_short_press = SW_DOWN_SHORT_PRESS;
    switch_states[SWITCH_DOWN].action_long_press = SW_DOWN_LONG_PRESS;

    switch_states[SWITCH_LEFT].action_short_press = SW_LEFT_SHORT_PRESS;
    switch_states[SWITCH_LEFT].action_long_press = SW_LEFT_LONG_PRESS;

    switch_states[SWITCH_RIGHT].action_short_press = SW_RIGHT_SHORT_PRESS;
    switch_states[SWITCH_RIGHT].action_long_press = SW_RIGHT_LONG_PRESS;

    switch_states[SWITCH_CENTER].action_short_press = SW_CENTER_SHORT_PRESS;
    switch_states[SWITCH_CENTER].action_long_press = SW_CENTER_LONG_PRESS;


#ifdef SUPPORT_SWITCH_ON_POTI_PIN
    switch_states[SWITCH_POTI].action_short_press = SW_POTI_SHORT_PRESS;
    switch_states[SWITCH_POTI].action_long_press = SW_POTI_LONG_PRESS;
#endif

    // Init actions that trigger immediately on long press
    for(byte i = 0; i < _SWITCHES_COUNT; ++i)
    {
        switch(switch_states[i].action_long_press)
        {
            case ACTION_FIXED_THROTTLE_VALUE:
                switch_states[i].trigger_every_time = true;
                break;
            default:
                break;
        }
    }
}

//
// Switch actions start here.
// Those can be executed when a button is pressed (short or long)
//
void action_set_soft_poti(int new_throttle_stat)
{
#if defined(SUPPORT_SOFT_POTI) || \
    defined(SUPPORT_THROTTLE_AUTO_CRUISE) || \
    defined(SUPPORT_POTI_SWITCHES) || \
    defined(SUPPORT_FIRST_AID_MENU)
    int power_poti;
    byte i=0;
    char buffer[12]="Poti       ";

    // Set soft poti if throttle value changed
    if (poti_stat != new_throttle_stat)
    {
#ifdef SUPPORT_DISPLAY_BACKLIGHT
        enable_custom_backlight(5000);  //switch backlight on for five seconds
#endif
        poti_stat = new_throttle_stat;
        if (poti_stat == 0)
            display_show_important_info(FROM_FLASH(msg_tempomat_reset), 0);
        else

        {
#if CONTROL_MODE == CONTROL_MODE_TORQUE                      //human power control mode
#ifdef SUPPORT_XCELL_RT
            buffer[9]='%';
            power_poti = poti_stat/1023.0* curr_power_poti_max; //power_poti_max is in this control mode interpreted as percentage. Example: power_poti_max=200 means; motor power = 200% of human power
#endif
#endif

#if CONTROL_MODE == CONTROL_MODE_NORMAL                      //constant power mode
            buffer[9]='W';
            power_poti = poti_stat/1023.0* curr_power_poti_max;
#endif

#if CONTROL_MODE == CONTROL_MODE_LIMIT_WH_PER_KM            //wh/km control mode
            buffer[9]=0;
            buffer[10]=1;
            power_poti = poti_stat / 1023.0 * whkm_max;        //power currently set by poti in relation to speed and maximum wattage per km
#endif
            do
            {
                buffer[7-i++] = (char)(((int)'0')+(power_poti % 10));
            }
            while ((power_poti /= 10) > 0);

            display_show_important_info(buffer, 1);
        }
    }
#endif
}

#if defined(SUPPORT_POTI_SWITCHES) || defined(SUPPORT_FIRST_AID_MENU)
void action_increase_poti()
{
    int new_stat = poti_stat + map(poti_level_step_size_in_watts, 0, curr_power_poti_max, 0, 1023);
    if (new_stat > 1023)
        new_stat = 1023;

    action_set_soft_poti(new_stat);
}

void action_decrease_poti()
{
    int new_stat = poti_stat - map(poti_level_step_size_in_watts, 0, curr_power_poti_max, 0, 1023);
    if (new_stat < 0)
        new_stat = 0;

    action_set_soft_poti(new_stat);
}

void action_set_fixed_poti()
{
    const int new_stat = map(poti_fixed_value_via_switch, 0, curr_power_poti_max, 0, 1023);
    action_set_soft_poti(new_stat);
}
#endif

static void action_fixed_throttle_value()
{
    throttle_stat = constrain(map(fixed_throttle_in_watts, 0, curr_power_max, 0, 1023), 0, 1023);
}

static void action_shutdown_system()
{
    // Shut down system
#if HARDWARE_REV >=2 || defined(TEENSY_VERSION)
    display_show_important_info(FROM_FLASH(msg_shutdown), 60);
    save_shutdown();
#endif
}

static void action_enable_backlight_long()
{
    // Toggle backlight
#ifdef SUPPORT_DISPLAY_BACKLIGHT
    enable_custom_backlight(60000);  //switch backlight on for one minute
#endif
}



static void action_toggle_bluetooth()
{
    // Toggle bluetooth
#if HARDWARE_REV >=2
    digitalWrite(bluetooth_pin, !digitalRead(bluetooth_pin));   //not available in 1.1
#endif
}


static void action_set_profile(const boolean new_profile)
{
    current_profile = new_profile;

    activate_new_profile();
}

#ifdef SUPPORT_LIGHTS_SWITCH
static void action_toggle_lights()
{
    lights_enabled = !lights_enabled;
    digitalWrite(lights_pin, !digitalRead(lights_pin));
}
#endif


static void handle_display_return(int result)
#if (DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
{
    switch (result)
    {
        case DISPLAY_ACTION_MENU_DISABLED:
            menu_active = false;
            break;
        case DISPLAY_ACTION_TOGGLE_LIGHTS:
            display_show_important_info("Toggled Lights", 0);
            action_toggle_lights();
            break;
        case DISPLAY_ACTION_TOGGLE_ACTIVE_PROFILE:
            display_show_important_info("Toggled Profile", 0);
            action_set_profile(!current_profile);
            break;
        case DISPLAY_ACTION_DISABLE_BRAKE:
            first_aid_ignore_break=true;
            break;
        case DISPLAY_ACTION_DISABLE_PAS:
            first_aid_ignore_pas=true;
            break;
        case DISPLAY_ACTION_DISABLE_WHEEL:
            first_aid_ignore_speed=true;
            break;
        case DISPLAY_ACTION_DISABLE_POTI:
            first_aid_ignore_poti=true;
            break;
        case DISPLAY_ACTION_DISABLE_THROTTLE:
            first_aid_ignore_throttle=true;
            break;
        case DISPLAY_ACTION_ENABLE_BRAKE:
            first_aid_ignore_break=false;
            break;
        case DISPLAY_ACTION_ENABLE_PAS:
            first_aid_ignore_pas=false;
            break;
        case DISPLAY_ACTION_ENABLE_WHEEL:
            first_aid_ignore_speed=false;
            break;
        case DISPLAY_ACTION_ENABLE_POTI:
            first_aid_ignore_poti=false;
            break;
        case DISPLAY_ACTION_ENABLE_THROTTLE:
            first_aid_ignore_throttle=false;
            break;
        case DISPLAY_ACTION_POTI_UP:
            action_increase_poti();
            break;
        case DISPLAY_ACTION_POTI_DOWN:
            action_decrease_poti();
            break;
        case DISPLAY_ACTION_SHUTDOWN:
            action_shutdown_system();
            break;
        case DISPLAY_ACTION_RESET_MAH:
            display_show_important_info("Battery reset", 0);
            mah = 0.0;
            wh = 0.0;
            break;
        case DISPLAY_ACTION_RESET_ODO:
            display_show_important_info("KM reset", 0);
            km = 0.0;
            break;
        default:
            break;
    }
#endif
}

static void action_enter_menu()
{
    if (menu_active)
        return;

#ifdef SUPPORT_DISPLAY_BACKLIGHT
    enable_custom_backlight(15 * 1000);  //switch backlight on for fifteen minute
#endif

    // Activate on the go menu
    menu_activity_expire = 0;
    menu_active = true;
    menu_changed = true;
#if !(DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
    // Reset to top level menu
    while (menu_system.back());
#else
    int res = keyPressed();
    handle_display_return(res);

#endif
}

#ifdef SUPPORT_GEAR_SHIFT
enum gear_shift { GEAR_LOW=0, GEAR_HIGH, GEAR_AUTO, _GEAR_END };
gear_shift current_gear = GEAR_AUTO;

static void action_set_gear(const gear_shift &new_gear)
{
    current_gear = new_gear;

    switch(current_gear)
    {
        case GEAR_LOW:
            digitalWrite(gear_shift_pin_low_gear, LOW);
            digitalWrite(gear_shift_pin_high_gear, HIGH);

            display_show_important_info(FROM_FLASH(msg_gear_shift_low_gear), 0);
            break;
        case GEAR_HIGH:
            digitalWrite(gear_shift_pin_low_gear, HIGH);
            digitalWrite(gear_shift_pin_high_gear, LOW);

            display_show_important_info(FROM_FLASH(msg_gear_shift_high_gear), 0);
            break;
        case GEAR_AUTO:
        default:
            digitalWrite(gear_shift_pin_low_gear, HIGH);
            digitalWrite(gear_shift_pin_high_gear, HIGH);

            display_show_important_info(FROM_FLASH(msg_gear_shift_auto_selection), 0);
            break;
    }
}

static void action_toggle_gear(bool include_auto)
{
    byte conv = static_cast<byte>(current_gear);
    gear_shift next_gear = static_cast<gear_shift>(conv+1);

    if (!include_auto && next_gear == GEAR_AUTO)
        next_gear = GEAR_LOW;

    // Safety protection and roll over in "include_auto" mode
    if (next_gear == _GEAR_END)
        next_gear = GEAR_LOW;

    action_set_gear(next_gear);
}
#endif

static void execute_action(const sw_action action)
{
    switch(action)
    {
        case ACTION_NONE:
            break;
        case ACTION_SET_SOFT_POTI:
            action_set_soft_poti(throttle_stat);
            break;
        case ACTION_SHUTDOWN_SYSTEM:
            action_shutdown_system();
            break;
        case ACTION_ENABLE_BACKLIGHT_LONG:
            action_enable_backlight_long();
            break;
        case ACTION_TOGGLE_BLUETOOTH:
            action_toggle_bluetooth();
            break;
        case ACTION_ENTER_MENU:
            action_enter_menu();
            break;
        case ACTION_PROFILE_1:
            action_set_profile(0);
            break;
        case ACTION_PROFILE_2:
            action_set_profile(1);
            break;
        case ACTION_PROFILE:
            action_set_profile(!current_profile);
            break;
        case ACTION_DISPLAY_PREV_VIEW:
            display_prev_view();
            break;
        case ACTION_DISPLAY_NEXT_VIEW:
            display_next_view();
            break;
#ifdef SUPPORT_LIGHTS_SWITCH
        case ACTION_TOGGLE_LIGHTS:
            action_toggle_lights();
            break;
#endif
#ifdef SUPPORT_POTI_SWITCHES
        case ACTION_INCREASE_POTI:
            action_increase_poti();
            break;
        case ACTION_DECREASE_POTI:
            action_decrease_poti();
            break;
        case ACTION_SET_FIXED_POTI_VALUE:
            action_set_fixed_poti();
            break;
#endif
        case ACTION_FIXED_THROTTLE_VALUE:
            action_fixed_throttle_value();
            break;
#ifdef SUPPORT_GEAR_SHIFT
        case ACTION_GEAR_SHIFT_LOW:
            action_set_gear(GEAR_LOW);
            break;
        case ACTION_GEAR_SHIFT_HIGH:
            action_set_gear(GEAR_HIGH);
            break;
        case ACTION_GEAR_SHIFT_AUTO:
            action_set_gear(GEAR_AUTO);
            break;
        case ACTION_GEAR_SHIFT_TOGGLE_LOW_HIGH:
            action_toggle_gear(false);
            break;
        case ACTION_GEAR_SHIFT_TOGGLE_LOW_HIGH_AUTO:
            action_toggle_gear(true);
            break;
#endif
        default:
            display_show_important_info(FROM_FLASH(msg_unknown_action), 2);
            break;
    }
}

// Switch handling code starts here
void handle_switch(const switch_name sw_name, boolean current_state, const switch_result &force_press)
{
    if (sw_name >= _SWITCHES_COUNT)
        return;

    enum switch_result res;
    if (force_press != PRESSED_NONE)
        res = force_press;
    else
        res = _read_switch(&switch_states[sw_name], current_state);

    if (res == PRESSED_NONE)
        return;

    // Handle control to menu system?
    if (menu_active)
    {
        _handle_menu_switch(sw_name, res);
        return;
    }

    // normal switch action
    switch(res)
    {
        case PRESSED_LONG:
            execute_action(switch_states[sw_name].action_long_press);
            break;
        case PRESSED_SHORT:
            execute_action(switch_states[sw_name].action_short_press);
            break;
        case PRESSED_NONE:
        default:
            break;
    }
}

// Workhose of switch handling: Detect short
// or long presses, also debounces the switches.
static enum switch_result _read_switch(switch_state *state, boolean switch_current)
{
    enum switch_result res = PRESSED_NONE;
    const unsigned long now = millis();

    if (switch_current==BUTTON_ON)
    {
        if (state->previous_state==BUTTON_OFF)
        {
            // first press
            state->first_press_time=now;
        }
        //No delay for boost button
        else if ((now - state->first_press_time)>10 && (state->trigger_every_time))
        {
            state->action_enabled = false;
            res = PRESSED_SHORT;
        }

        else if ((now - state->first_press_time)>1000 &&
                 (state->action_enabled || state->trigger_every_time))
        {
            state->action_enabled = false;
            res = PRESSED_LONG;
        }
    }
    else if (state->previous_state==BUTTON_ON
             && (now - state->first_press_time)>10
             && (now - state->first_press_time)<1000
             && state->action_enabled)
    {
        state->action_enabled = false;
        res = PRESSED_SHORT;
    }
    else
        state->action_enabled = true;

    state->previous_state = switch_current;

    return res;
}

static void _handle_menu_switch(const enum switch_name sw, const enum switch_result res)
{
#ifdef SUPPORT_DISPLAY_BACKLIGHT
    enable_custom_backlight(15 * 1000);  //switch backlight on for fifteen seconds
#endif
    int res_key = DISPLAY_ACTION_NONE;
    switch(res)
    {
        case PRESSED_SHORT:
            if (sw == MENU_BUTTON_DOWN)
#if (DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
                updatePosition(1);
#else
                menu_system.next(true);
#endif
            else if (sw == MENU_BUTTON_UP)
#if (DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
                updatePosition(-1);
#else
                menu_system.prev(true);
#endif
#if (DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
            else if (sw == MENU_BUTTON_SELECT) {
                res_key=keyPressed();
                handle_display_return(res_key);
            }
#endif
            menu_changed = true;
            break;
        case PRESSED_LONG:
#if (DISPLAY_TYPE & DISPLAY_TYPE_ILI22)
           res_key=keyPressed();
            handle_display_return(res_key);
#else
            menu_system.select();
#endif
            menu_changed = true;

            break;
        default:
            break;
    }
}
