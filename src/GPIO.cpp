#include "GPIO.h"
#include <iostream>

constexpr std::string chip_name("/dev/gpiochip1");

GPIO::GPIO() : chip(chip_name)
{
    gpiod::line_settings settings;
    settings.set_direction(gpiod::line::direction::OUTPUT);
    settings.set_output_value(gpiod::line::value::ACTIVE); // Start with active state (on)

    // 3. Prepare a request
    requests.push_back(chip.prepare_request()
                           .set_consumer("Dolbin")
                           .add_line_settings(gpio_write_pin, settings)
                           .do_request());

    std::cout << "LED on GPIO " << gpio_write_pin << " blinking..." << std::endl;
}

GPIO::~GPIO(){}

void GPIO::SetValue(bool value)
{
    if (value)
    {
        requests.front().set_value(gpio_write_pin, gpiod::line::value::ACTIVE);
    }
    else
    {
        requests.front().set_value(gpio_write_pin, gpiod::line::value::INACTIVE);
    }
    std::cout << "получил значение " << value << std::endl;
}
