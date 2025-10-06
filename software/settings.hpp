#ifndef SETTINGS_H
#define SETTINGS_H

extern "C"
{
#include "data_structures.h"
}

#include <eeprom.hpp>
#include "chore.hpp"
#include <stdint.h>

// void write_settings(void);
// void read_settings(void);

typedef struct {
    Chores chores;
    uint32_t good_color;
    uint32_t warn_color;
    uint32_t overdue_color;
    uint32_t low_priority_overdue_color;
    uint8_t  max_overdue_chores; // max before lower priority chores get assigned a different color
} settings_packet_t;

class Settings
{
    public:
        Settings(Eeprom* eeprom);
        void write(void);
        void read(void);
        chore_t* get_chore(uint8_t port);
        void set_chore(uint8_t port, chore_t* chore);
        settings_packet_t* packet;
    private:
        Eeprom* eeprom;
};

#endif