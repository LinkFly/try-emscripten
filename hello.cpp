#include <iostream>
#include <math.h>
#include <emscripten.h>
#include <memory>
#include <functional>

using namespace std;

string bytes_to_string(uint8_t* bytes, size_t size) {
  string result;
  for(int i = 0; i < size; ++i) {
    result += static_cast<char>(bytes[i]);
  }
  return result;
}

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

  void register_event_handler(const string& event, const function<void(uint8_t*,size_t)>&) {
  
  }
  
  void recv_event(uint8_t* bytes, size_t size) {
    
  }
  
  EMSCRIPTEN_KEEPALIVE
  void send_event(uint8_t* event_bytes, size_t eventt_size, uint8_t* data_bytes, size_t data_size) {
    string event = bytes_to_string(event_bytes, eventt);
    string data = bytes_to_string(data_bytes, data_size);
    cout << "event: " << event << " data: " << data << endl;
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
      console.log('===============================');

      function bytes_alloc(str) {
	let data = [];
	for(let i = 0; i < str.length; ++i)
	  data[i] = str.charCodeAt(i);
	let offset = Module._malloc(str.length);
	Module.HEAP8.set(new Uint8Array(data), offset);
	return offset;
      }

      function bytes_dealloc(offset) {
	Module._free(offset);
      }

      let event = 'open';
      let data = 'project';
      let event_raw = bytes_alloc(event);
      let data_raw = bytes_alloc(data);
      Module._send_event(event_raw, event.length, data_raw, data.length);
      bytes_dealloc(event_raw);
      bytes_dealloc(data_raw);
      
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
