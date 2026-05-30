# Adjust flash partitions for Adafruit UF2 bootloader
# App region: 0x00000000 - 0x000F3FFF
# Bootloader: 0x000F4000 - 0x000FFFFF

set(DT_FLASH_APP_SIZE 0xF4000)
