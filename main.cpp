#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

int main(int argc, char **argv) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " <font> <size>." << endl;
		return -1;
	}
	SDL_Window *window = nullptr;
	unsigned long chars[256];

	std::ifstream in("data/ascii.txt");
	unsigned int i, c;
	while (in >> i >> c)
		chars[i] = c;
	in.close();

	if (TTF_Init() == -1) {
		cout << "TTF_Init: " << TTF_GetError() << endl;
		return 1;
	}

	TTF_Font *font = TTF_OpenFont(argv[1], atoi(argv[2]));
	int w, h;
	TTF_SizeText(font, "0", &w, &h);
	h++;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError()
				  << std::endl;
		return 2;
	}
	SDL_Surface *screen = nullptr;
	screen = SDL_CreateRGBSurface(0, w * 16, h * 16, 32, 0, 0, 0, 0);
	if (screen == nullptr) {
		SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
		return 3;
	}
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = w * 16;
	rect.h = h * 16;
	SDL_FillRect(screen, &rect, 0x000000);
	SDL_Color color = {255, 255, 255};
	SDL_Surface *text_surface;
	int x = 0, y = 0;
	rect.w = w;
	rect.h = h;
	for (int n = 0; n < 256; n++) {
		wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;
		string c = convert.to_bytes(chars[n]);
		text_surface = TTF_RenderUTF8_Blended(font, c.c_str(), color);
		rect.x = x;
		rect.y = y;
		SDL_BlitSurface(text_surface, nullptr, screen, &rect);
		x = x + w;
		if (x == w * 16) {
			x = 0;
			y = y + h;
		}
	}
	IMG_SavePNG(screen, "font.png");
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
