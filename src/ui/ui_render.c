#include <ui.h>

#define LERP(a, b, amount) (a + amount * (b - a))

static void SetChildClients(Tag* parent, int* viewport);

void UI_TagsToBuffer(Tag* tags, int count, int* viewport){
    for(int i = 0; i < count; i++){
        if(!tags[i].parent){
            tags[i].client[0] = 0;
            tags[i].client[1] = 0;
            tags[i].client[2] = viewport[0];
            tags[i].client[3] = viewport[1];

            SetChildClients(tags + i, viewport);
        }
    }

    
}

static void SetChildClients(Tag* parent, int* viewport){
    if(parent[1].depth <= parent->depth)
        return;

    int dim = parent->type == TYPE_COL;
    int space = parent->client[2 + dim];

    int children_dim = 0;

    for(Tag* child = parent + 1; child; child = child->sibling)
        if(child->position == POSITION_DEFAULT)
            children_dim += child->rect[dim];

    int remaining_space = parent->client[2 + dim] - children_dim;

    int offset[2];
    offset[0] = parent->client[0];
    offset[1] = parent->client[1];

    if(parent->alignment)
        offset[dim] += remaining_space / parent->alignment;

    for(Tag* child = parent + 1; child; child = child->sibling){
        if(child->position == POSITION_DEFAULT){
            child->client[0] = offset[0];
            child->client[1] = offset[1];
            child->client[2] = child->rect[2];
            child->client[3] = child->rect[3];

            offset[dim] += child->client[2 + dim];
        }

        else if(child->position == POSITION_ABSOLUTE){
            child->client[0] = parent->client[0] + child->rect[0];
            child->client[1] = parent->client[1] + child->rect[1];
            child->client[2] = child->rect[2];
            child->client[3] = child->rect[3];
        }

        else if(child->position == POSITION_ANCHORED){
            child->client[0] = LERP(parent->client[0], parent->client[2], child->anchors[0]);
            child->client[1] = LERP(parent->client[1], parent->client[3], child->anchors[1]);
            child->client[2] = LERP(parent->client[0], parent->client[2], child->anchors[2]);
            child->client[3] = LERP(parent->client[1], parent->client[3], child->anchors[3]);
        }

        SetChildClients(child, viewport);
    }
}