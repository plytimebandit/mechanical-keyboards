#!/usr/bin/env bash

if [ "$1" = '-c' ]; then
  echo "Cleaning build directory."
  rm -rf "$HOME/dev/repos/zmk/app/build"
elif [ "$1" = '-f' ]; then
  echo "Flashing '$2' in 10 seconds ..."
  sleep 10
  if ! ls /media/*/XIAO-SENSE &> /dev/null; then
    echo "Target directory '/media/*/XIAO-SENSE' does not exist. Device mounted?"
    exit 1
  fi
  if [ "$2" = 'left' ]; then
    cp "$HOME/dev/repos/zmk/app/build/left/zephyr/zmk.uf2" /media/*/XIAO-SENSE/
    echo "Done."
  elif [ "$2" = 'right' ]; then
    cp "$HOME/dev/repos/zmk/app/build/right/zephyr/zmk.uf2" /media/*/XIAO-SENSE/
    echo "Done."
  elif [ "$2" = 'reset' ]; then
    cp "$HOME/dev/repos/zmk/app/build/zephyr/*.uf2" /media/*/XIAO-SENSE/
    echo "Done."
  else
    echo "Don't know what to flash: Valid params are '-f left' or '-f right' or '-f reset'."
    exit 1
  fi
  exit
fi

config_dir="$(pwd)/config"
pushd "$HOME/dev/repos/zmk/app" || exit 1

if [ "$1" = '-r' ]; then
  echo "Building Settings Reset image ..."
  rm -rf "$HOME/dev/repos/zmk/app/build"
  west build -b seeeduino_xiao_ble -- -DSHIELD=settings_reset -DZMK_CONFIG="$config_dir"
  echo 'Done.'

else
  echo "Building left side ..."
  echo "----------------------"
  if west build -d build/left -b seeeduino_xiao_ble -- -DSHIELD=dactyl_manuform_left -DZMK_CONFIG="$config_dir"; then
    echo "Building right side ..."
    echo "-----------------------"
    west build -d build/right -b seeeduino_xiao_ble -- -DSHIELD=dactyl_manuform_right -DZMK_CONFIG="$config_dir"

  else
    echo "Not building right side due to previous errors."
  fi

fi

echo "Finished build."

popd || { echo 'Cannot return to previous directory'; exit 1; }
