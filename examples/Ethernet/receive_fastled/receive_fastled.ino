#include <Artnet.h>
#include <FastLED.h>

// Ethernet stuff
const IPAddress ip(192, 168, 0, 201);
uint8_t mac[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};

ArtnetReceiver artnet;
uint32_t universe = 1;

// FastLED
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];
const uint8_t PIN_LED_DATA = 3;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    FastLED.addLeds<NEOPIXEL, PIN_LED_DATA>(leds, NUM_LEDS);

    Ethernet.begin(mac, ip);
    artnet.begin();

    // if Artnet packet comes to this universe, this function is called
    artnet.subscribe(universe, [&](uint8_t* data, uint16_t size)
    {
        // set led
        // artnet data size per packet is 512 max
        // so there is max 170 pixel per packet (per universe)
        for (size_t pixel = 0; pixel < NUM_LEDS; ++pixel)
        {
            size_t idx = pixel * 3;
            leds[pixel].r = data[idx + 0];
            leds[pixel].g = data[idx + 1];
            leds[pixel].b = data[idx + 2];
        }
        FastLED.show();
    });
}

void loop()
{
    artnet.parse(); // check if artnet packet has come and execute callback
}
