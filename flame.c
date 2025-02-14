// A program to recreate the fire animation used on Doom.
// Code adapted from: https://github.com/Ponup/doom-fire/
// which, in turn, was based on this article: https://fabiensanglard.net/doom_fire_psx/index.html

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// each element in the palette is a 32-bits integer encoding RGBA (where Alpha is the MSB)
static const uint32_t palette[] = {
  0xFF070707,
  0xFF07071F,
  0xFF070F2F,
  0xFF070F47,
  0xFF071757,
  0xFF071F67,
  0xFF071F77,
  0xFF07278F,
  0xFF072F9F,
  0xFF073FAF,
  0xFF0747BF,
  0xFF0747C7,
  0xFF074FDF,
  0xFF0757DF,
  0xFF0757DF,
  0xFF075FD7,
  0xFF075FD7,
  0xFF0F67D7,
  0xFF0F6FCF,
  0xFF0F77CF,
  0xFF0F7FCF,
  0xFF1787CF,
  0xFF1787C7,
  0xFF178FC7,
  0xFF1F97C7,
  0xFF1F9FBF,
  0xFF1F9FBF,
  0xFF27A7BF,
  0xFF27A7BF,
  0xFF2FAFBF,
  0xFF2FAFB7,
  0xFF2FB7B7,
  0xFF37B7B7,
  0xFF6FCFCF,
  0xFF9FDFDF,
  0xFFC7EFEF,
  0xFFFFFFFF,
};

#define image_width 320
#define image_height 240

static const int start_last_row = (image_width * (image_height - 1));
static const int upper = 3;
static const int lower = -3;
static const int height_max = 3;

// paletted pixel data (used for calculating the animation)
static uint8_t pixels[image_width * image_height];

// RGBA pixel data (used for drawing)
static uint32_t buffer[image_width * image_height];

// this function is called once before the animation starts
void init(void) {
  // set all pixels to black
  memset(pixels, 0, image_width * image_height);
  for (int row = 0, index = 0; row < image_height; row++) {
    for (int column = 0; column < image_width; column++, index++) {
      buffer[index] = palette[0];
    }
  }

  // draw the "igniter" and the bottom of the image
  for (int w = 0; w < image_width; w++) {
    pixels[start_last_row + w] = 36;
    buffer[start_last_row + w] = palette[36];
  }
}

// this function is called periodically
void *anim(void) {
  // animate the flames
  for (int row = image_height - 1 - 1; row > 0; row--) {
    for (int column = 0; column < image_width; column++) {
      int current_row = (row * image_width);
      int next_row = ((row + 1) * image_width);
      int target_pixel = current_row + column;
      int decay = (rand() % height_max) & 1;
      int movement = (rand() % (upper - lower + 1)) + lower;
      int from_pixel = next_row + column + movement;

      if (from_pixel >= 0 && from_pixel < image_width * image_height) {
        int new_pixel = (pixels[from_pixel]) - decay;
        if (new_pixel >= 0 && new_pixel <= 36) {
          pixels[target_pixel] = new_pixel;
          buffer[target_pixel] = palette[new_pixel];
        }
      }
    }
  }

  // the image buffer to be used by the JavaScript side
  return buffer;
}
