#!/bin/sh

export HSMGEN_DIR=./thirdparty/hsmcpp/tools/scxml2gen

python3 $HSMGEN_DIR/scxml2gen.py -code -s ./smart_curtains.scxml -c SmartCurtainsHsm -thpp $HSMGEN_DIR/template.hpp -tcpp $HSMGEN_DIR/template.cpp -d ./src/hsm/