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
        }
    }
}
chore_status_t Chores::check_chore_status(rtc_reading_t time, chore_t* chore)
{
    if (chore->chore_type == PERIODIC)
    {
        int64_t current_epoch = rtc_reading_to_epoch(time);
        int64_t time_since_last_done = current_epoch - chore->time_last_done;
        if(time_since_last_done/60 > chore->deadline)
        {
            return OVERDUE;
        }
        else if(time_since_last_done/60 >= (chore->deadline-chore->warning_length_mintues))
        {
            return WARNING;
        }
        return GOOD;
    }
    else if (chore->chore_type == DAY_OF_WEEK)
    {
        if (time.weekday == (chore->deadline & 0xff))
        {
            uint32_t time_mintues = time.minute + 60*time.hour;
            if (time_mintues > (chore->deadline>>8))
            {
                return OVERDUE;
            }
            else if (time_mintues >= ((chore->deadline>>8) - chore->warning_length_mintues))
            {
                return WARNING;
            }
            return GOOD;
        }
    }
    else if (chore->chore_type == DAY_OF_MONTH)
    {
        if (time.day == chore->deadline&0xff)
        {
            uint32_t time_mintues = time.minute + 60*time.hour;
            if (time_mintues > (chore->deadline>>8))
            {
                return OVERDUE;
            }
            else if (time_mintues >= ((chore->deadline>>8) - chore->warning_length_mintues))
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
    // std::array<chore_t, NUM_CHORES> new_chores;
    // for (uint8_t i_port=0; i_port<NUM_CHORES; i_port++)
    // {
    //     for (uint8_t j_chore=0; j_chore<NUM_CHORES; j_chore++)
    //     {
    //         if (ports[i_port] == this->chores[j_chore].port)
    //         {
    //             new_chores[i_port] = this->chores[j_chore];
    //             break;
    //         }
    //     }
    // }
    // this->chore_list = new_chores;
}