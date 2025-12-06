#include "GPIO.h"
#include <iostream>
#include <array>

constexpr std::string chip_name("/dev/gpiochip1");
constexpr std::array<int, 4> indexis = {12, 11, 110, 68};
GPIO::GPIO() : chip(chip_name)
{
    gpiod::line_settings settings;
    settings.set_direction(gpiod::line::direction::OUTPUT);
    settings.set_output_value(gpiod::line::value::INACTIVE); // Start with active state (on)
    for (auto index : indexis)
    {
        // 3. Prepare a request
        requests.push_back(chip.prepare_request()
                               .set_consumer("Dolbin")
                               .add_line_settings(index, settings)
                               .do_request());
    }

}

GPIO::~GPIO() {}

void GPIO::SetValue(bool value, int gpio_pin)
{
    if (value)
    {
        requests.at(gpio_pin).set_value(indexis.at(gpio_pin), gpiod::line::value::ACTIVE);
    }
    else
    {
        requests.at(gpio_pin).set_value(indexis.at(gpio_pin), gpiod::line::value::INACTIVE);
    }
    std::cout << "получил значение " << value << std::endl;
}
