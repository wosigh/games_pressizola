// Define some image filenames
#define WOSI_LOGO	"webos-internals-logo.png"

typedef struct {
	SDL_Surface *screen;			// Main SDL Surface
	SDL_Surface *back;			// SDL Back Buffer
	SDL_Rect logo;				// A rectangle for placing the title
	SDL_Rect title;				// A rectangle for placing the title
	SDL_Rect text;				// A rectangle for placing the text
	FONT titlefont, textfont;		// A couple of fonts, names explain what they are for
} SPLASH;

typedef struct {
	SDL_Rect rect;				// Rectangle to grab a character
	SDL_Surface *img;			// Image containing the characters
} FONT;
