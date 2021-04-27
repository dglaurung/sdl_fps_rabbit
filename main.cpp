#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <deque>
#include "FpsCounter.h"


#define WIDTH 800
#define HEIGHT 480
#define TARGET_FPS 25
#define FPS_REFRESH_TIME 1000

#define FF_RENDER_EVENT (SDL_USEREVENT + 1)


TTF_Font *open_font(const char *file, int size) {
  TTF_Font *font = TTF_OpenFont(file, size);
  if (!font) {
    printf("Font error %s!\n", file);
    exit(-1);
  }
  return font;
}

static Uint32 sdl_render_cb(Uint32 interval, void *opaque) {
  SDL_Event event;
  event.type = FF_RENDER_EVENT;
  event.user.data1 = opaque;
  SDL_PushEvent(&event);
  return 0;
}

/* schedule a video refresh in 'delay' ms */
static void schedule_rendering(int delay) {
  SDL_AddTimer(delay, sdl_render_cb, nullptr);
}

static inline void set_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  Uint8 *target_pixel = (Uint8 *) surface->pixels + y * surface->pitch +
      x * sizeof(Uint32);
  *(Uint32 *) target_pixel = pixel;
}

int main(int argc, char **argv) {
  int gogogo = 1;
  SDL_Surface *screen;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  TTF_Init();
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_NOFRAME);


  int MODEL_DRAW_OFFSET = 30;
  std::deque<int> model;

  int seed = 128;

  for (int i = 0; i < WIDTH; ++i) {
    model.push_back(seed);
    seed = (seed + 1) % 255;
  }

  TTF_Font *eu_font = open_font("freesansbold.ttf", 20);

  auto fps_counter = FpsCounter::create(2000);

  auto _render_time_span = 1000 / TARGET_FPS;

  schedule_rendering(_render_time_span);

  long last_fps_render_time = 0L;

  SDL_Color fps_color;
  fps_color.r = 0;
  fps_color.g = 255;
  fps_color.b = 0;

  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = WIDTH;
  rect.h = HEIGHT;

  SDL_Surface *text_surface = TTF_RenderUTF8_Blended(eu_font, "Gathering FPS stats...", fps_color);
  SDL_Rect sdl_dst_rect;
  sdl_dst_rect.w = text_surface->w;
  sdl_dst_rect.h = text_surface->h;
  sdl_dst_rect.x = (WIDTH - sdl_dst_rect.w) / 2;
  sdl_dst_rect.y = (HEIGHT - sdl_dst_rect.h) / 2;

  while (gogogo) {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
      if (event.type == FF_RENDER_EVENT) {
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 120, 120, 120));
        model.pop_front();
        seed = (seed + 1) % 255;
        model.push_back(seed);

        if (argc < 2) {
          for (int x = 0; x < WIDTH; ++x) {
            auto color = SDL_MapRGB(screen->format, model[x], model[x], model[x]);
            for (int y = MODEL_DRAW_OFFSET; y < HEIGHT - MODEL_DRAW_OFFSET; ++y) {
              set_pixel32(screen, x, y, color);
            }
          }
        }

        if (SDL_GetTicks() - last_fps_render_time > FPS_REFRESH_TIME) {
          std::stringstream sstr;
          sstr << "Current FPS" << " " << int(fps_counter->get_current_fps() + 0.5f);
          auto str = sstr.str();
          const char *pstr = str.c_str();
          SDL_FreeSurface(text_surface);
          text_surface = TTF_RenderUTF8_Blended(eu_font, pstr, fps_color);

          sdl_dst_rect.w = text_surface->w;
          sdl_dst_rect.h = text_surface->h;
          sdl_dst_rect.x = (WIDTH - sdl_dst_rect.w) / 2;
          sdl_dst_rect.y = (HEIGHT - sdl_dst_rect.h) / 2;

          last_fps_render_time = SDL_GetTicks();
        }

        {
          SDL_FillRect(screen, &sdl_dst_rect, SDL_MapRGB(screen->format, 10, 10, 10));
          SDL_BlitSurface(text_surface, NULL, screen, &sdl_dst_rect);
        }


        SDL_Flip(screen);
        schedule_rendering(_render_time_span);

        fps_counter->frame_rendered();
      }
      else if (event.type == SDL_KEYDOWN) {
        gogogo = 0;
      }
      else if (event.type == SDL_QUIT) {
        SDL_Quit();
        exit(0);
      }
    }
  }
  SDL_FreeSurface(text_surface);
  SDL_Quit();

  return 0;
}