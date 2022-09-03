#include "hc595.h"
#include <stdio.h>

#ifdef _DEBUG
#include <stdio.h>
#endif // _DEBUG

hc595::hc595(spi_inst_t *spi, uint ds, uint shclk, uint stclk, size_t count) : _link_count(count), _ds(ds), _shclk(shclk), _stclk(stclk), _spi(spi)
{
    // spi functoin pins init
    spi_init(this->_spi, DEFAULT_SPI_BAUDRATE);
    gpio_set_function(this->_ds, GPIO_FUNC_SPI);    // mosi
    gpio_set_function(this->_shclk, GPIO_FUNC_SPI); // sck

    gpio_init(this->_stclk);                        // cs
    gpio_set_dir(this->_stclk, GPIO_OUT);
    gpio_put(this->_stclk, 1);

    // this->clear();
}

void hc595::write(const uint8_t *buf)
{
    this->_begin();
    this->_write_raw(buf);
    this->_out();
}

inline void hc595::_write_raw(const uint8_t *src)
{
    this->_begin();
    spi_write_blocking(this->_spi, src, this->_link_count);
    this->_out();
}

void hc595::clear()
{
    auto buf = new uint8_t[this->_link_count];
    for (int i = 0; i < this->_link_count; i++)
    {
        buf[i] = 0x00;
    }
    this->_write_raw(buf);
    delete[] buf;
}

inline void hc595::_begin()
{
    // asm volatile("nop \n nop \n nop");
    gpio_put(this->_stclk, 0);  // Active low
    // asm volatile("nop \n nop \n nop");
}

inline void hc595::_out()
{
    // asm volatile("nop \n nop \n nop");
    gpio_put(this->_stclk, 1);
    // asm volatile("nop \n nop \n nop");
}

hc595::~hc595()
{
    spi_deinit(this->_spi);
}
