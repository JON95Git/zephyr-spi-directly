#include <device.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(app);

struct spi_cs_control spi_cs = {
    .gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
    .gpio_pin = 25,
    .gpio_dt_flags = GPIO_ACTIVE_LOW,
    .delay = 10,
};

#define SPI_CS (&spi_cs)

const struct spi_config spi_cfg = {
    .frequency = 20000000,
    .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
    .cs = SPI_CS,
};

void main(void)
{
    uint8_t cmd = 0xA5;
    struct spi_buf tx_buf = {.buf = &cmd, .len = 1};
    struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};
    const struct device *spi;

    spi = device_get_binding("SPI_0");
    if (!device_is_ready(spi)) {
        LOG_ERR("Device SPI not ready, aborting test");
        return;
    }

    while (1) {
        spi_write(spi, &spi_cfg, &tx_bufs);
        k_sleep(K_MSEC(1000));
    }
}
