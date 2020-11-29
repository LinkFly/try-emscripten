#include <iostream>
#include <math.h>
#include <emscripten.h>
#include <memory>

using namespace std;

extern "C" {

extern void my_js(void);

int int_sqrt(int x) {
  return sqrt(x);
}

EMSCRIPTEN_KEEPALIVE
void sayHi() {
  cout << "Hi!" << endl;
}

EMSCRIPTEN_KEEPALIVE
int daysInWeek() {
  return 7;
}

  EMSCRIPTEN_KEEPALIVE
  void modify_array(uint8_t* ar, size_t size) {
    for(size_t i = 0; i < size; ++i) {
      cout << (int)ar[i] << " ";
      ar[i] += 10;
    }
    cout << endl;
  }

  static uint8_t inmem[] = {5, 6, 7};
  
  EMSCRIPTEN_KEEPALIVE
  void* get_inmem() {
    uint8_t* pinmem = &inmem[0];
    return pinmem;
  }

  EMSCRIPTEN_KEEPALIVE
  void print_inmem() {
    for(int i = 0; i < 3; ++i)
      cout << (int)inmem[i] << " ";
    cout << endl;
  }

} // extern "C"

void my_calljs() {
  emscripten_run_script("console.log('test-test');");
}

EM_JS(void, call_alert, (), {
    alert('hello world!');
    throw 'all done';
});

int main() {
  cout << "Hello!" << endl;
  my_calljs();
  
  EM_ASM({
      console.log('test-2 Value: ' + $0);
      var offset = Module._malloc(3);
      Module.HEAP8.set(new Uint8Array([1,2,3]), offset);
      Module._modify_array(offset, 3);
      var result = [];
      result[0] = Module.getValue(offset);
      result[1] = Module.getValue(offset + 1);
      result[2] = Module.getValue(offset + 2);
      console.log("modified array: ", result);
      Module._free(offset);

      var offset2 = Module._get_inmem();
      for(let i = 0; i < 3; ++i) {
	let curval = Module.getValue(offset2 + i);
	Module.setValue(offset2 + i, curval + 20);
      }

      Module._print_inmem();
	 //throw 'all done2';
    }, 100);
  int x = EM_ASM_INT({
      return $0 + 1;
    }, 100);
  cout << "Value2: " << x << endl;
  //call_alert();
  //////////////

  my_js();
}
