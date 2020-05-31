#include <arhblocks.h>
#include <arhblock_defines.h>
#include <stdlib.h>
#include <cglm.h>
#include <math.h>

#define SIGN(x) ((x) > 0) - ((x) < 0)

vec3 raycast_hit;
vec3 raycast_hit_prev;

void RayCastLine(vec3 start, vec3 end, int maxdist){
    vec3 curr;
    vec3 step;
    vec3 next;
    vec3 delta;

    for(int i = 0; i < 3; i++){
        curr[i] = floor(start[i]);
        step[i] = SIGN(end[i] - start[i]);
        delta[i] = 1.0f / fabs(end[i] - start[i]);

        float min = floor(start[i]);
        float max = min + 1.0f;

        next[i] = ((start[i] > end[i]) ? (start[i] - min) : (max - start[i])) * delta[i];
    }

    for(int i = 0; i < maxdist; i++){
        if(next[0] < next[1] && next[0] < next[2]){
            next[0] += delta[0];
            curr[0] += step[0];
        }
        else if(next[1] < next[2]){
            next[1] += delta[1];
            curr[1] += step[1];
        }
        else{
            next[2] += delta[2];
            curr[2] += step[2];
        }

        glm_vec3_copy(raycast_hit, raycast_hit_prev);
        glm_vec3_copy(curr, raycast_hit);

        if(GetBlockAny(curr[0], curr[1], curr[2]) != TYPE_AIR)
            return;
    }
}

void RayCast(vec3 start, vec3 dir, int maxdist){
    vec3 end;

    for(int i = 0; i < 3; i++)
        end[i] = start[i] + dir[i] * maxdist;

    RayCastLine(start, end, maxdist);
}