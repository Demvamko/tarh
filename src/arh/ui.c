// #include <lib/glew.h>
// #include <cglm.h>
// #include <arh/gl.h>
// #include <arh/cam.h>
// #include <arh/ui.h>
// #include <arh/std.h>
// #include <ext/pack.h>

// typedef struct Vert{
//     short pos[2];
//     short uv[2];
//     uchar color[4];
//     uchar z;
// } Vert;

// static Attribute VERT_ATTRIBS[] = { 
//     { 2, GL_SHORT , 0, sizeof(Vert), 0 , 1 }, 
//     { 2, GL_USHORT, 1, sizeof(Vert), 4 , 0 },
//     { 4, GL_UBYTE , 1, sizeof(Vert), 8 , 0 },
//     { 1, GL_UBYTE , 1, sizeof(Vert), 12, 0 },
//     { 0 }
// };

// static Vert* verts;

// static Tag* active;
// static Tag* hot;

// static int max = 1;
// static int count = 0;

// void InitUI(){
// }

// void UpdateUI(){

// }

// void RenderUI(){

// }

// Tag* UI_AddChild(Tag* parent, Tag* child){
//     EXPAND(verts, count, max, sizeof(Vert) * 6)

//     child->vertices = verts + count;
//     count++;



//     if(parent->child){
//         Tag* sibling = parent->child;

//         while(sibling->sibling)
//             sibling = sibling->sibling;

//         sibling->sibling = child;
//     }
//     else
//         parent->child = child;

//     child->parent = parent;

//     UI_RefreshChildClients(parent);

//     return child;
// }


// void UI_RefreshChildClients(Tag* parent){
//     #define CHILD_SIZE(dim) (child->size[dim] + (parent->client[2 + dim] * (child->size[dim] <= 1.0f)))

//     //PREPARE PARENT OFFSET
//     int dim = parent->layout;
//     int space = parent->client[2 + dim] - parent->client[dim];

//     for(Tag* child = parent->child; child; child = child->sibling)
//         space -= CHILD_SIZE(dim);

//     int muls[] = { 0.0f, 1.0f, 0.5f };

//     int offset[2] = { parent->client[0], parent->client[1] };
//     offset[dim] += space * muls[parent->align[dim]];

//     //CALCULATE CHILD CLIENT POSITIONS
//     for(Tag* child = parent->child; child; child = child->sibling){
//         offset[1 - dim] = parent->client[3 - dim] * muls[parent->align[dim]]; 

//         child->client[0] = offset[0];
//         child->client[1] = offset[1];
//         child->client[2] = offset[0] + CHILD_SIZE(0);
//         child->client[3] = offset[1] + CHILD_SIZE(1);

//         offset[dim] += CHILD_SIZE(dim);

//         UI_RefreshChildClients(child);
//         UI_RefreshVerts(child);
//     }

//     #undef CHILD_SIZE
// }

// #define UI_ATLAS_UVS 0
// static int UI_RECT_TABLE[6][2] = { { 0, 1 }, { 0, 3 }, { 2, 3 }, { 2, 3 }, { 2, 1 }, { 0, 1 } };

// void UI_RefreshVerts(Tag* tag){
//     #define FOR(x) for(int i = 0; i < x; i++)

//     Vert* vert = tag->vertices;
//     float* uv = Arh_GetResource(ATLAS_IMG_acacia_tree);

//     for(int v = 0; v < 6; v++, vert++){
//         FOR(2) vert->pos[i] = tag->client[UI_RECT_TABLE[v][i]];
//         FOR(2) vert->uv[i] = 0xFFFF * uv[UI_RECT_TABLE[v][i]];
//         FOR(4) vert->color[i] = tag->color[i];

//         vert->z = tag->depth;
//     }

//     #undef FOR
// }