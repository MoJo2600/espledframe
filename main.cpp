#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
// #include "lib/mylib.h"
#include "lib/gifdec.h"

long long millis() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

const int NUM_LEDS = 64;
long leds[64];

struct animation {
    int
        repeats = 5,
        currentRepeat = 1,
        pause = 500,
        currentFrame = 0,
        frameCount;
    const long * frames; // Pointer to an array
};

// Actual array
const long animation1_images[2][NUM_LEDS] = {
    {
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009
    },
    {
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
    }
};

const long animation2_images[3][NUM_LEDS] = {
    {
        0x000000, 0x000009, 0x000009, 0x000009, 0x000009, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000009, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000009, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
    },
    {
        0x000000, 0x000009, 0x000009, 0x000009, 0x000009, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000009, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000009, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
    },
    {
        0x000000, 0x000009, 0x000009, 0x000009, 0x000009, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000009, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000000, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000, 0x000000,
        0x000009, 0x000009, 0x000000, 0x000000, 0x000000, 0x000009, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
        0x000009, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000009, 0x000000,
    },
};

animation animation1;
animation animation2;

// const long *animation1_images_ptr = animation1_images;

// Define all animations
void defineAnimations() {
    // Set pointer to array address
    animation1.frames = animation1_images[0];
    animation1.pause = 1000;
    animation1.repeats = 3;
    animation1.frameCount = sizeof(animation1_images) / sizeof(animation1_images[0]);

    animation2.frames = animation2_images[0];
    animation2.pause = 500;
    animation2.repeats = 5;
    animation2.frameCount = sizeof(animation2_images) / sizeof(animation2_images[0]);
}

void showFrame(const long *arr, int frame) {
    int lines = NUM_LEDS / 8;
    int startIndex = 0;
    int endIndex = 0;

    int counter = 0;
    printf("Frame %d\n", frame);
    for (int line = 0; line < lines; line++)
    {
        startIndex = line * 8;
        int index = 0;
        for (int column = 0; column < 8; column++) {
            if (line % 2 == 0) {
                index = startIndex + column;
            } else {
                index = startIndex + 7 - column;
            }
            // printf("%lu,", *((arr+frame*NUM_LEDS) + index));
            // leds[counter] = arr[frame][index];
            leds[counter] = *((arr + frame * NUM_LEDS) + index);
            if (*((arr+frame*NUM_LEDS) + startIndex + column) == 0) {
                printf(" ");
            } else {
                printf("%lu", *((arr+frame*NUM_LEDS) + startIndex + column));
            }
            counter++;
        }
        printf("\n");
    }
}

unsigned long previousMillis = 0;

void animationLoop(animation *anim) {
    if (anim->currentRepeat <= anim->repeats) {

        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= anim->pause)
        {
            printf("Time passed: %lld\n", millis());
            previousMillis = currentMillis;
            showFrame(anim->frames, anim->currentFrame);
            printf("frames: %d, currentFrame %d, repeat %d\n", anim->frameCount, anim->currentFrame, anim->currentRepeat);
            if (anim->currentFrame == anim->frameCount-1)
            {
                anim->currentFrame = 0;
                anim->currentRepeat++;
            }
            else
            {
                anim->currentFrame++;
            }
        }

    } else {
        // ... wenn alle repeats gespielt sind, gehe zurück zur abfrage
        anim->currentRepeat = 0;
        anim = nullptr;
    }
}

// current_animation is ein Zeiger der auf eine typedef animation zeigen soll
// er wird mit nullptr initialisiert, damit klar ist, er zeigt auf "nichts"
animation* current_animation = nullptr;
char input_character;

#define FILENAME "img.gif"

void setup() {
    defineAnimations();

    // gd_GIF *gif;
    // int *color, *frame;
    // char const *file = FILENAME;

    // const char s[] = {'i', 'm', 'g', '.', 'g', 'i', 'f', '\0'};
    // gif = gd_open_gif(argv[1]);

    //gif = gd_open_gif("img.gif");
    //frame = (int *) malloc(gif->width * gif->height * 3);

    //printf("GIF %dx%d %d colors",
    //         gif->width, gif->height, gif->palette->size);
}

void loop() {

    // wenn gerade keine Animation gesetzt ist, dann...
    if (current_animation == nullptr) {
        //... frag welche Animation gespielt werden soll
        printf( "Enter a or b:");
        input_character = getchar();

        // ... setze den Pointer auf die Adresse der animation
        if (input_character == 'a') {
            current_animation = &animation1;
        } else if (input_character == 'b') {
            current_animation = &animation2;
        }
    } else {
        // Wenn der Pointer gesetzt ist, dann spiele die entsprechende animation
        if (current_animation->currentRepeat <= current_animation->repeats) {
            animationLoop(current_animation);
        } else {
            // ... wenn alle repeats gespielt sind, gehe zurück zur abfrage
            current_animation->currentRepeat = 0;
            current_animation = nullptr;
        }
    }
}


// Not for ESP controller
// Some stuff do be able to stop the program with Ctrl+c
volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}

int rgbToAnsi256(int r, int g, int b) {
    // we use the extended greyscale palette here, with the exception of
    // black and white. normal palette only has 4 greyscale shades.
    if (r == g && g == b) {
        if (r < 8) {
            return 16;
        }

        if (r > 248) {
            return 231;
        }

        return ((r - 8) / 247) * 24 + 232;
    }

    int ansi = 16
        + (36 * r / 255 * 5)
        + (6 * g / 255 * 5)
        + b / 255 * 5;

    return ansi;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, inthand);

    // int *foo;
    gd_GIF *gif;
    const char *file = "img.gif";
    uint8_t *color, *frame;
    int ret;

    gif = gd_open_gif(file);
    frame = (uint8_t*) malloc(gif->width * gif->height * 3);

    printf("GIF %dx%d %d colors\n",
             gif->width, gif->height, gif->palette->size);

    color = &gif->gct.colors[gif->bgindex * 3];


    // printf("%n", color);

    setup();

    unsigned long delay = 0;
    while (!stop)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= delay)
        {
            previousMillis = currentMillis;

            ret = gd_get_frame(gif);
            if (ret == -1)
                return ret;

            gd_render_frame(gif, frame);
            color = frame;
            for (int i = 0; i < gif->height; i++) {
                for (int j = 0; j < gif->width; j++) {
                    //addr = surface->pixels + (i * surface->pitch + j * sizeof(pixel));
                    //pixel = SDL_MapRGB(surface->format, color[0], color[1], color[2]);
                    //memcpy(addr, &pixel, sizeof(pixel));
                    if (color[0] == 0 && color[1] == 0 && color[2] == 0) {
                        printf(" ");
                    } else {
                        int ansicolor = rgbToAnsi256(color[0], color[1], color[2]);
                        printf("\u001b[38;5;%dm0", ansicolor);
                    }
                    
                    // printf("Pixel %u, %u, %u", color[0], color[1], color[2]);
                    color += 3;
                }
                printf("\n");
            }

            delay = gif->gce.delay * 10;
            // printf("delay: %d", delay);

            if (ret == 0)
                gd_rewind(gif);
        }
        // loop();
    }
    free(frame);

    return 0;
}
