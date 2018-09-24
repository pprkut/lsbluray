#ifndef _LSBLURAY_H_
#define _LSBLURAY_H_

#ifdef __linux__
#define DEFAULT_BLURAY_DEVICE "/dev/sr0"
#elif __FreeBSD__ || __DragonFly__ || __NetBSD__ || __OpenBSD__
#define DEFAULT_BLURAY_DEVICE "/dev/cd0"
#elif __APPLE__ && __MACH__
#define DEFAULT_BLURAY_DEVICE "/dev/disk1"
#else
#define DEFAULT_BLURAY_DEVICE "/dev/bluray"
#endif

#include <sys/time.h>
#include <libbluray/bluray.h>

extern char *program_name;

extern int opt_t;

struct bd_info {
    const BLURAY_DISC_INFO *generic;
    int title_count;
    struct {
        struct timeval duration;
        int chapter_count;
        int clip_count;
        int angle_count;
        int mark_count;

        struct {
            int video_count;
            int audio_count;
            int pg_count;
            int ig_count;
            int sec_video_count;
            int sec_audio_count;
        } * clips;
    } *titles;
};

struct duration {
    int hours;
    int minutes;
    int seconds;
    int microseconds;
};

#endif /* _LSBLURAY_H_ */
