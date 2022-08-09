#!/bin/bash
cd keyboards/xelaco/pegasushoof/keymaps/xelaco
qmk generate-compilation-database
mv ../../../../../compile_commands.json .
cd ../../../nanoslider/keymaps/xelaco/
qmk generate-compilation-database
mv ../../../../../compile_commands.json .
cd ../../../niu_mini/keymaps/xelaco/
qmk generate-compilation-database
mv ../../../../../compile_commands.json .
cd ../../../../..
vim -p keyboards/xelaco/pegasushoof/keymaps/xelaco/keymap.c keyboards/xelaco/nanoslider/keymaps/xelaco/keymap.c keyboards/xelaco/niu_mini/keymaps/xelaco/keymap.c
