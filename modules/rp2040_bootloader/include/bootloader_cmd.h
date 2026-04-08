#ifndef BOOTLOADER_CMD_H_
#define BOOTLOADER_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_RP2040_BOOTLOADER)
int cmd_bootloader(const struct shell *sh, int argc, char **argv);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BOOTLOADER_CMD_H_ */
