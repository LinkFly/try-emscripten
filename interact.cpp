#include <iostream>
#include <math.h>
#include <memory>
#include <functional>
#include <map>
#include <vector>
#include <set>
#include <cassert>

#include <emscripten.h>

using namespace std;

// Utils
string bytes_to_string(uint8_t* bytes, size_t size) {
  string result;
  for(int i = 0; i < size; ++i) {
    result += static_cast<char>(bytes[i]);
  }
  return result;
}
////

class Events {
public:
  void addEvents(const vector<string>& events) {
    for(auto it = events.begin(); it != events.end(); ++it) {
      auto itFind = events_handlers.find(*it);
      if (itFind == events_handlers.end())
	events_handlers[*it] = (function<void(const string&)>)(nullptr);
    }
  }
  
  void register_callback(const string& event, const function<void(const string&)>& callback) {
    auto it = events_handlers.find(event);
     assert(it != events_handlers.end());
    events_handlers[event] = callback;
  }

  void send_event(uint8_t* event_bytes, size_t event_size, uint8_t* data_bytes, size_t data_size) {
    string event = bytes_to_string(event_bytes, event_size);
    string data = bytes_to_string(data_bytes, data_size);
    cout << "event: " << event << endl;
    events_handlers[event](data);
  }

private:
  map<string, function<void(const string&)>> events_handlers;
};

static Events events;

void init(const vector<string>& events_names) {
  events.addEvents(events_names);
}

void register_event_handler(const string& event, const function<void(const string&)>& callback) {
  events.register_callback(event, callback);
}

extern "C" {  
  EMSCRIPTEN_KEEPALIVE
  void send_event(uint8_t* event_bytes, size_t event_size, uint8_t* data_bytes, size_t data_size) {
    events.send_event(event_bytes, event_size, data_bytes, data_size);
  }
}

int main() {
  init({"open", "close"});
  register_event_handler("open",
			 [](const string& data) {
			   cout << "data: " << data << endl;
			 });
  
  EM_ASM(
      // helpers
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
      /////
      
      let event = 'open';
      let data = 'project';
      let event_raw = bytes_alloc(event);
      let data_raw = bytes_alloc(data);
      Module._send_event(event_raw, event.length, data_raw, data.length);
      bytes_dealloc(event_raw);
      bytes_dealloc(data_raw);
  );
  return 0;
}
