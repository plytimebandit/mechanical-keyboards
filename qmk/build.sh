#!/usr/bin/env bash

# https://docs.qmk.fm/#/

log_build_info() {
	output_file="$(dirname "$(readlink build.sh)")/build.info"
	git_changes="$(git status --porcelain | wc -l)"

	echo "Build:             $(date '+%Y-%m-%d %H:%m:%S')" > "$output_file"
	echo "Build version:     $(./build.sh -v)" >> "$output_file"
	echo "Last git commit:   $(git rev-parse HEAD)" >> "$output_file"
	echo "Outgoing changes:  $(if [ $git_changes -eq 0 ]; then echo no; else echo yes; fi)" >> "$output_file"
}

if [ -z "$1" ]; then
	echo "Arguments:"
	echo "  -c      compile"
	echo "  -f      flash"
	echo "  -f2     flash twice to flash both sides without interaction at the terminal"
	echo "  -r      reset eeprom"
	echo "  -r      [device] [avrdude.conf]: reset eeprom"
	echo "  -j      c2json"
	echo "  -v      print quantum version"
	echo "  -i      some information"
	echo "  --cln   clean up QMK folder"
	exit 1
fi

qmk_kb_params='-kb handwired/dactyl_manuform/5x6 -km honey-milk-de'

if [ "$1" == '-c' ]; then
	qmk compile ${qmk_kb_params}

elif [ "$1" == '-f' ]; then
	qmk flash ${qmk_kb_params}
	log_build_info

elif [ "$1" == '-f2' ]; then
	qmk flash ${qmk_kb_params} && qmk flash ${qmk_kb_params}
	log_build_info

elif [ "$1" == '-r' ]; then
	if [ -n "$2" ] && [ -n "$3" ]; then
		./reset.sh "$2" "$3"
	else
		./reset.sh
	fi

elif [ "$1" == '-j' ]; then
	qmk -v c2json -o honey-milk-de_keymap.json ${qmk_kb_params} keyboards/handwired/dactyl_manuform/5x6/keymaps/honey-milk-de/keymap.c

elif [ "$1" == '-v' ]; then
	if [ ! -d .build ]; then
		# This comes from the Makefile
		git describe --abbrev=0 --tags
	else
		find .build -name 'version.h' -exec cat {} \; | grep QMK_VERSION | sed -En 's/.*"(.+)"/\1/p'
	fi

elif [ "$1" == '-i' ]; then
	echo "Some versions of Quantum have chattering issues. QMK Firmware 0.13.34 works though."

elif [ "$1" == '--cln' ]; then
    qmk clean -a

else
	echo "Error: Argument unknown. Run without arguments for help."
	exit 1
fi

exit 0
