// typedef struct Ling {
//     uint id;

//     uchar health;
//     uchar hunger;
//     uchar tiredness;
//     uchar happiness;

//     uint animation;

//     Routine routine;

//     uint bufferid;
//     float position[3];
//     float rotation;

//     float speed;
// } Ling;

// typedef struct Routine {
//     void (*tick)(Ling* ling, float t, float dt);
//     void (*next)(Ling* ling, float t, float dt);
//     char data[16];
// } Routine;

// #define ANIM_WALKING 0
// #define ANIM_DIGGING 0
// #define ANIM_STORING 0

// void

// static void Ling_R_Idle(Ling* ling, float t, float dt){
//     //FIND ROUTINE THAT LING CAN DO
//     //ASSIGN ROUTINE
//     if(1){ //found_action_digging
//         ling->routine.tick = Ling_R_Moving;
//         ling->routine.next = Ling_R_Digging;

//         ((float*)ling->routine.data)[0] = 0;
//         ((float*)ling->routine.data)[1] = 0;
//         ((float*)ling->routine.data)[2] = 0;

//         ling->routine.data[15] = ANIM_DIGGING;
//         ling->animation = Arh_Anim_Start(ANIM_WALKING);
//     }
// }

// float* Arh_Path_FindNext(float* a, float* b, float movement, float dt);

// static void Ling_R_Moving(Ling* ling, float t, float dt){
//     float* nextpos = Arh_Path_FindNext(ling->position, ling->routine.data, ling->speed, dt);

//     if(nextpos){
//         Arh_Anim_Update(ling->animation, t);
//         ling->position[0] = nextpos[0];
//         ling->position[1] = nextpos[1];
//         ling->position[2] = nextpos[2];
//         //UPDATE LING ROTATION
//         return;
//     }
//     else{
//         ling->routine.tick = ling->routine.next;
//         ling->animation = Arh_Anim_Start(ling->routine.data[15], t);
//     }
// }

// static void Ling_R_Digging(Ling* ling, float t, float dt){
//     if(Arh_Anim_Update(ANIM_DIGGING))
//         return;

//     ling->routine.tick = Ling_R_Moving;
//     ling->routine.next = Ling_R_Store;

//     ling->routine.data[15] = ANIM_STORING;
//     ling->routine.data[14] = 0;

//     ((float*)ling->routine.data)[0] = 0;
//     ((float*)ling->routine.data)[1] = 0;
//     ((float*)ling->routine.data)[2] = 0;   
// }

// static void Ling_R_Store(Ling* ling, float t, float dt){
//     if(Arh_Anim_Update(ling->animation))
//         return;

//     Arh_Storage_Add(ling->routine.data[14], 0);
//     Ling_R_Idle(ling, t, dt);
// }