#ifndef DEFAULT_HPP
#define DEFAULT_HPP

#include "chore.hpp"
#include "settings.hpp"
#include "Adafruit_NeoPixel.hpp"

extern Adafruit_NeoPixel rgb1, rgb2, rgb3;  // defined in main.cpp. Normally declared there too
/** for reference from chore.hpp
typedef enum {
    DAILY,
    SEMIWEEKLY,
    DAY_OF_MONTH,
    DAY_OF_WEEK,
    EVERY_TWO_WEEKS,
    EVERY_FOUR_WEEKS,
    PERIODIC
} chore_type_t;

typedef struct {
    uint8_t day_of_week_or_month : 5;
    uint8_t week_offset : 2;  // week 0 is the first week of the month (meaning first occurence of the day of the week)
    uint8_t current_week_index : 2;
    uint32_t time_seconds : 17;
    uint32_t epoch_delta;  // epochs are int64 but used uint32 to save space because there is no sane reason to have a negative interval
} chore_time_t;

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
 
typedef struct {
    uint8_t rgb_port;
    uint8_t index;
} rgb_pixel_t;
 */

 inline uint32_t time2secs(uint8_t hours, uint8_t minutes, uint8_t seconds) { return hours*60*60 + minutes*60 + seconds; }

//  (If this seems clunky, it's because you really shouldn't be using this anyway!)
// Note! Minimum GCC/G++ version is 10.4. I think. Anyway it works on 14.3.1; just update if you have issues related to the defaults.hpp file
const static chore_t default_chores[] = {
    {.str_id="Take drugs, morning, grace", .responsible_party="Grace", .port=1, .deadline={.time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=1}},
    {.str_id="Take drugs, morning, lydia", .responsible_party="Lydia", .port=2, .deadline={.time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=2}},
    {.str_id="Take drugs, midday, grace", .responsible_party="Grace", .port=3, .deadline={.time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=15*60, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=3}},
    {.str_id="Take drugs, midday, lydia", .responsible_party="Lydia", .port=4, .deadline={.time_seconds=time2secs(13, 0, 0)}, .warning_length_seconds=15*60, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=4}},
    {.str_id="Take drugs, evening, grace", .responsible_party="Grace", .port=5, .deadline={.time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=5}},
    {.str_id="Take drugs, evening, Lydia", .responsible_party="Lydia", .port=6, .deadline={.time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=6}},
    {.str_id="Check for dishes and food", .responsible_party="Grace", .port=7, .deadline={.time_seconds=time2secs(22, 0, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=7}},
    {.str_id="Take out trash", .responsible_party="Lydia", .port=8, .deadline={.time_seconds=time2secs(72, 0, 0)}, .warning_length_seconds=24*3600, .chore_type=PERIODIC, .rgb_stuff={.rgb_port=1, .index=8}},
    {.str_id="Vacuum rats", .responsible_party="Lydia", .port=9, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(23, 0, 0)}, .warning_length_seconds=24*3600, .chore_type=DAY_OF_WEEK, .rgb_stuff={.rgb_port=1, .index=9}},
    {.str_id="Clean rats", .responsible_party="Lydia", .port=10, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(23, 0, 0)}, .warning_length_seconds=24*3600, .chore_type=DAY_OF_WEEK, .rgb_stuff={.rgb_port=1, .index=10}},
    {.str_id="Clean dishwasher", .responsible_party="Grace", .port=11, .deadline={.time_seconds=48*3600}, .warning_length_seconds=12*3600, .chore_type=PERIODIC, .rgb_stuff={.rgb_port=1, .index=11}},
    {.str_id="Inspect/clean kitchen", .responsible_party="Grace", .port=12, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(22, 30, 0)}, .warning_length_seconds=4*3600, .chore_type=DAY_OF_WEEK, .rgb_stuff={.rgb_port=1, .index=12}},
    {.str_id="Brush teeth", .responsible_party="Both", .port=13, .deadline={.time_seconds=time2secs(22, 30, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=13}},
    {.str_id="Wash water bottles", .responsible_party="Grace", .port=14, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(22, 30, 0)}, .warning_length_seconds=3600, .chore_type=EVERY_TWO_WEEKS, .rgb_stuff={.rgb_port=1, .index=14}},
    {.str_id="Replace water filter", .responsible_party="Grace", .port=15, .deadline={.day_of_week_or_month=15, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=48*3600, .chore_type=DAY_OF_MONTH, .rgb_stuff={.rgb_port=1, .index=15}},
    {.str_id="Mop floors", .responsible_party="Grace", .port=16, .deadline={.day_of_week_or_month=15, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=48*3600, .chore_type=DAY_OF_MONTH, .rgb_stuff={.rgb_port=1, .index=16}},
    {.str_id="Clean bathroom", .responsible_party="Lydia", .port=17, .deadline={.time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=3600, .chore_type=DAILY, .rgb_stuff={.rgb_port=1, .index=17}},
    {.str_id="Check HVAC filter", .responsible_party="Lydia", .port=18, .deadline={.day_of_week_or_month=15, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=48*3600, .chore_type=DAY_OF_MONTH, .rgb_stuff={.rgb_port=2, .index=1}},
    {.str_id="Laundry, bedding", .responsible_party="Lydia", .port=19, .deadline={.day_of_week_or_month=2, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=24*3600, .chore_type=DAY_OF_WEEK, .rgb_stuff={.rgb_port=2, .index=2}},
    {.str_id="Replace water filter", .responsible_party="Grace", .port=20, .deadline={.day_of_week_or_month=15, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=48*3600, .chore_type=DAY_OF_MONTH, .rgb_stuff={.rgb_port=2, .index=3}},
    {.str_id="Laundry, clothes", .responsible_party="Lydia", .port=21, .deadline={.day_of_week_or_month=2, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=24*3600, .chore_type=DAY_OF_WEEK, .rgb_stuff={.rgb_port=2, .index=4}},
    {.str_id="Clean refrigerator", .responsible_party="Grace", .port=22, .deadline={.day_of_week_or_month=15, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=48*3600, .chore_type=DAY_OF_MONTH, .rgb_stuff={.rgb_port=2, .index=5}},
    {.str_id="Declutter table", .responsible_party="Grace", .port=23, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(22, 30, 0)}, .warning_length_seconds=3600, .chore_type=EVERY_TWO_WEEKS, .rgb_stuff={.rgb_port=2, .index=6}},
    {.str_id="Vaccum carpets, except rats", .responsible_party="Lydia", .port=24, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(22, 30, 0)}, .warning_length_seconds=3600, .chore_type=EVERY_TWO_WEEKS, .rgb_stuff={.rgb_port=2, .index=7}},
    {.str_id="Medical Documentation ", .responsible_party="Both", .port=25, .deadline={.day_of_week_or_month=5, .time_seconds=time2secs(22, 30, 0)}, .warning_length_seconds=3600, .chore_type=EVERY_TWO_WEEKS, .rgb_stuff={.rgb_port=2, .index=8}},
    {.str_id="Reconcile shared expenses ", .responsible_party="Both", .port=26, .deadline={.day_of_week_or_month=1, .time_seconds=time2secs(9, 30, 0)}, .warning_length_seconds=24*3600, .chore_type=DAY_OF_MONTH, .rgb_stuff={.rgb_port=2, .index=9}}
};



// functions
void load_defaults(Settings& settings);

#endif