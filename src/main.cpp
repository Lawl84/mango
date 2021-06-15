#include <SDL.h>
#include "mango.h"

int main(int argc, char** argv)
{
    Mango* m = new Mango();
    m->mainloop();
    delete m;
    return 0;
}
