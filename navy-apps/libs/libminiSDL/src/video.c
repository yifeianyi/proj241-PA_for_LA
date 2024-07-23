#include <NDL.h>
#include <sdl-video.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
    assert(dst && src);
    assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);

    uint32_t source_height = src->h;
    uint32_t source_width = src->w;
    uint32_t copy_height = source_height;
    uint32_t copy_width = source_width;
    uint32_t source_start_offset = 0;
    uint32_t destination_start_offset = 0;
    uint32_t bytes_per_pixel = dst->format->BytesPerPixel;
    uint32_t destination_pitch = dst->pitch;
    uint32_t source_pitch = src->pitch;

// Adjust the copy area and start offset if a source rectangle is specified
  if (srcrect != NULL) {
      copy_height = srcrect->h;
      copy_width = srcrect->w;
      source_start_offset = srcrect->y * source_width + srcrect->x;
  }

  // Adjust the destination start offset if a destination rectangle is specified
  if (dstrect != NULL) {
      destination_start_offset = dstrect->y * destination_pitch / bytes_per_pixel + dstrect->x;
  }

// Perform the blit operation
  for (uint32_t row = 0; row < copy_height; ++row) {
      void *destination = (uint8_t *)dst->pixels + (destination_start_offset + row * (destination_pitch / bytes_per_pixel)) * bytes_per_pixel;
      void *source = (uint8_t *)src->pixels + (source_start_offset + row * source_width) * bytes_per_pixel;
      memcpy(destination, source, copy_width * bytes_per_pixel);
  }
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
  int32_t start_offset = 0; // Initial offset in the pixel buffer
  uint32_t surface_height = dst->h;
  uint32_t surface_width = dst->w;
  uint32_t rect_height = surface_height; // The height of the rectangle area to fill
  uint32_t rect_width = surface_width;  // The width of the rectangle area to fill

  if (dstrect != NULL) {
    // If the destination rectangle is specified, update the offset and dimensions
    start_offset = dstrect->y * surface_width + dstrect->x;
    rect_height = dstrect->h;
    rect_width = dstrect->w;
  }

  int bytes_per_pixel = dst->format->BytesPerPixel; // Calculate bytes per pixel based on the format

// Fill the specified rectangle with the color
  for (uint32_t row = 0; row < rect_height; ++row) {
    void *pixel_row_start = (uint8_t *)dst->pixels + (start_offset + row * surface_width) * bytes_per_pixel;
    memset(pixel_row_start, color, rect_width * bytes_per_pixel);
  }
}

void SDL_UpdateRect(SDL_Surface *s, int x, int y, int w, int h) {
  if (s == NULL || s->pixels == NULL) {
        // Handle null pointer
        return;
    }

    if (w == 0 || h == 0) {
        w = s->w;
        h = s->h;
    }

    uint32_t len = (uint32_t)w * (uint32_t)h;
    uint32_t *buf = (uint32_t *)malloc(len * sizeof(uint32_t));

    if (buf == NULL) {
        // Handle memory allocation failure
        return;
    }
    
    uint8_t *pixels = (uint8_t *)s->pixels;
    uint8_t bytes_per_pixel = s->format->BytesPerPixel;
    uint32_t pitch = s->pitch;

    for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
            int index = row * w + col;  // index in the buffer
            int pixel_pos = (y + row) * (pitch / bytes_per_pixel) + (x + col);  // pixel position in the surface

            if (s->format->BitsPerPixel == 32) {
                uint32_t *pixels_32 = (uint32_t *)pixels;
                buf[index] = pixels_32[pixel_pos];
            }
            else if (s->format->BitsPerPixel == 8) {
                uint8_t index_in_palette = pixels[pixel_pos];
                SDL_Color rgba_color = s->format->palette->colors[index_in_palette];
                buf[index] = (rgba_color.a << 24) | (rgba_color.r << 16) | (rgba_color.g << 8) | (rgba_color.b); 
            }
            else {
                // Unsupported pixel format
                free(buf);
                fprintf(stderr, "SDL_UpdateRect: Unsupported pixel format with %d bits per pixel\n", s->format->BitsPerPixel);
                return;
            }
        }
    }

    NDL_DrawRect(buf, x, y, w, h);

    free(buf);
}

// APIs below are already implemented.

static inline int maskToShift(uint32_t mask) {
  switch (mask) {
    case 0x000000ff: return 0;
    case 0x0000ff00: return 8;
    case 0x00ff0000: return 16;
    case 0xff000000: return 24;
    case 0x00000000: return 24; // hack
    default: assert(0);
  }
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
    uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
  assert(depth == 8 || depth == 32);
  SDL_Surface *s = malloc(sizeof(SDL_Surface));
  assert(s);
  s->flags = flags;
  s->format = malloc(sizeof(SDL_PixelFormat));
  assert(s->format);
  if (depth == 8) {
    s->format->palette = malloc(sizeof(SDL_Palette));
    assert(s->format->palette);
    s->format->palette->colors = malloc(sizeof(SDL_Color) * 256);
    assert(s->format->palette->colors);
    memset(s->format->palette->colors, 0, sizeof(SDL_Color) * 256);
    s->format->palette->ncolors = 256;
  } else {
    s->format->palette = NULL;
    s->format->Rmask = Rmask; s->format->Rshift = maskToShift(Rmask); s->format->Rloss = 0;
    s->format->Gmask = Gmask; s->format->Gshift = maskToShift(Gmask); s->format->Gloss = 0;
    s->format->Bmask = Bmask; s->format->Bshift = maskToShift(Bmask); s->format->Bloss = 0;
    s->format->Amask = Amask; s->format->Ashift = maskToShift(Amask); s->format->Aloss = 0;
  }

  s->format->BitsPerPixel = depth;
  s->format->BytesPerPixel = depth / 8;

  s->w = width;
  s->h = height;
  s->pitch = width * depth / 8;
  assert(s->pitch == width * s->format->BytesPerPixel);

  if (!(flags & SDL_PREALLOC)) {
    s->pixels = malloc(s->pitch * height);
    assert(s->pixels);
  }

  return s;
}

