# Changelog

## FIX: USB Boot for Bitlocker

14.11.2026

Former approached didn't work out.
The settings were in `config/boards/shields/dactyl_manuform/dactyl_manuform.conf`. Inside a shield folder, Zephyr only loads a `.conf` whose name exactly matches the shield being built, so `dactyl_manuform_left.conf` or `dactyl_manuform_right.conf`. ZMK only looks for the shared `dactyl_manuform.conf` at the top of `config/`.
Moved the config file up to `config/`.
