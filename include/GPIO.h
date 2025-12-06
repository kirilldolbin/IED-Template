#include <cstdint>
#include <gpiod.hpp>
#include <list>

class GPIO
{
public:
    explicit GPIO();

    ~GPIO();

    void SetValue(bool value);

private:
    gpiod::chip chip;
    std::list<gpiod::line_request> requests;
    int gpio_write_pin=110;
};