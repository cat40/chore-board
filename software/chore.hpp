#ifndef CHORE_H
#define CHORE_H

#include <stdint.h>
#include <array>

extern "C"
{
#include "config.h"
}

#include "pcf8523.hpp"
#include "dll.hpp"

typedef enum {
    GOOD,
    WARNING,
    OVERDUE,
    INVALID
} chore_status_t;

typedef enum {
    DAILY,  // todo handle this case
    DAY_OF_MONTH,
    DAY_OF_WEEK,
    EVERY_TWO_WEEKS,
    EVERY_FOUR_WEEKS,
    PERIODIC
} chore_type_t;

typedef struct {
    // for daily/weekly/etc chores
    uint8_t day_of_week_or_month : 5;
    uint8_t week_offset : 2;  // week 0 is the first week of the month (meaning first occurence of the day of the week)
    uint8_t current_week_index : 2;
    uint32_t time_seconds : 17;
    // for periodic chores
    uint32_t epoch_delta;  // epochs are int64 but used uint32 to save space because there is no sane reason to have a negative interval
} chore_time_t;

typedef struct {
    uint8_t rgb_port;
    uint8_t index;
} rgb_pixel_t;

typedef struct {
    char str_id[ID_LENGTH];
    char responsible_party[PARTY_LENGTH];
    uint8_t port;
    chore_time_t deadline;
    uint32_t warning_length_seconds;
    chore_type_t chore_type;
    int64_t time_last_done;  // unix epoch
    uint32_t color;
    rgb_pixel_t rgb_stuff;
} chore_t;

class Chores
{
    public:
        Chores(void);
        void reorder_chores(uint8_t ports[NUM_CHORES]);
        chore_t* get_chore_by_port(uint8_t port);
        void update_chore_status(rtc_reading_t reading, uint8_t max_overdue_chores);  // updates chore status but does not change the shown colors
        void set_chore_priority(uint8_t port, uint8_t priority); // 0 is lowest. Will insert the chore at this level, pushing the ones after down. 
        uint8_t get_chores_on_rgb(uint8_t rgb_num, chore_t** chore_list); // returns the number of chores on it
        // DoubleLinkedList<chore_t*> chore_list;
        chore_t* chores[NUM_CHORES];
        static chore_status_t check_chore_status(rtc_reading_t time, chore_t* chore);
};

#endif