emcc hello.cpp -o hello.html --js-library "mylib.js" -s EXPORTED_FUNCTIONS='["_main","_int_sqrt", "_malloc", "_free"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap", "setValue", "getValue"]'
