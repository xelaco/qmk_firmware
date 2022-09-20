#!/bin/bash

QMKDIR=$(pwd)

function genererBDD () {
  qmk generate-compilation-database
  sed -i -e "s/-mcall-prologues //g" "$QMKDIR"/compile_commands.json
  mv "$QMKDIR"/compile_commands.json .
}

cd "$QMKDIR"/keyboards/xelaco/pegasushoof/keymaps/xelaco
genererBDD

cd "$QMKDIR"/keyboards/xelaco/nanoslider/keymaps/xelaco
genererBDD

cd "$QMKDIR"/keyboards/xelaco/niu_mini/keymaps/xelaco
genererBDD

cd "$QMKDIR"/keyboards/xelaco/lulu/rp2040/keymaps/xelaco
genererBDD

cd "$QMKDIR"
vim -p keyboards/xelaco/pegasushoof/keymaps/xelaco/keymap.c keyboards/xelaco/nanoslider/keymaps/xelaco/keymap.c keyboards/xelaco/niu_mini/keymaps/xelaco/keymap.c keyboards/xelaco/lulu/rp2040/keymaps/xelaco/keymap.c
