#!/bin/bash

cd ${0%/*}
cd ..

# Wait for login to complete.
sleep 10
./src/display-events-sdl
