#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "data.h"
#include "accent.h"
#include "./actors/actor.h"

#include "serialization.h"

bool serialize_map(World *world, const char *filename)
{
    FILE *save = fopen(filename, "w");
    if (save == NULL)
        return false;
    // Width and height
    fprintf(save, "%u,%u\n", world->width, world->height);
    // Tiles
    size_t board_area = world->width * world->height;
    for (int i = 0; i < board_area; i++)
    {
        fprintf(save, "%d,", world->board[i]);
        if ((i + 1) % world->width == 0)
            fprintf(save, "\n");
    }
    // Accents
    for (int i = 0; i < board_area; i++)
    {
        fprintf(save, "%d,", world->accents[i].type);
        if ((i + 1) % world->width == 0)
            fprintf(save, "\n");
    }
    // Actors
    for (int i = 0; i < board_area; i++)
    {
        fprintf(save, "%d,", world->actors[i].type);
        if ((i + 1) % world->width == 0)
            fprintf(save, "\n");
    }
    fclose(save);
    return true;
}

bool deserialize_map(World *world, const char *filename)
{
    FILE *save = fopen(filename, "r");
    if (save == NULL)
        return false;

    char *line = NULL;
    size_t len = 0;
    char c = '\0';
    // Read the first line, get the width and height
    while ((c = fgetc(save)) != '\n')
    {
        if (c == EOF)
        {
            fclose(save);
            return false;
        }
        len++;
    }
    line = (char *)malloc(sizeof(char) * (len + 1));
    rewind(save);
    for (int i = 0; i < len; i++)
    {
        line[i] = fgetc(save);
    }
    line[len] = '\0';
    char *raw_dim = line;
    uint16_t width, height;
    char *dim = NULL;
    dim = strtok(raw_dim, ",");
    width = strtol(dim, NULL, 0);
    dim = strtok(NULL, ",");
    height = strtol(dim, NULL, 0);
    free(line);
    line = NULL;
    // Validate the width and height.
    if (width < 0 || height < 0)
    {
        fclose(save);
        return false;
    }
    // Allocate the board, the accents and the actors.
    init_world(world, width, height);

    // Board base.
    char buff[256] = {0};
    size_t pos = 0;
    int val = 0;
    size_t buff_size = width * height;
    for (size_t i = 0; i < buff_size;)
    {
        c = fgetc(save);
        switch (c)
        {
        case EOF:
            free_world(world);
            fclose(save);
            return false;
        case '\n':
        case '\r':
            continue;
        case ',':
            val = strtol(buff, NULL, 0);
            world->board[i] = (char)val;
            memset(buff, '\0', pos);
            val = 0;
            pos = 0;
            i++;
            continue;
        default:
            buff[pos] = c;
            pos++;
        }
    }

    // Accents.
    pos = 0;
    val = 0;
    memset(buff, '\0', 255);
    for (size_t i = 0; i < buff_size;)
    {
        c = fgetc(save);
        switch (c)
        {
        case EOF:
            free_world(world);
            fclose(save);
            return false;
        case '\n':
        case '\r':
            continue;
        case ',':
            val = strtol(buff, NULL, 0);
            if (val > NO_ACCENT && val < NUM_ACCENTS)
                world->accents[i] = create_accent[val]();
            memset(buff, '\0', pos);
            val = 0;
            pos = 0;
            i++;
            continue;
        default:
            buff[pos] = c;
            pos++;
        }
    }

    // Actors.
    pos = 0;
    val = 0;
    memset(buff, '\0', 255);
    for (size_t i = 0; i < buff_size;)
    {
        c = fgetc(save);
        switch (c)
        {
        case EOF:
            free_world(world);
            fclose(save);
            return false;
        case '\n':
        case '\r':
            continue;
        case ',':
            val = strtol(buff, NULL, 0);
            if (val > NO_ACTOR && val < NUM_ACTORS)
                world->actors[i] = create_actor[val]((ActorPosition) { .x = i % width, .y = i % height }, 0);
            memset(buff, '\0', pos);
            val = 0;
            pos = 0;
            i++;
            continue;
        default:
            buff[pos] = c;
            pos++;
        }
    }
    fclose(save);
    return true;
}
