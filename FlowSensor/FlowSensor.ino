// FlowSensor
//
// Use Arduino Adafruit "Trinket" board with HL30 flow sensor and "Neopixel" display for
// monitoring water flow for laser cutter cooling
//
// NOTES
//
// Trinket pin allocation:
// - https://learn.adafruit.com/introducing-trinket/pinouts
//
// Pin0 - pulse counter (pulse input from flow meter)
// Pin1 - neopixel output: (string of addrssable LEDs - use for readout gauge)
// Pin2 - flow OK output: high output if flow OK (signal to laser cutter)
//
// Arduino IDE:
// - https://www.arduino.cc/en/Main/Software
//
// Board support: 
// - https://adafruit.github.io/arduino-board-index/package_adafruit_index.json (configure through IDE)
//
// Flow sensor: 
// - https://www.seeedstudio.com/G1%26amp%3B2%26quot%3B-Water-Flow-Sensor-p-635.html
// - http://wiki.seeedstudio.com/wiki/G1/2_Water_Flow_sensor
//
// Libraries: 
// - neopixel - https://github.com/adafruit/Adafruit_NeoPixel
// - instructions - https://learn.adafruit.com/trinket-neopixel-led-longboard/software, etc

// Initial code copied from Adafruit example

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DEBUG 1

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
#define NEOPIXEL_PIN 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Initialize

int wait_time = 1000;
int flow_rate = 0;

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
      if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    // End of trinket special code
    strip.begin();
    strip.setBrightness(25); // 50 = 1/3 brightness
    strip.show(); // Initialize all pixels to 'off'
}

// Main control loop

void loop() {
    flow_rate = read_flow_rate();
    // set_flow_monitor_output(flow_rate);
    set_flow_rate_display(flow_rate);
    delay(wait_time);
}

// Read flow rate sensor
int read_flow_rate() {
    // @@TODO: put real code here - see 'FlowSensor.ino' example code
    flow_rate++;
    if (flow_rate > 12) {
      flow_rate = 0;
    }
    return flow_rate;
}

// Set flow rate monitor output signal
void set_flow_monitor_output(int flow_rate) {
    // @@TODO: put real code here
    if (flow_rate >= 6) {
        // set output pin 2 high
    } else {
        // set output pin 2 low
    }
}

// Set flow rate display
void set_flow_rate_display(int flow_rate) {
    // Reduce flow rate to range 0..12
    int fr_0_12 = flow_rate;
    // Choose colour
    int colour_max = 127;
    int colour_mid = 63;
    int colour_low = 31;
    uint32_t colour_map[] = {
        strip.Color(colour_mid, 0, colour_mid), // 00 violet
        strip.Color(colour_mid, 0, colour_mid), // 01 violet 
        strip.Color(colour_mid, 0, colour_mid), // 02 violet
        strip.Color(0, 0, colour_max),          // 03 blue
        strip.Color(0, 0, colour_max),          // 04 blue
        strip.Color(0, colour_max, 0),          // 05 green
        strip.Color(0, colour_max, 0),          // 06 green
        strip.Color(colour_mid, colour_low, 0), // 07 yellow
        strip.Color(colour_mid, colour_low, 0), // 08 yellow
        strip.Color(colour_mid, colour_mid, 0), // 09 orange
        strip.Color(colour_mid, colour_mid, 0), // 10 orange
        strip.Color(colour_max, 0, 0),          // 11 red
        strip.Color(colour_max, 0, 0),          // 12 red
    };
    uint32_t c = colour_map[fr_0_12];
    // Set pixels
    for ( uint16_t i=0 ; i < 12 ; i++ ) {
        if (i >= fr_0_12) {
            c = strip.Color(0, 0, 0);
        }
        strip.setPixelColor(i, c);
    }
    strip.show();
}



