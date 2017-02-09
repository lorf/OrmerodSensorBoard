# See https://community.platformio.org/t/burn-bootloader-to-attiny85-via-arduinoasisp/662/9

Import("env")

env.Replace(
    UNLOCKCMD='$UPLOADER $UPLOADERFLAGS -V -Ulock:w:0x3F:m',
    SETFUSECMD='$UPLOADER $UPLOADERFLAGS -Uefuse:w:0xFE:m -Uhfuse:w:0xD5:m -Ulfuse:w:0xE2:m',
    UPLOADBOOTCMD='$UPLOADER $UPLOADERFLAGS -Uflash:w:$SOURCES:i',
    LOCKCMD='$UPLOADER $UPLOADERFLAGS -V -Ulock:w:0x2F:m'
)

bootloader_path = "bootloaders/optiboot_attiny85_tuned.hex"
#bootloader_path = "bootloaders/optiboot_attiny85.hex"
uploadboot = env.Alias(
    "uploadboot", bootloader_path,
    [
        env.VerboseAction("$UNLOCKCMD", "Unlocking flash"),
        #env.VerboseAction("$SETFUSECMD", "Setting fuses"),
        env.VerboseAction("$UPLOADBOOTCMD", "Uploading bootloader $SOURCE"),
        env.VerboseAction("$LOCKCMD", "Locking flash")
    ])
AlwaysBuild(uploadboot)
