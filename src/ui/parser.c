#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arhstd.h>
#include <rect.h>
#include <ui.h>

const char delimiters[] = " \"";

#define ARGS(len, reciever, offset, parser) \
    for(int i = 0; i < len; i++){ \
        char* ftok = strtok(0, delimiters); \
        reciever[i + offset] = parser(ftok); \
    }

Tag* UI_LoadFile(char* path, int* out_count){
    int size = 0;
    char* file = ArhLoadFile(path, &size);

    int count;
    return UI_ParseFile(file, size, &count);
}

Tag* UI_ParseFile(char* file, int size, int* out_count){
    Tag* tags = calloc(1024, sizeof(Tag));
    Tag* last = 0;
    int cur = 0;

    char* line = file;

    for(int i = 0; i < size; i++){
        if(file[i] != '\n')
            continue;

        file[i] = '\0';

        //IF NOT A TAG, THEN WE ARE A TEXT
        if(!strstr(line, "::")){

            //IF LAST ONE WAS ALSO A TEXT UNDO TERMINATION
            if(last->text)
                *(line - 1) = '\n';
            else
                last->text = line;
            
            line = file + i + 1;
        }
        else {
            UI_ParseTag(tags + cur, line, last);

            line = file + i + 1;
            last = tags + cur;
            cur++;
        }
    }

    *out_count = cur;
    return tags;
}

void UI_ParseTag(Tag* tag, char* line, Tag* last){
    
    for(;line[tag->depth] == ' '; tag->depth++);

    char* tok = strtok(line, delimiters);

    do{
        if(!strcmp(tok, "::")){
            tok = strtok(0, delimiters);
            continue;
        }

        //DIRECTION
        else if(!strcmp(tok, "row"))
            tag->type = TYPE_ROW;

        else if(!strcmp(tok, "col"))
            tag->type = TYPE_COL;

        else if(!strcmp(tok, "table"))
            tag->type = TYPE_TABLE;

        //ALIGNING
        else if(!strcmp(tok, "start"))
            tag->alignment = ALIGN_START;

        else if(!strcmp(tok, "center"))
            tag->alignment = ALIGN_CENTER;

        else if(!strcmp(tok, "end"))
            tag->alignment = ALIGN_END;

        //DISPLAYING
        else if(!strcmp(tok, "overflow"))
            tag->display = DISPLAY_OVERFLOW;

        else if(!strcmp(tok, "fit"))
            tag->display = DISPLAY_FIT;

        //POSITIONING
        else if(!strcmp(tok, "default"))
            tag->position = POSITION_DEFAULT;

        else if(!strcmp(tok, "absolute"))
            tag->position = POSITION_ABSOLUTE;

        else if(!strcmp(tok, "anchored"))
            tag->position = POSITION_ANCHORED;

        //DIMENSIONS
        else if(!strcmp(tok, "size="))
            ARGS(2, tag->rect, 2, atoi)

        else if(!strcmp(tok, "position="))
            ARGS(2, tag->rect, 0, atoi)

        else if(!strcmp(tok, "anchors=")){
            ARGS(4, tag->anchors, 0, atof)
            tag->position = POSITION_ANCHORED;
        }

        else if(!strcmp(tok, "absolute=")){
            ARGS(4, tag->rect, 0, atoi)
            tag->position = POSITION_ABSOLUTE;
        }

        else
            strcpy(tag->custom_tag, tok);

        tok = strtok(0, delimiters);

    } while(tok);

    //FIND SIBLING AND PARENT
    while(last && last->depth >= tag->depth){
        if(last->depth == tag->depth)
            last->sibling = tag;

        last = last->parent;
    }

    tag->parent = last;

}