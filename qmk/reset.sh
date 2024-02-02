#!/usr/bin/env bash


# See: https://github.com/qmk/qmk_firmware/pull/14134
resetFile='util/reset.eep' # since 0.14.0
if [ ! -f "$resetFile" ]; then
    resetFile='quantum/tools/eeprom_reset.hex' # before 0.14.0
fi
if [ ! -f "$resetFile" ]; then
    echo "Neither reset file 'util/reset.eep' nor 'quantum/tools/eeprom_reset.hex' exist. Cannot reset mcu."
    exit 1
fi

unset USB
unset avrdudeConfig

if [ -n "$1" ] && [ -n "$2" ]; then
    USB="$1"
    avrdudeConfig="$2"

else
    # See: platforms/avr/flash.mk

    printf "Waiting for USB serial port - reset your controller now (Ctrl+C to cancel)"

    TMP1=`mktemp`
    TMP2=`mktemp`
    unset USB

    ls /dev/tty* > $TMP1

    while [ -z "$USB" ]; do
        sleep 0.5
        printf "."
        ls /dev/tty* > $TMP2
        USB=`comm -13 $TMP1 $TMP2 | grep -o '/dev/tty.*'`
        mv $TMP2 $TMP1
    done
    printf "\n"
    rm $TMP1

    echo
    echo "Device $USB has appeared; assuming it is the controller."
    printf "Waiting for $USB to become writable."
    while [ ! -w "$USB" ]; do
        sleep 0.5
        printf "."
    done
    echo ""

    avrdudeConfig=$(avrdude -v 2>&1 | grep -o '\".*avrdude\.conf\"' | tr -d '"')
fi

avrdude -p atmega32u4 -c avr109 -U eeprom:w:${resetFile}:i -P "$USB" -C "$avrdudeConfig"
