#include "Sprite16.h"

Sprite16::Sprite16(uint8_t width, uint8_t height, uint16_t data, ...)
  :_width(0)
  ,_height(0)
  ,_buffer(nullptr)
{
  // create everything
  init(width, height);

  // read in the data
  va_list ap;
  va_start(ap, data);
  for (uint8_t row = 0; ; row++)
  {
    _buffer[row] = data;

    if (row == _height - 1)
    {
      break;
    }

#if defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_UNOR4_MINIMA)
    // NOTE: Uno R4's integer is 32-bit long; reading va_arg as 16-bit is an undefined behaviour,
    //       and in case of Arduino, this will cause a CRASH!
    data = static_cast<uint16_t>(va_arg(ap, uint32_t));
#else
    data = static_cast<uint16_t>(va_arg(ap, uint16_t));
#endif
  }
  va_end(ap);
}

void Sprite16::init(const uint8_t width, const uint8_t height)
{
  // store height and width
  _width  = min(width, 16);
  _height = min(height, 8);

  // create the buffer
  _buffer = static_cast<uint16_t *>(calloc(_height, sizeof(uint16_t)));
  if (_buffer == nullptr) {
    Serial.begin(9600);
    Serial.println("Failed to allocate memory for sprite buffer");
    Serial.flush();
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
}

uint8_t Sprite16::height() const {
  return _height;
}

uint8_t Sprite16::width() const {
  return _width;
}

uint16_t Sprite16::readRow(uint8_t row) const {
  if (row > _height)
  {
    return 0;
  }

  return _buffer[row];
}
