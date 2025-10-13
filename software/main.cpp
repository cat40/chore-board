#include "pico/stdlib.h"
#include "pico/stdio.h"

extern "C"
{
#include "config.h"
#include "utilities.h"
#include "data_structures.h"
#include "command.h"
}

#include <stdio.h>
#include <stdint.h>

#include "Adafruit_NeoPixel.hpp"
#include "button_matrix.hpp"
#include "settings.hpp"
#include "eeprom.hpp"
#include "pcf8523.hpp"
#include "chore.hpp"
#include "uart.hpp"

Adafruit_NeoPixel rgb1(DEFAULT_NUM_PIXELS, RGB_1_PIN, PIXEL_PARAMETERS);
Adafruit_NeoPixel rgb2(DEFAULT_NUM_PIXELS, RGB_2_PIN, PIXEL_PARAMETERS);
Adafruit_NeoPixel rgb3(DEFAULT_NUM_PIXELS, RGB_3_PIN, PIXEL_PARAMETERS);

ButtonMatrix buttons(MATRIX_ROWS, MATRIX_COLUMNS, NUM_MATRIX_ROWS, NUM_MATRIX_COLUMNS, 200);

Eeprom eeprom (I2C, EEPROM_ADDRESS);
Settings settings(&eeprom);

Pcf8523 rtc(I2C);
rtc_reading_t rtc_reading;
rtc_reading_t DEFAULT_RTC_READING = { .second=21, .minute=43, .hour=5, .day=1, .weekday=0, .month=4, .year=2260 };
uint8_t DEFAULT_CENTURY = 22;

uint32_t rgb1_colors[NUM_CHORES];
uint32_t rgb2_colors[NUM_CHORES];
uint32_t rgb3_colors[NUM_CHORES];
chore_t* rgb1_chores[NUM_CHORES];
chore_t* rgb2_chores[NUM_CHORES];
chore_t* rgb3_chores[NUM_CHORES];
uint8_t rgb1_num, rgb2_num, rgb3_num;

Chores chores;

void setup(void);
void blink(void);
void update_rgb_port(Adafruit_NeoPixel& port, uint8_t count, chore_t* chores[], uint32_t colors[]);

int main(void)
{
    stdio_usb_init();

    busy_wait_ms(STARTUP_WAIT);

    printf("Starting program chore-board, hash: %s\n", COMMIT_HASH);
    printf("Data structure sizes:\n");
    printf("Chore struct: %d\n", sizeof(chore_t));
    printf("Chores object: %d\n", sizeof(chores));
    printf("Settings packet:%d \n", sizeof(settings_packet_t));

    setup();

    printf("Done setup\n");

    rgb1.updateLength(30);
    

    uint32_t hue = 0;

    // todo pull chore list from settings

    // while (1)
    // {
    //     rgb1.fill(rgb1.ColorHSV(hue));
    //     rgb1.setBrightness(0x0f);
    //     rgb1.show();
    //     busy_wait_ms(25);
    //     hue += 256;
    //     printf("color: %u\n", hue);
    // }

    uint8_t led_status = 0;
    busy_wait_ms(5000);
    while(1)
    {
        printf("main loop\n");
        // poll_command();
        rtc.get_reading(&rtc_reading);
        printf("rtc reading\n");
        printf("RTC reading: %02d%02d-%02d-%02d %02d:%02d:%02d\n", rtc.century, rtc_reading.year, rtc_reading.month, rtc_reading.day, rtc_reading.hour, rtc_reading.minute, rtc_reading.second);
        // chores.update_chore_status(rtc_reading, settings.packet->max_overdue_chores);
        // printf("update chores\n");
        // rgb1_num = chores.get_chores_on_rgb(1, rgb1_chores);
        // rgb2_num = chores.get_chores_on_rgb(2, rgb2_chores);
        // rgb3_num = chores.get_chores_on_rgb(3, rgb3_chores);
        printf("get chores\n");
        printf("%d, %d, %d\n", rgb1_num, rgb2_num, rgb3_num);
        update_rgb_port(rgb1, rgb1_num, rgb1_chores, rgb1_colors);  // the problem seems to be passing the neopixel object as a value and not a pointer
        update_rgb_port(rgb2, rgb2_num, rgb2_chores, rgb2_colors);
        update_rgb_port(rgb3, rgb3_num, rgb3_chores, rgb3_colors);
        // rgb1.show();
        // rgb2.show();
        // rgb3.show();
        busy_wait_ms(5000);  // give the rtc time to actually be an rtc
        printf("update port finished\n");
        blink();
    }
}

void setup(void)
{
    setup_digital_output(FUNSIES_LED_PIN, 1);

    i2c_init(I2C, I2C_CLOCK_SPEED);
    // gpio_init(I2C_SDA);
    // gpio_init(I2C_SCL);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);
    gpio_pull_up(I2C_SDA);
    // buttons.init();
    rtc.init(true);
    rtc.set_time(&DEFAULT_RTC_READING);
    rtc.get_reading(&rtc_reading);
    printf("rtc reading\n");
    printf("RTC reading: %02d%02d-%02d-%02d %02d:%02d:%02d\n", rtc.century, rtc_reading.year, rtc_reading.month, rtc_reading.day, rtc_reading.hour, rtc_reading.minute, rtc_reading.second);
    // chores.update_chore_status(rtc_reading, settings.packet->max_overdue_chores);

}

void blink(void)
{
    printf("blink\n");
    busy_wait_ms(1000);
    gpio_put(FUNSIES_LED_PIN, 1);
    busy_wait_ms(1000);
    gpio_put(FUNSIES_LED_PIN, 0);
}

void update_rgb_port(Adafruit_NeoPixel& port, uint8_t count, chore_t* chores[], uint32_t colors[])
{
    printf("update_rgb_port with count %d\n", count);
    if (count > 0)
    {
        port.updateLength(count);
        for (uint8_t i=0; i<count; i++)
        {
            chore_t* chore = chores[i];
            colors[chore->rgb_stuff.index] = chore->color;
        }
        for (uint8_t i=0; i<count; i++)
        {
            port.setPixelColor(i, colors[i]);
        }
    }
    else
    {
        port.setBrightness(0);
    }
}