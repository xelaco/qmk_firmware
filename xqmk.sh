#!/bin/bash
qmk generate-compilation-database -kb xelaco/pegasushoof -km xelaco
qmk generate-compilation-database -kb xelaco/nanoslider -km xelaco
qmk generate-compilation-database -kb xelaco/niu_mini -km xelaco
vim -S qmk.vim
