#include <cstdint>
#include <gpiod.hpp>
#include <vector>

class GPIO
{
public:
    explicit GPIO();

    ~GPIO();

    void SetValue(bool value, int gpio_pin);

private:
    gpiod::chip chip;
    std::vector<gpiod::line_request> requests;
    int gpio_write_pin=110;
};