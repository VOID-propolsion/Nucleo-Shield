#!/bin/bash

echo "Flashing..."
openocd -f ./openocd.cfg -c "program ../../build/Astrea.elf verify reset exit"
