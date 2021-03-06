'use strict';

// Sometimes we need to test that using either the name, alias, or UUID
// produces the same result. The following objects help us do that.
var generic_access = {
  alias: 0x1800,
  name: 'generic_access',
  uuid: '00001800-0000-1000-8000-00805f9b34fb'
};
var device_name = {
  alias: 0x2a00,
  name: 'gap.device_name',
  uuid: '00002a00-0000-1000-8000-00805f9b34fb'
};
var reconnection_address = {
  alias: 0x2a03,
  name: 'gap.reconnection_address',
  uuid: '00002a03-0000-1000-8000-00805f9b34fb'
};
var heart_rate = {
  alias: 0x180d,
  name: 'heart_rate',
  uuid: '0000180d-0000-1000-8000-00805f9b34fb'
};
var glucose = {
  alias: 0x1808,
  name: 'glucose',
  uuid: '00001808-0000-1000-8000-00805f9b34fb'
};
var battery_service = {
  alias: 0x180f,
  name: 'battery_service',
  uuid: '0000180f-0000-1000-8000-00805f9b34fb'
};
var battery_level = {
  alias: 0x2A19,
  name: 'battery_level',
  uuid: '00002a19-0000-1000-8000-00805f9b34fb'
};

// TODO(jyasskin): Upstream this to testharness.js: https://crbug.com/509058.
function callWithKeyDown(functionCalledOnKeyPress) {
  return new Promise(resolve => {
    function onKeyPress() {
      document.removeEventListener('keypress', onKeyPress, false);
      resolve(functionCalledOnKeyPress());
    }
    document.addEventListener('keypress', onKeyPress, false);

    eventSender.keyDown(' ', []);
  });
}

// Calls requestDevice() in a context that's 'allowed to show a popup'.
function requestDeviceWithKeyDown() {
  let args = arguments;
  return callWithKeyDown(() => navigator.bluetooth.requestDevice.apply(navigator.bluetooth, args));
}

// Calls testRunner.getBluetoothManualChooserEvents() until it's returned
// |expected_count| events. Or just once if |expected_count| is undefined.
function getBluetoothManualChooserEvents(expected_count) {
  return new Promise((resolve, reject) => {
    let events = [];
    let accumulate_events = new_events => {
      events.push(...new_events);
      if (events.length >= expected_count) {
        resolve(events);
      } else {
        testRunner.getBluetoothManualChooserEvents(accumulate_events);
      }
    };
    testRunner.getBluetoothManualChooserEvents(accumulate_events);
  });
}

// errorUUID(alias) returns a UUID with the top 32 bits of
// '00000000-97e5-4cd7-b9f1-f5a427670c59' replaced with the bits of |alias|.
// For example, errorUUID(0xDEADBEEF) returns
// 'deadbeef-97e5-4cd7-b9f1-f5a427670c59'. The bottom 96 bits of error UUIDs
// were generated as a type 4 (random) UUID.
function errorUUID(uuidAlias) {
  // Make the number positive.
  uuidAlias >>>= 0;
  // Append the alias as a hex number.
  var strAlias = '0000000' + uuidAlias.toString(16);
  // Get last 8 digits of strAlias.
  strAlias = strAlias.substr(-8);
  // Append Base Error UUID
  return strAlias + '-97e5-4cd7-b9f1-f5a427670c59';
}

// Function to test that a promise rejects with the expected error type and
// message.
function assert_promise_rejects_with_message(promise, expected, description) {
  return promise.then(() => {
    assert_unreached('Promise should have rejected: ' + description);
  }, error => {
    assert_equals(error.name, expected.name, 'Unexpected Error Name:');
    if (expected.message) {
      assert_equals(error.message, expected.message, 'Unexpected Error Message:');
    }
  });
}

// Parses add-device(name)=id lines in
// testRunner.getBluetoothManualChooserEvents() output, and exposes the name->id
// mapping.
class AddDeviceEventSet {
  constructor() {
    this._idsByName = new Map();
    this._addDeviceRegex = /^add-device\(([^)]+)\)=(.+)$/;
  }
  assert_add_device_event(event, description) {
    let match = this._addDeviceRegex.exec(event);
    assert_true(!!match, event + "isn't an add-device event: " + description);
    this._idsByName.set(match[1], match[2]);
  }
  has(name) {
    return this._idsByName.has(name);
  }
  get(name) {
    return this._idsByName.get(name);
  }
}

function runGarbageCollection()
{
  // Run gc() as a promise.
  return new Promise(
      function(resolve, reject) {
        GCController.collect();
        setTimeout(resolve, 0);
      });
}

// Creates |num_listeners| promises. Each adds an event listener
// to object. The promises resolve once the object fires |event| but
// reject if the event is fired before |object|.|func|() resolves.
// Returns a promise that fulfills with the result of |object|.|func()|
// and |event.target.value| of each of the other promises.
function assert_event_fires_after_promise(object, func, event, num_listeners) {
  num_listeners = num_listeners !== undefined ? num_listeners : 1;

  if (object[func] === undefined) {
    return Promise.reject('Function \'' + func + '\' not available in object.');
  }
  let should_resolve = false;
  let event_promises = [];
  for (let i = 0; i < num_listeners; i++) {
    event_promises.push(new Promise((resolve, reject) => {
      let event_listener = (e) => {
        object.removeEventListener(event, event_listener);
        if (should_resolve) {
          resolve(e.target.value);
        } else {
          reject(event + ' was triggered before the promise resolved.');
        }
      };
      object.addEventListener(event, event_listener);
    }));
  }
  return object[func]().then(result => {
    should_resolve = true;
    return Promise.all([result, ...event_promises]);
  });
}

// Returns a promise that resolves after 100ms unless
// the the event is fired on the object in which case
// the promise rejects.
function assert_no_events(object, event_name) {
  return new Promise((resolve, reject) => {
    let event_listener = (e) => {
      object.removeEventListener(event_name, event_listener);
      assert_unreached('Object should not fire an event.');
    };
    object.addEventListener(event_name, event_listener);
    // TODO(ortuno): Remove timeout.
    // http://crbug.com/543884
    setTimeout(() => {
      object.removeEventListener(event_name, event_listener);
      resolve();
    }, 100);
  });
}

class TestCharacteristicProperties {
  constructor(properties) {
    this.broadcast = properties.broadcast || false;
    this.read = properties.read || false;
    this.writeWithoutResponse = properties.writeWithoutResponse || false;
    this.write = properties.write || false;
    this.notify = properties.notify || false;
    this.indicate = properties.indicate || false;
    this.authenticatedSignedWrites = properties.authenticatedSignedWrites || false;
    this.reliableWrite = properties.reliableWrite || false;
    this.writableAuxiliaries = properties.writableAuxiliaries || false;
  }
}

function assert_properties_equal(properties, expected_properties) {
  for (let key in expected_properties) {
    assert_equals(properties[key], expected_properties[key]);
  }
}
