#include <device.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(app);

#define SPI_MESSAGE 0xA5

// Defines another way to get CS pin configuration
// struct spi_cs_control spi_cs = {
//     .gpio_dev = DEVICE_DT_GET(DT_SPI_DEV_CS_GPIOS_CTLR(DT_NODELABEL(spidev))),
//     .gpio_pin = DT_SPI_DEV_CS_GPIOS_PIN(DT_NODELABEL(spidev)),
//     .gpio_dt_flags = DT_SPI_DEV_CS_GPIOS_FLAGS(DT_NODELABEL(spidev)),
//     .delay = 10,
// };

const struct spi_config spi_cfg = {
    .frequency = DT_PROP(DT_NODELABEL(spi0), clock_frequency),
    .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
    .cs = SPI_CS_CONTROL_PTR_DT(DT_NODELABEL(spidev), 10),
};

void main(void)
{
    uint8_t cmd = SPI_MESSAGE;
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
