emcc interact.cpp -o interact.html --js-library "mylib.js" -s EXPORTED_FUNCTIONS='["_main", "_malloc", "_free"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap", "setValue", "getValue"]'
