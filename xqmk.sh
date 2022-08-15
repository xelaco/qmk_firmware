#!/bin/bash

function genererBDD () {
qmk generate-compilation-database
sed -i -e "s/-mcall-prologues //g" ../../../../../compile_commands.json
mv ../../../../../compile_commands.json .
}

cd keyboards/xelaco/pegasushoof/keymaps/xelaco
genererBDD

cd ../../../nanoslider/keymaps/xelaco/
genererBDD

cd ../../../niu_mini/keymaps/xelaco/
genererBDD

cd ../../../../..
vim -p keyboards/xelaco/pegasushoof/keymaps/xelaco/keymap.c keyboards/xelaco/nanoslider/keymaps/xelaco/keymap.c keyboards/xelaco/niu_mini/keymaps/xelaco/keymap.c
