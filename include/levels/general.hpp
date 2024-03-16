#include <SDL.h>
class Level {
    public:
        virtual void setup();
        virtual void show();
        virtual void update(SDL_Event key);
    private:
        virtual void events(SDL_Event key);
};