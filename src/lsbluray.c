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
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <libbluray/bluray.h>
#include "getopt.h"
#include "version.h"
#include "lsbluray.h"
#include "output_human.h"

char* program_name;

int opt_t = 0;

static void version(void)
{
    printf("lsbluray %s\n", LSBLURAY_VERSION);
}

static void usage(void)
{
    version();
    printf("Usage: %s [options] [-t track_number] [bluray path] \n", program_name);
    printf("\n");
    printf("Options:\n");
    printf("\n");
    printf("\tOther options:\n");
    printf("\t  -h this message\n");
    printf("\t  -V version information\n");
    printf("\n");
}

int main(int argc, char *argv[])
{

    int c;
    char *bluray_device = DEFAULT_BLURAY_DEVICE;
    struct stat device_stat;
    int ret = 0;

    program_name = argv[0];

    while ((c = getopt(argc, argv, "hV?t:")) != EOF)
    {
        switch (c)
        {
            case 'h':
            case '?':
                usage();
                return 0;
            case 'V':
                version();
                return 0;
            case 't':
                opt_t = atoi(optarg);
                break;
        }
    }

    if (argv[optind])
    {
        bluray_device = argv[optind];
    }

    ret = stat(bluray_device, &device_stat);

    if ( ret < 0 )
    {
        fprintf(stderr, "Couldn't find device %s!\n", bluray_device);
        return 1;
    }

    BLURAY *bd = bd_open(bluray_device, NULL);

    if (!bd)
    {
        fprintf(stderr, "Couldn't open disc %s!\n", bluray_device);
        return 2;
    }

    struct bd_info bd_info;

    bd_info.generic = bd_get_disc_info(bd);

    if (!bd_info.generic)
    {
        fprintf(stderr, "Couldn't get information for disc!\n");
        return 2;
    }

    bd_info.title_count = bd_get_titles(bd, 0, 0);

    if ( opt_t > bd_info.title_count || opt_t < 0)
    {
        fprintf (stderr, "Only %d titles on this disc!\n", bd_info.title_count);
        return 5;
    }

    bd_info.titles = calloc(bd_info.title_count, sizeof(*bd_info.titles));

    for (int i=0; i < bd_info.title_count; i++)
    {
        if (opt_t != i+1 && opt_t != 0)
        {
            continue;
        }

        BLURAY_TITLE_INFO *title_info = bd_get_title_info(bd, i, 0);

        bd_info.titles[i].duration.tv_sec  = (int) title_info->duration / 90000;
        bd_info.titles[i].duration.tv_usec = (int) ((title_info->duration / 90000.0 - bd_info.titles[i].duration.tv_sec) * 1000000);

        bd_info.titles[i].chapter_count = title_info->chapter_count;
        bd_info.titles[i].clip_count    = title_info->clip_count;
        bd_info.titles[i].angle_count   = title_info->angle_count;
        bd_info.titles[i].mark_count    = title_info->mark_count;
    }

    print_human_readable(&bd_info);

    free(bd_info.titles);

    bd_close(bd);
}
