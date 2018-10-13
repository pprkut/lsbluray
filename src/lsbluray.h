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

extern int opt_a, opt_c, opt_d, opt_s, opt_t, opt_v, opt_x;

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
            struct timeval duration;
            struct {
                const char *format;
                const char *aspect_ratio;
                const char *resolution;
                const char *fps;
            } *video_streams;
            struct {
                const char *format;
                const char *samplerate;
                const char *channels;
                const char *language_code;
                const char *language_name;
            } *audio_streams;
            struct {
                const char *language_code;
                const char *language_name;
            } *subtitle_streams;
        } *clips;

        struct {
            struct timeval duration;
            unsigned int clip;
        } *chapters;
    } *titles;
};

struct duration {
    int hours;
    int minutes;
    int seconds;
    int microseconds;
};

#endif /* _LSBLURAY_H_ */
