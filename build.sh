#!/bin/bash

if ! [ "`grep -e ^8 -e ^9 /etc/debian_version`" ] ; then
echo compile need debian8/debian9 system
fi

if ! [ "`which arm-none-eabi-gcc`" ] ; then
apt install gcc-arm-none-eabi make
fi

case "$1" in
  LV|CM|CL|DB|GB)
    mkdir -p firmware/$1
    cp utils/mt2625_bootloader.bin utils/flash_download.cfg firmware/$1
    echo Making $1 Version
    echo HWVER :=$1 >utils/config.mk
    echo LINK_FILE:=link_option_$1.tmp >>utils/config.mk
    make
    ;;
  CLEAN|clean)
    make clean
    ;;
esac

