#include <cglm.h>

typedef struct Track {
    int* times;
    float* quats;
    int len;
} Track;

void UpdateRotations(Track* track, float* rotation, int time){
    int key = 0;

    int prev = track->times[0];
    int curr = track->times[1];

    time = time % track->times[track->len - 1];

    for(key = 1; key < track->len; key++)
        if(track->times[key - 1] < time && track->times[key] > time)
            break;

    rotation[0] = track->quats[key + 0];
    rotation[1] = track->quats[key + 1];
    rotation[2] = track->quats[key + 2];
    rotation[3] = track->quats[key + 3];
}