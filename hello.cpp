#include <iostream>
#include <math.h>
#include <emscripten.h>

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

} // extern "C"

// don't work
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
