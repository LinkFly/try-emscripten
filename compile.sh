#./emcc activate latest
#sh ./emsdk_env.sh

emcc hello.cpp -o hello.html --js-library "mylib.js" -s EXPORTED_FUNCTIONS='["_main","_int_sqrt"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

