#include <ESP8266WiFi.h>

const int NUM_LEDS = 64;
long leds[64];

struct animation {
    int
        repeats = 5,
        currentRepeat = 1,
        pause = 500,
        currentFrame = 0,
        frameCount = 0;
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
    animation1.frameCount = 2;

    animation2.frames = animation2_images[0];
    animation2.pause = 500;
    animation2.repeats = 5;
    animation2.frameCount = 3;
}

void showFrame(const long *arr, int frame) {
    int lines = NUM_LEDS / 8;
    int startIndex = 0;
    int endIndex = 0;

    int counter = 0;
    Serial.printf("Frame %d\n", frame);
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
            leds[counter] = *((arr + frame * NUM_LEDS) + index);
            if (*((arr+frame*NUM_LEDS) + startIndex + column) == 0) {
                Serial.printf(" ");
            } else {
                Serial.printf("%lu", *((arr+frame*NUM_LEDS) + startIndex + column));
            }
            counter++;
        }
        Serial.printf("\n");
    }
}

unsigned long previousMillis = 0;

void animationLoop(animation *anim) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= anim->pause)
    {
        previousMillis = currentMillis;
        Serial.println("here we go! >");
        Serial.printf("CurrentFrame: %d", anim->currentFrame);
        Serial.printf("FrameCount: %d", anim->frameCount);
        Serial.printf("Repeat: %d", anim->currentRepeat);
        showFrame(anim->frames, anim->currentFrame);
        // // Serial.println("frames: %d, currentFrame %d, repeat %d\n", anim->frameCount, anim->currentFrame, anim->currentRepeat);
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
}

// current_animation is ein Zeiger der auf eine typedef animation zeigen soll
// er wird mit nullptr initialisiert, damit klar ist, er zeigt auf "nichts"
animation* current_animation = &animation1;

void setup() {
  Serial.begin(74880);
  delay(10);

  defineAnimations();
}

// current_animation = &animation1;
void loop() {

    // wenn gerade keine Animation gesetzt ist, dann...
    if (current_animation != nullptr) {
        // Wenn der Pointer gesetzt ist, dann spiele die entsprechende animation
        if (current_animation->currentRepeat <= current_animation->repeats) {
            animationLoop(current_animation);
        } else {
            // ... wenn alle repeats gespielt sind, gehe zurück zur abfrage
            current_animation->currentRepeat = 0;
            current_animation = &animation1;
            // current_animation = nullptr;
        }
    }
}


// // Not for ESP controller
// // Some stuff do be able to stop the program with Ctrl+c
// volatile sig_atomic_t stop;

// void inthand(int signum) {
//     stop = 1;
// }

// int main(int argc, char **argv) {
//     signal(SIGINT, inthand);

//     defineAnimations();

//     while (!stop)
//     {
//         loop();
//     }
//     return 0;
// }
