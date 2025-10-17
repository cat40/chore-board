#include "chore.hpp"
#include "dll.hpp"

extern "C"
{
#include "data_structures.h"
}

#include <array>

Chores::Chores(void)
{

}

chore_t* Chores::get_chore_by_port(uint8_t port)
{
    // todo implement proper iterator
    for (uint8_t i=0; i<NUM_CHORES; i++)
    {
        chore_t* chore = chores[i];
        if (chore->port == port)
        {
            return chore;
        }
    }
    return nullptr;
}

void Chores::update_chore_status(rtc_reading_t rtc_reading, uint8_t max_overdue_chores)  // updates chore status but does not change the shown colors
{        
    chore_t* chore;
    uint8_t num_overdue = 0;
    for (uint8_t i=0; i<NUM_CHORES; i++)
    {
        chore = chores[i];
        chore_status_t chore_status = check_chore_status(rtc_reading, chore);
        // todo update these to use colors defined in the settings
        switch (chore_status)
        {
            case GOOD:
                chore->color = GOOD_COLOR;
                break;
            case WARNING:
                chore->color = WARNING_COLOR;
                break;
            case OVERDUE:
                chore->color = num_overdue++ < max_overdue_chores ? OVERDUE_COLOR : OVERDUE_LOW_PRIORITY_COLOR;
                break;
            case INVALID:
            default:
                break;
        }
    }
}
chore_status_t Chores::check_chore_status(rtc_reading_t time, chore_t* chore)
{
    uint32_t time_seconds = time.second + 60*time.minute + 60*60*time.hour;
    if (chore->chore_type == PERIODIC)
    {
        int64_t current_epoch = rtc_reading_to_epoch(time);
        int64_t time_since_last_done = current_epoch - chore->time_last_done;
        if(time_since_last_done > chore->deadline.epoch_delta)
        {
            return OVERDUE;
        }
        else if(time_since_last_done >= (chore->deadline.epoch_delta-chore->warning_length_seconds))
        {
            return WARNING;
        }
        return GOOD;
    }
    else if (chore->chore_type == DAY_OF_WEEK)
    {
        if (time.weekday == chore->deadline.day_of_week_or_month)
        {
            if (time_seconds > chore->deadline.time_seconds)
            {
                return OVERDUE;
            }
            else if (time_seconds >= (chore->deadline.time_seconds- chore->warning_length_seconds))
            {
                return WARNING;
            }
            return GOOD;
        }
    }
    else if (chore->chore_type == EVERY_TWO_WEEKS || chore->chore_type == EVERY_FOUR_WEEKS)
    {
        uint8_t modulus = chore->chore_type == EVERY_TWO_WEEKS ? 2 : 4;
        if (time.weekday == chore->deadline.day_of_week_or_month)
        {
            chore->deadline.current_week_index += 1;
            chore->deadline.current_week_index %= modulus;
            if (chore->deadline.current_week_index == chore->deadline.week_offset)
            {
                if (time_seconds > chore->deadline.time_seconds)
                {
                    return OVERDUE;
                }
                else if (time_seconds >= (chore->deadline.time_seconds- chore->warning_length_seconds))
                {
                    return WARNING;
                }
            }
            return GOOD;
        }
    }
    else if (chore->chore_type == DAY_OF_MONTH)
    {
        if (time.day == chore->deadline.day_of_week_or_month)
        {
            if (time_seconds > (chore->deadline.time_seconds))
            {
                return OVERDUE;
            }
            else if (time_seconds >= ((chore->deadline.time_seconds) - chore->warning_length_seconds))
            {
                return WARNING;
            }
            return GOOD;
        }
    }
    return INVALID;
}

void Chores::reorder_chores(uint8_t ports[NUM_CHORES])
{
    // the dumb inefficent way, but it's simple and works
    // todo swap i and j for clarity
    chore_t* temp_chores[NUM_CHORES];
    // make new chores list
    for (uint8_t i=0; i<NUM_CHORES; i++)
    {
        temp_chores[i] = get_chore_by_port(ports[i]);
    }
    // copy new list into old one
    for (uint8_t i=0; i<NUM_CHORES; i++)
    {
        chores[i] = temp_chores[i];
    }
}

void Chores::set_chore_priority(uint8_t port, uint8_t priority) // 0 is lowest. Will insert the chore at this level, pushing the ones after down. 
{
    // todo implement this
}

uint8_t Chores::get_chores_on_rgb(uint8_t rgb_num, chore_t** chore_list)
{
    uint8_t j = 0;
    for (uint8_t i=0; i<NUM_CHORES; i++)
    {
        if (chores[i]->rgb_stuff.rgb_port == rgb_num)
        {
            chore_list[j++] = chores[i];
        }
    }
    return j;
}