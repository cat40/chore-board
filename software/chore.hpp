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
    DAY_OF_MONTH,
    DAY_OF_WEEK,
    PERIODIC
} chore_type_t;

typedef struct {
    char str_id[ID_LENGTH];
    uint8_t port;
    uint32_t deadline;  // interpretation changes depending on chore type. See comment below
    uint32_t warning_length_mintues;
    chore_type_t chore_type;
    int64_t time_last_done;  // unix epoch
    uint32_t color;
} chore_t;
/* Deadline interpretation by chore type:
    * Day of month: LSB is month day (1-28). Don't put 29, 30, or 31 if you value your sanity... The rest is the time of day in mintues
    * Day of week: Week day (0-6). Sunday is 0.
    * Periodic: Interval in mintues. 
    * todo add a setting to let the first two set what time of day to trigger
*/

class Chores
{
    public:
        Chores(void);
        void reorder_chores(uint8_t ports[NUM_CHORES]);
        chore_t* get_chore_by_port(uint8_t port);
        void update_chore_status(rtc_reading_t reading, uint8_t max_overdue_chores);  // updates chore status but does not change the shown colors
        void set_chore_priority(uint8_t port, uint8_t priority); // 0 is lowest. Will insert the chore at this level, pushing the ones after down. 
        // DoubleLinkedList<chore_t*> chore_list;
        chore_t* chores[NUM_CHORES];
        static chore_status_t check_chore_status(rtc_reading_t time, chore_t* chore);
};

#endif