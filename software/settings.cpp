#include "settings.hpp"
#include "eeprom.hpp"

#include <stdint.h>

#include <cstring>

Settings::Settings(Eeprom* eeprom)
{
    this->eeprom = eeprom;
}

void Settings::write(void)
{
    uint8_t i2c_buffer[sizeof(packet)];
    memcpy(i2c_buffer, &packet, sizeof(packet));
    eeprom->write_data(0, i2c_buffer, sizeof(packet));
}

void Settings::read(void)
{
    uint8_t i2c_buffer[sizeof(packet)];
    eeprom->read_data(0, i2c_buffer, sizeof(packet));
    memcpy(&packet, i2c_buffer, sizeof(packet));
}

chore_t* Settings::get_chore(uint8_t port)
{
    return packet->chores.get_chore_by_port(port);
}

void Settings::set_chore(uint8_t port, chore_t* chore)
{
    // todo implement this
}
