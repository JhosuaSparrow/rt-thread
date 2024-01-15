# @Author: Jack Sun
# @Date:   2024-01-08 13:56:52
# @Last Modified by:   jack.sun
# @Last Modified time: 2024-01-12 11:26:49

TAGET_CONFIG_FILE=hc32f40a.cfg

$OPENOCD_PATH/bin/openocd \
    -f $OPENOCD_PATH/share/openocd/scripts/interface/cmsis-dap.cfg \
    -f $OPENOCD_PATH/share/openocd/scripts/target/$TAGET_CONFIG_FILE \
    -c init -c halt -c "flash write_image erase rtthread.bin 0x00000000" -c reset -c shutdown
