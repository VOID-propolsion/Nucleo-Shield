#!/bin/bash

echo "Flashing..."
openocd -f ./openocd.cfg -c "program ./build/Nucleo-Shield.elf verify reset exit"
