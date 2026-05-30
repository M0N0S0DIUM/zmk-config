// SPDX-License-Identifier: MIT

#include <zephyr/init.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(cm1000_board, CONFIG_ZMK_LOG_LEVEL);

/*
 * BQ24075 charge current via ISET resistor network:
 *
 *   ISET_10K (P0.26)  ISET_3K9 (P1.15)  Charge current
 *   Hi-Z              Hi-Z               0 mA  (charging off)
 *   Low               Hi-Z               ~89 mA
 *   Hi-Z              Low                ~228 mA
 *   Low               Low                ~318 mA  (default)
 *
 * Both pins driven low at boot for ~318 mA (0.16C on 2000 mAh cell).
 */

#define ISET_10K_NODE DT_NODELABEL(iset_10k)
#define ISET_3K9_NODE DT_NODELABEL(iset_3k9)

static const struct gpio_dt_spec iset_10k = GPIO_DT_SPEC_GET(ISET_10K_NODE, gpios);
static const struct gpio_dt_spec iset_3k9 = GPIO_DT_SPEC_GET(ISET_3K9_NODE, gpios);

static int cm1000_board_init(void) {
    int ret;

    if (!gpio_is_ready_dt(&iset_10k)) {
        LOG_ERR("ISET 10k GPIO not ready");
        return -ENODEV;
    }
    if (!gpio_is_ready_dt(&iset_3k9)) {
        LOG_ERR("ISET 3.9k GPIO not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&iset_10k, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure ISET 10k pin: %d", ret);
        return ret;
    }

    ret = gpio_pin_configure_dt(&iset_3k9, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure ISET 3.9k pin: %d", ret);
        return ret;
    }

    LOG_INF("BQ24075 ISET: 350mA charge current enabled");
    return 0;
}

SYS_INIT(cm1000_board_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);
