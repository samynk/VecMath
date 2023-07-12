rd /s /q windowsbuild
mkdir windowsbuild
cd windowsbuild
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cpack -C Release
