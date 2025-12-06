#include <cstdint>
#include <gpiod.hpp>

class GPIO
{
public:
    explicit GPIO();

    ~GPIO();

    void SetValue(bool value);

private:
    gpiod::chip chip;
};