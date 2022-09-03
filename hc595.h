#pragma once
#include <stddef.h>
#include "pico/types.h"
#include "pico.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

// default port definations
#ifndef spi_default
#define spi_default spi0
#endif
#define PICO_DEFAULT_SPI_SCK 2
#define PICO_DEFAULT_SPI_MOSI 3
#define PICO_DEFAULT_SPI_MISO 4
#define PICO_DEFAULT_SPI_CS 5

// default baudrate
#ifndef DEFAULT_SPI_BAUDRATE
#define DEFAULT_SPI_BAUDRATE (5000*1000)
#endif  // DEFAULT_SPI_BAUDRATE

/*! \brief  hc595 class based on spi
*/
class hc595
{
private:
    size_t _link_count; // how many 595 chips are linked
    uint _ds;           // data serial
    uint _shclk;        // shift register clock input
    uint _stclk;        // storage register clock input
    spi_inst_t *_spi;   // spi port

    /*! \brief  write the buffer to 595. It takes 3*sleep_time us to send every byte of data. In this case, 6 us/B
    * \param src buffer to write
    */
    inline void _write_raw(const uint8_t *src);

    /*! \brief  begin transmission */
    inline void _begin();

    /*! \brief  send a rise to storage register clock input, move the data to output */
    inline void _out();

public:
    /*! \brief  Create a 74hc595 link object on a specified i2c port. 
    * \param spi SPI instance specifier
    * \param ds GPIO number to serial data input pin on 595. default = PICO_DEFAULT_SPI_MOSI
    * \param shclk GPIO number to shift register clock input pin on 595. default = PICO_DEFAULT_SPI_SCK
    * \param stclk GPIO number to storage register clock input pin on 595. default = PICO_DEFAULT_SPI_CS
    * \param count The count of 595 link. default = 1
    */
    hc595(spi_inst_t *spi = PICO_DEFAULT_SPI, 
          uint ds = PICO_DEFAULT_SPI_MOSI,
          uint shclk = PICO_DEFAULT_SPI_SCK,
          uint stclk = PICO_DEFAULT_SPI_CS,
          size_t count = 1);

    /*! \brief  Write data to 595 from the buffer
    * \param buf Pointer to data array. Note: the size of the buffer MUST not be shorter than the count of the 595 chip in the link, or could lead to memory leak. 
    */
    void write(const uint8_t *buf);

    /*! \brief  Clear all 595s */
    void clear(void);
    ~hc595();
};
