#include "defaults.hpp"
#include "settings.hpp"

void load_defaults(Settings& settings)
{
    for (chore_t chore : default_chores)
    {
        settings.set_chore(chore.port, &chore);
    }
    settings.write();
}