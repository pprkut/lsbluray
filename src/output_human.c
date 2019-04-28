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
        printf("Disc ID: Unknown");
    }
    else
    {
        printf("Disc ID: ");
        for (int i = 0; i < sizeof(bd_info->generic->disc_id); i++) {
            printf("%02X", bd_info->generic->disc_id[i]);
        }
    }

    if (bd_info->generic->aacs_detected == 1)
    {
        printf(", AACS: MKBv%i", bd_info->generic->aacs_mkbv);
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

    struct duration length;
    char usec[7];

    for (int i=0; i < bd_info->title_count; i++)
    {
        if (opt_t != i+1 && opt_t != 0)
        {
            continue;
        }

        length.hours   = bd_info->titles[i].duration.tv_sec / 60 / 60;
        length.minutes = bd_info->titles[i].duration.tv_sec / 60 % 60;
        length.seconds = bd_info->titles[i].duration.tv_sec % 60;
        length.microseconds = bd_info->titles[i].duration.tv_usec;

        snprintf(usec, 7, "%06lu", bd_info->titles[i].duration.tv_usec);

        printf("Title: %02i", i+1);
        printf(", Length: %02i:%02i:%02i.%.3s", length.hours, length.minutes, length.seconds, usec);
        printf(", Chapters: %02i", bd_info->titles[i].chapter_count);
        printf(", Clips: %02i", bd_info->titles[i].clip_count);
        printf(", Angles: %02i", bd_info->titles[i].angle_count);
        printf(", Marks: %02i\n", bd_info->titles[i].mark_count);

        if (bd_info->titles[i].chapters)
        {
            for (int j=0; j < bd_info->titles[i].chapter_count; j++)
            {
                length.hours   = bd_info->titles[i].chapters[j].duration.tv_sec / 60 / 60;
                length.minutes = bd_info->titles[i].chapters[j].duration.tv_sec / 60 % 60;
                length.seconds = bd_info->titles[i].chapters[j].duration.tv_sec % 60;
                length.microseconds = bd_info->titles[i].chapters[j].duration.tv_usec;

                snprintf(usec, 7, "%06lu", bd_info->titles[i].chapters[j].duration.tv_usec);

                printf("\tChapter: %02i", j+1);
                printf(", Length: %02i:%02i:%02i.%.3s", length.hours, length.minutes, length.seconds, usec);
                printf(", Clip: %02i\n", bd_info->titles[i].chapters[j].clip + 1);
            }
        }

        if (bd_info->titles[i].marks)
        {
            for (int j=0; j < bd_info->titles[i].mark_count; j++)
            {
                length.hours   = bd_info->titles[i].marks[j].start.tv_sec / 60 / 60;
                length.minutes = bd_info->titles[i].marks[j].start.tv_sec / 60 % 60;
                length.seconds = bd_info->titles[i].marks[j].start.tv_sec % 60;
                length.microseconds = bd_info->titles[i].marks[j].start.tv_usec;

                snprintf(usec, 7, "%06lu", bd_info->titles[i].marks[j].start.tv_usec);

                printf("\tMark: %02i", j+1);
                printf(", Start: %02i:%02i:%02i.%.3s", length.hours, length.minutes, length.seconds, usec);
                printf(", Clip: %02i\n", bd_info->titles[i].marks[j].clip + 1);
            }
        }

        if (bd_info->titles[i].clips)
        {
            for (int j=0; j < bd_info->titles[i].clip_count; j++)
            {
                if (opt_D != j+1 && opt_D != 0)
                {
                    continue;
                }

                length.hours   = bd_info->titles[i].clips[j].duration.tv_sec / 60 / 60;
                length.minutes = bd_info->titles[i].clips[j].duration.tv_sec / 60 % 60;
                length.seconds = bd_info->titles[i].clips[j].duration.tv_sec % 60;
                length.microseconds = bd_info->titles[i].clips[j].duration.tv_usec;

                snprintf(usec, 7, "%06lu", bd_info->titles[i].clips[j].duration.tv_usec);

                printf("\tClip: %02i", j+1);
                printf(", Length: %02i:%02i:%02i.%.3s", length.hours, length.minutes, length.seconds, usec);
                printf(", Video streams: %02i", bd_info->titles[i].clips[j].video_count);
                printf(", Audio streams: %02i", bd_info->titles[i].clips[j].audio_count);
                printf(", PG subtitle streams: %02i", bd_info->titles[i].clips[j].pg_count);
                printf(", IG menu streams: %02i\n", bd_info->titles[i].clips[j].ig_count);

                if (bd_info->titles[i].clips[j].video_streams)
                {
                    for (int k=0; k < bd_info->titles[i].clips[j].video_count; k++)
                    {
                        printf("\t\tVideo: %02i", k+1);
                        printf(", Format: %s", bd_info->titles[i].clips[j].video_streams[k].format);
                        printf(", Resolution: %s", bd_info->titles[i].clips[j].video_streams[k].resolution);
                        printf(", Aspect Ratio: %s", bd_info->titles[i].clips[j].video_streams[k].aspect_ratio);
                        printf(", FPS: %s\n", bd_info->titles[i].clips[j].video_streams[k].fps);
                    }
                }

                if (bd_info->titles[i].clips[j].audio_streams)
                {
                    for (int k=0; k < bd_info->titles[i].clips[j].audio_count; k++)
                    {
                        printf("\t\tAudio: %02i", k+1);
                        printf(", Language: %s - %s", bd_info->titles[i].clips[j].audio_streams[k].language_code, bd_info->titles[i].clips[j].audio_streams[k].language_name);
                        printf(", Format: %s", bd_info->titles[i].clips[j].audio_streams[k].format);
                        printf(", Frequency: %s", bd_info->titles[i].clips[j].audio_streams[k].samplerate);
                        printf(", Channels: %s\n", bd_info->titles[i].clips[j].audio_streams[k].channels);
                    }
                }

                if (bd_info->titles[i].clips[j].subtitle_streams)
                {
                    for (int k=0; k < bd_info->titles[i].clips[j].pg_count; k++)
                    {
                        printf("\t\tSubtitle: %02i", k+1);
                        printf(", Language: %s - %s\n", bd_info->titles[i].clips[j].subtitle_streams[k].language_code, bd_info->titles[i].clips[j].subtitle_streams[k].language_name);
                    }
                }
            }
        }
    }
}
