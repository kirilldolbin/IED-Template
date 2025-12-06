    #include "GPIO.h"
    #include <iostream>

    GPIO::GPIO(): chip("gpiochip1")
    {
        gpiod::line led = chip.get_line(LED_LINE_OFFSET);
        gpiod::line_request config;
        config.consumer     = "Blink";
        config.request_type = gpiod::line_request::DIRECTION_OUTPUT;
        led.request(config);
    }

    GPIO::~GPIO()
    {
    }

    void GPIO::SetValue(bool value)
    {
        std::cout << "получил значение " << value << std::endl;
    }
