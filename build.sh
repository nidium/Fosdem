#!/bin/sh
git clone http://www.github.com/nidum/Nidium/
git clone http://www.github.com/nidum/NidiumTools/
PYTHONPATH=NidiumTools/src/
NIDIUM_PATH=Nidium/
cd Nidium
./configure_libnidiumcore
cd ..
clang++ VanillaJS/main.cpp -o VanillaRobot \
    -I$NIDIUM_PATH/third-party/mozilla-central/js/src/obj/dist/include/ \
    -include $NIDIUM_PATH/build/third-party/js-config.h \
    -L$NIDIUM_PATH/build/third-party/ \
    -std=c++11 \
    -ljs_static -lmozglue -lnspr4 -lz 

clang++ IDL/main.cpp IDL/JSRobot.cpp IDL/JSRobotBase.cpp -O IDLRobot \
    -I$NIDIUM_PATH/src/ \
    -I$NIDIUM_PATH/src/libapenetwork/src/ \
    -I$NIDIUM_PATH/third-party/mozilla-central/js/src/obj/dist/include/ \
    -L$NIDIUM_PATH/build/third-party/\
    -L$NIDIUM_PATH/gyp/build/Release/ \
    -L$NIDIUM_PATH/src/libapenetwork/gyp/build/Release/ \
    -include $NIDIUM_PATH/build/third-party/js-config.h \
    -std=c++11 \
    -lnidiumcore -lnetwork -ljs_static -lmozglue -lnspr4 -lcares -lhttp_parser -lssl -lcrypto -lzip -lm -lpthread -lleveldb -lz
