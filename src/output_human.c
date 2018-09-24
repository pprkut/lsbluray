/*****************************************************************************
 * Copyright (C) 2018 Heinz Wiesinger <pprkut@liwjatan.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "output_human.h"

void print_human_readable(struct bd_info *bd_info)
{
    if (bd_info->generic->disc_id[0] == 0)
    {
        printf("Disc Title: Unknown");
    }
    else
    {
        printf("Disc Title: %s", bd_info->generic->disc_id);
    }

    if (bd_info->generic->aacs_detected == 1)
    {
        printf(", AACS: MKVv%i", bd_info->generic->aacs_mkbv);
    }
    else
    {
        printf(", AACS: None");
    }

    if (bd_info->generic->bdplus_detected == 1)
    {
        printf(", BD+: Gen %i, %i\n", bd_info->generic->bdplus_gen, bd_info->generic->bdplus_date);
    }
    else
    {
        printf(", BD+: None\n");
    }

    printf("Disc Name: %s\n", bd_info->generic->disc_name);
    printf("Disc Volume ID: %s\n", bd_info->generic->udf_volume_id);

    for (int i=0; i < bd_info->title_count; i++)
    {
        if (opt_t != i+1 && opt_t != 0)
        {
            continue;
        }

        struct duration length;

        length.hours   = bd_info->titles[i].duration.tv_sec / 60 / 60;
        length.minutes = bd_info->titles[i].duration.tv_sec / 60 % 60;
        length.seconds = bd_info->titles[i].duration.tv_sec % 60;
        length.microseconds = bd_info->titles[i].duration.tv_usec;

        char usec[7];
        snprintf(usec, 7, "%lu", bd_info->titles[i].duration.tv_usec);

        printf("Title: %02i", i+1);
        printf(", Length: %02i:%02i:%02i.%.3s", length.hours, length.minutes, length.seconds, usec);
        printf(", Chapters: %02i", bd_info->titles[i].chapter_count);
        printf(", Clips: %02i", bd_info->titles[i].clip_count);
        printf(", Angles: %02i", bd_info->titles[i].angle_count);
        printf(", Marks: %02i\n", bd_info->titles[i].mark_count);
    }
}