SDL_Surface* SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth,
    int pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
  SDL_Surface *s = SDL_CreateRGBSurface(SDL_PREALLOC, width, height, depth,
      Rmask, Gmask, Bmask, Amask);
  assert(pitch == s->pitch);
  s->pixels = pixels;
  return s;
}

void SDL_FreeSurface(SDL_Surface *s) {
  if (s != NULL) {
    if (s->format != NULL) {
      if (s->format->palette != NULL) {
        if (s->format->palette->colors != NULL) free(s->format->palette->colors);
        free(s->format->palette);
      }
      free(s->format);
    }
    if (s->pixels != NULL && !(s->flags & SDL_PREALLOC)) free(s->pixels);
    free(s);
  }
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
  if (flags & SDL_HWSURFACE) NDL_OpenCanvas(&width, &height);
  return SDL_CreateRGBSurface(flags, width, height, bpp,
      DEFAULT_RMASK, DEFAULT_GMASK, DEFAULT_BMASK, DEFAULT_AMASK);
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
  assert(src && dst);
  assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
  assert(dst->format->BitsPerPixel == 8);

  int x = (srcrect == NULL ? 0 : srcrect->x);
  int y = (srcrect == NULL ? 0 : srcrect->y);
  int w = (srcrect == NULL ? src->w : srcrect->w);
  int h = (srcrect == NULL ? src->h : srcrect->h);

  assert(dstrect);
  if(w == dstrect->w && h == dstrect->h) {
    /* The source rectangle and the destination rectangle
     * are of the same size. If that is the case, there
     * is no need to stretch, just copy. */
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_BlitSurface(src, &rect, dst, dstrect);
  }
  else {
    assert(0);
  }
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, int firstcolor, int ncolors) {
  assert(s);
  assert(s->format);
  assert(s->format->palette);
  assert(firstcolor == 0);

  s->format->palette->ncolors = ncolors;
  memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

  if(s->flags & SDL_HWSURFACE) {
    assert(ncolors == 256);
    for (int i = 0; i < ncolors; i ++) {
      uint8_t r = colors[i].r;
      uint8_t g = colors[i].g;
      uint8_t b = colors[i].b;
    }
    SDL_UpdateRect(s, 0, 0, 0, 0);
  }
}

static void ConvertPixelsARGB_ABGR(void *dst, void *src, int len) {
  int i;
  uint8_t (*pdst)[4] = dst;
  uint8_t (*psrc)[4] = src;
  union {
    uint8_t val8[4];
    uint32_t val32;
  } tmp;
  int first = len & ~0xf;
  for (i = 0; i < first; i += 16) {
#define macro(i) \
    tmp.val32 = *((uint32_t *)psrc[i]); \
    *((uint32_t *)pdst[i]) = tmp.val32; \
    pdst[i][0] = tmp.val8[2]; \
    pdst[i][2] = tmp.val8[0];

    macro(i + 0); macro(i + 1); macro(i + 2); macro(i + 3);
    macro(i + 4); macro(i + 5); macro(i + 6); macro(i + 7);
    macro(i + 8); macro(i + 9); macro(i +10); macro(i +11);
    macro(i +12); macro(i +13); macro(i +14); macro(i +15);
  }

  for (; i < len; i ++) {
    macro(i);
  }
}

SDL_Surface *SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, uint32_t flags) {
  assert(src->format->BitsPerPixel == 32);
  assert(src->w * src->format->BytesPerPixel == src->pitch);
  assert(src->format->BitsPerPixel == fmt->BitsPerPixel);

  SDL_Surface* ret = SDL_CreateRGBSurface(flags, src->w, src->h, fmt->BitsPerPixel,
    fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

  assert(fmt->Gmask == src->format->Gmask);
  assert(fmt->Amask == 0 || src->format->Amask == 0 || (fmt->Amask == src->format->Amask));
  ConvertPixelsARGB_ABGR(ret->pixels, src->pixels, src->w * src->h);

  return ret;
}

uint32_t SDL_MapRGBA(SDL_PixelFormat *fmt, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  assert(fmt->BytesPerPixel == 4);
  uint32_t p = (r << fmt->Rshift) | (g << fmt->Gshift) | (b << fmt->Bshift);
  if (fmt->Amask) p |= (a << fmt->Ashift);
  return p;
}

int SDL_LockSurface(SDL_Surface *s) {
  return 0;
}

void SDL_UnlockSurface(SDL_Surface *s) {
}
