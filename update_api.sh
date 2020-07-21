#!/bin/bash

~/src/godot/bin/godot.x11.tools.64.llvm --gdnative-generate-json-api api.json

cd godot-cpp

scons platform=linux generate_bindings=yes target=debug use_llvm=yes use_custom_api_file=yes custom_api_file=../api.json bits=64 -j4
scons platform=linux generate_bindings=yes target=release use_llvm=yes use_custom_api_file=yes custom_api_file=../api.json bits=64 -j4

cd ..
