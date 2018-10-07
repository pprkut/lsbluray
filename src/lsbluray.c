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

#include <string.h>
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

static struct { char code[3];	char name[21];}
language[] = {
    {"aar", "Afar"}, {"abk", "Abkhazian"}, {"ace", "Achinese"}, {"ach", "Acoli"}, {"ada", "Adangme"}, {"ady", "Adyghe"},
    {"afh", "Afrihili"}, {"afr", "Afrikaans"}, {"ain", "Ainu"}, {"aka", "Akan"}, {"akk", "Akkadian"}, {"alb", "Albanian"},
    {"ale", "Aleut"}, {"alt", "Southern Altai"}, {"amh", "Amharic"}, {"anp", "Angika"}, {"ara", "Arabic"}, {"arg", "Aragonese"},
    {"arm", "Armenian"}, {"arn", "Mapudungun"}, {"arp", "Arapaho"}, {"arw", "Arawak"}, {"asm", "Assamese"}, {"ast", "Asturian"},
    {"ava", "Avaric"}, {"ave", "Avestan"}, {"awa", "Awadhi"}, {"aym", "Aymara"}, {"aze", "Azerbaijani"}, {"bak", "Bashkir"},
    {"bal", "Baluchi"}, {"bam", "Bambara"}, {"ban", "Balinese"}, {"baq", "Basque"}, {"bas", "Basa"}, {"bej", "Beja"},
    {"bel", "Belarusian"}, {"bem", "Bemba"}, {"ben", "Bengali"}, {"bho", "Bhojpuri"}, {"bih", "Bihari"}, {"bik", "Bikol"},
    {"bin", "Bini"}, {"bis", "Bislama"}, {"bla", "Siksika"}, {"bod", "Tibetan"}, {"bos", "Bosnian"}, {"bra", "Braj"},
    {"bre", "Breton"}, {"bua", "Buriat"}, {"bug", "Buginese"}, {"bul", "Bulgarian"}, {"bur", "Burmese"}, {"byn", "Blin"},
    {"cad", "Caddo"}, {"car", "Galibi Carib"}, {"cat", "Catalan"}, {"ceb", "Cebuano"}, {"ces", "Czech"}, {"cha", "Chamorro"},
    {"chb", "Chibcha"}, {"che", "Chechen"}, {"chg", "Chagatai"}, {"chi", "Chinese"}, {"chk", "Chuukese"}, {"chm", "Mari"},
    {"chn", "Chinook jargon"}, {"cho", "Choctaw"}, {"chp", "Chipewyan"}, {"chr", "Cherokee"}, {"chu", "Church Slavic"},
    {"chv", "Chuvash"}, {"chy", "Cheyenne"}, {"cop", "Coptic"}, {"cor", "Cornish"}, {"cos", "Corsican"}, {"cre", "Cree"},
    {"crh", "Crimean Tatar"}, {"csb", "Kashubian"}, {"cym", "Welsh"}, {"cze", "Czech"}, {"dak", "Dakota"}, {"dan", "Danish"},
    {"dar", "Dargwa"}, {"del", "Delaware"}, {"deu", "German"}, {"dgr", "Dogrib"}, {"din", "Dinka"}, {"div", "Divehi"},
    {"doi", "Dogri"}, {"dsb", "Lower Sorbian"}, {"dua", "Duala"}, {"dut", "Dutch"}, {"dyu", "Dyula"}, {"dzo", "Dzongkha"},
    {"efi", "Efik"}, {"eka", "Ekajuk"}, {"elx", "Elamite"}, {"eng", "English"}, {"epo", "Esperanto"}, {"est", "Estonian"},
    {"eus", "Basque"}, {"ewe", "Ewe"}, {"ewo", "Ewondo"}, {"fan", "Fang"}, {"fao", "Faroese"}, {"fas", "Persian"}, {"fat", "Fanti"},
    {"fij", "Fijian"}, {"fil", "Filipino"}, {"fin", "Finnish"}, {"fon", "Fon"}, {"fra", "French"}, {"fre", "French"},
    {"frr", "Northern Frisian"}, {"frs", "Eastern Frisian"}, {"fry", "Western Frisian"}, {"ful", "Fulah"}, {"fur", "Friulian"},
    {"gaa", "Ga"}, {"gay", "Gayo"}, {"gba", "Gbaya"}, {"geo", "Georgian"}, {"ger", "German"}, {"gez", "Geez"}, {"gil", "Gilbertese"},
    {"gla", "Gaelic"}, {"gle", "Irish"}, {"glg", "Galician"}, {"glv", "Manx"}, {"gon", "Gondi"}, {"gor", "Gorontalo"}, {"got", "Gothic"},
    {"grb", "Grebo"}, {"grn", "Guarani"}, {"gsw", "Swiss German"}, {"guj", "Gujarati"}, {"gwi", "Gwich'in"}, {"hai", "Haida"},
    {"hat", "Haitian"}, {"hau", "Hausa"}, {"haw", "Hawaiian"}, {"heb", "Hebrew"}, {"her", "Herero"}, {"hil", "Hiligaynon"},
    {"him", "Himachali"}, {"hin", "Hindi"}, {"hit", "Hittite"}, {"hmn", "Hmong"}, {"hmo", "Hiri Motu"}, {"hrv", "Croatian"},
    {"hsb", "Upper Sorbian"}, {"hun", "Hungarian"}, {"hup", "Hupa"}, {"hye", "Armenian"}, {"iba", "Iban"}, {"ibo", "Igbo"},
    {"ice", "Icelandic"}, {"ido", "Ido"}, {"iii", "Sichuan Yi"}, {"ijo", "Ijo languages"}, {"iku", "Inuktitut"}, {"ile", "Interlingue"},
    {"ilo", "Iloko"}, {"ind", "Indonesian"}, {"inh", "Ingush"}, {"ipk", "Inupiaq"}, {"isl", "Icelandic"}, {"ita", "Italian"},
    {"jav", "Javanese"}, {"jbo", "Lojban"}, {"jpn", "Japanese"}, {"jpr", "Judeo-Persian"}, {"jrb", "Judeo-Arabic"},
    {"kaa", "Kara-Kalpak"}, {"kab", "Kabyle"}, {"kac", "Kachin"}, {"kal", "Kalaallisut"}, {"kam", "Kamba"}, {"kan", "Kannada"},
    {"kas", "Kashmiri"}, {"kat", "Georgian"}, {"kau", "Kanuri"}, {"kaw", "Kawi"}, {"kaz", "Kazakh"}, {"kbd", "Kabardian"},
    {"kha", "Khasi"}, {"khm", "Central Khmer"}, {"kho", "Khotanese"}, {"kik", "Kikuyu"}, {"kin", "Kinyarwanda"}, {"kir", "Kirghiz"},
    {"kmb", "Kimbundu"}, {"kok", "Konkani"}, {"kom", "Komi"}, {"kon", "Kongo"}, {"kor", "Korean"}, {"kos", "Kosraean"},
    {"kpe", "Kpelle"}, {"krc", "Karachay-Balkar"}, {"krl", "Karelian"}, {"kru", "Kurukh"}, {"kua", "Kuanyama"}, {"kum", "Kumyk"},
    {"kur", "Kurdish"}, {"kut", "Kutenai"}, {"lad", "Ladino"}, {"lah", "Lahnda"}, {"lam", "Lamba"}, {"lao", "Lao"}, {"lat", "Latin"},
    {"lav", "Latvian"}, {"lez", "Lezghian"}, {"lim", "Limburgan"}, {"lin", "Lingala"}, {"lit", "Lithuanian"}, {"lol", "Mongo"},
    {"loz", "Lozi"}, {"ltz", "Luxembourgish"}, {"lua", "Luba-Lulua"}, {"lub", "Luba-Katanga"}, {"lug", "Ganda"}, {"lui", "Luiseno"},
    {"lun", "Lunda"}, {"luo", "Luo"}, {"lus", "Lushai"}, {"mac", "Macedonian"}, {"mad", "Madurese"}, {"mag", "Magahi"},
    {"mah", "Marshallese"}, {"mai", "Maithili"}, {"mak", "Makasar"}, {"mal", "Malayalam"}, {"man", "Mandingo"}, {"mao", "Maori"},
    {"mar", "Marathi"}, {"mas", "Masai"}, {"may", "Malay"}, {"mdf", "Moksha"}, {"mdr", "Mandar"}, {"men", "Mende"},
    {"mic", "Mi'kmaq"}, {"min", "Minangkabau"}, {"mkd", "Macedonian"}, {"mlg", "Malagasy"}, {"mlt", "Maltese"}, {"mnc", "Manchu"},
    {"mni", "Manipuri"}, {"moh", "Mohawk"}, {"mol", "Moldavian"}, {"mon", "Mongolian"}, {"mos", "Mossi"}, {"mri", "Maori"},
    {"msa", "Malay"}, {"mul", "Multiple languages"}, {"mus", "Creek"}, {"mwl", "Mirandese"}, {"mwr", "Marwari"}, {"mya", "Burmese"},
    {"myv", "Erzya"}, {"nap", "Neapolitan"}, {"nau", "Nauru"}, {"nav", "Navajo"}, {"nbl", "Ndebele, South"}, {"nde", "Ndebele, North"},
    {"ndo", "Ndonga"}, {"nds", "Low German"}, {"nep", "Nepali"}, {"new", "Nepal Bhasa"}, {"nia", "Nias"}, {"niu", "Niuean"},
    {"nld", "Dutch"}, {"nno", "Norwegian Nynorsk"}, {"nob", "Bokmal, Norwegian"}, {"nog", "Nogai"}, {"nor", "Norwegian"},
    {"nqo", "N'Ko"}, {"nso", "Pedi"}, {"nya", "Chichewa"}, {"nym", "Nyamwezi"}, {"nyn", "Nyankole"}, {"nyo", "Nyoro"}, {"nzi", "Nzima"},
    {"oji", "Ojibwa"}, {"ori", "Oriya"}, {"orm", "Oromo"}, {"osa", "Osage"}, {"oss", "Ossetian"}, {"pag", "Pangasinan"},
    {"pal", "Pahlavi"}, {"pam", "Pampanga"}, {"pan", "Panjabi"}, {"pap", "Papiamento"}, {"pau", "Palauan"}, {"per", "Persian"},
    {"phn", "Phoenician"}, {"pli", "Pali"}, {"pol", "Polish"}, {"pon", "Pohnpeian"}, {"por", "Portuguese"}, {"pus", "Pushto"},
    {"que", "Quechua"}, {"raj", "Rajasthani"}, {"rap", "Rapanui"}, {"rar", "Rarotongan"}, {"roh", "Romansh"}, {"rom", "Romany"},
    {"ron", "Romanian"}, {"rum", "Romanian"}, {"run", "Rundi"}, {"rup", "Aromanian"}, {"rus", "Russian"}, {"sad", "Sandawe"},
    {"sag", "Sango"}, {"sah", "Yakut"}, {"sam", "Samaritan Aramaic"}, {"san", "Sanskrit"}, {"sas", "Sasak"}, {"sat", "Santali"},
    {"scn", "Sicilian"}, {"sco", "Scots"}, {"sel", "Selkup"}, {"shn", "Shan"}, {"sid", "Sidamo"}, {"sin", "Sinhala"},
    {"slk", "Slovak"}, {"slo", "Slovak"}, {"slv", "Slovenian"}, {"sma", "Southern Sami"}, {"sme", "Northern Sami"},
    {"smj", "Lule Sami"}, {"smn", "Inari Sami"}, {"smo", "Samoan"}, {"sms", "Skolt Sami"}, {"sna", "Shona"}, {"snd", "Sindhi"},
    {"snk", "Soninke"}, {"sog", "Sogdian"}, {"som", "Somali"}, {"sot", "Sotho, Southern"}, {"spa", "Spanish"}, {"sqi", "Albanian"},
    {"srd", "Sardinian"}, {"srn", "Sranan Tongo"}, {"srp", "Serbian"}, {"srr", "Serer"}, {"ssw", "Swati"}, {"suk", "Sukuma"},
    {"sun", "Sundanese"}, {"sus", "Susu"}, {"sux", "Sumerian"}, {"swa", "Swahili"}, {"swe", "Swedish"}, {"syr", "Syriac"},
    {"tah", "Tahitian"}, {"tam", "Tamil"}, {"tat", "Tatar"}, {"tel", "Telugu"}, {"tem", "Timne"}, {"ter", "Tereno"}, {"tet", "Tetum"},
    {"tgk", "Tajik"}, {"tgl", "Tagalog"}, {"tha", "Thai"}, {"tib", "Tibetan"}, {"tig", "Tigre"}, {"tir", "Tigrinya"}, {"tiv", "Tiv"},
    {"tkl", "Tokelau"}, {"tlh", "Klingon"}, {"tli", "Tlingit"}, {"tmh", "Tamashek"}, {"tog", "Tonga (Nyasa)"},
    {"ton", "Tonga (Tonga Islands)"}, {"tpi", "Tok Pisin"}, {"tsi", "Tsimshian"}, {"tsn", "Tswana"}, {"tso", "Tsonga"},
    {"tuk", "Turkmen"}, {"tum", "Tumbuka"}, {"tur", "Turkish"}, {"tvl", "Tuvalu"}, {"twi", "Twi"}, {"tyv", "Tuvinian"},
    {"udm", "Udmurt"}, {"uga", "Ugaritic"}, {"uig", "Uighur"}, {"ukr", "Ukrainian"}, {"umb", "Umbundu"}, {"und", "Undetermined"},
    {"urd", "Urdu"}, {"uzb", "Uzbek"}, {"vai", "Vai"}, {"ven", "Venda"}, {"vie", "Vietnamese"}, {"vol", "Volapuk"}, {"vot", "Votic"},
    {"wal", "Wolaitta"}, {"war", "Waray"}, {"was", "Washo"}, {"wel", "Welsh"}, {"wln", "Walloon"}, {"wol", "Wolof"}, {"xal", "Kalmyk"},
    {"xho", "Xhosa"}, {"yao", "Yao"}, {"yap", "Yapese"}, {"yid", "Yiddish"}, {"yor", "Yoruba"}, {"zap", "Zapotec"}, {"zbl", "Blissymbols"},
    {"zen", "Zenaga"}, {"zha", "Zhuang"}, {"zho", "Chinese"}, {"zul", "Zulu"}, {"zun", "Zuni"}, {"zxx", "No linguistic content"},
    {"zza", "Zaza"}
};

const char *video_type[5] = {"MPEG1", "MPEG2", "VC1", "H.264", "Unknown"};
const char *aspect_ratio[3] = {"4/3", "16/9", "Unknown"};
const char *resolution[8] = {"480i", "480p", "576i", "576p", "720p", "1080i", "1080p", "Unknown"};
const char *video_rate[7] = {"23.976", "24", "25", "29.97", "50", "59.94", "Unknown"};

const char *audio_type[10] = {"MPEG1", "MPEG2", "LPCM", "AC3", "DTS", "TrueHD", "E-AC3", "DTS-HD", "DTS-HD MA", "Unknown"};
const char *audio_samplerate[4] = {"48000", "96000", "192000", "Unknown"};
const char *audio_channels[4] = {"Mono", "Stereo", "Multi-Channel", "Unknown"};

char* program_name;

int opt_c = 0, opt_d = 0, opt_t = 0;

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
    printf("\t  -c chapters\n");
    printf("\t  -d clips\n");
    printf("\n");
    printf("\tOther options:\n");
    printf("\t  -h this message\n");
    printf("\t  -V version information\n");
    printf("\n");
}

static char* get_language_name(const char* code)
{
    int i=0;
    while (memcmp(language[i].code, code, 3) && language[i].name[0])
    {
        i++;
    }

    return language[i].name;
}

static time_t get_seconds(uint64_t duration)
{
    return (int) duration / 90000;
}

static suseconds_t get_usceconds(uint64_t duration)
{
    return (int) ((duration / 90000.0 - get_seconds(duration)) * 1000000);
}

int main(int argc, char *argv[])
{

    int c;
    char *bluray_device = DEFAULT_BLURAY_DEVICE;
    struct stat device_stat;
    int ret = 0;

    program_name = argv[0];

    while ((c = getopt(argc, argv, "hV?t:dc")) != EOF)
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
            case 'c':
                opt_c = 1;
                break;
            case 'd':
                opt_d = 1;
                break;
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

        bd_info.titles[i].duration.tv_sec  = get_seconds(title_info->duration);
        bd_info.titles[i].duration.tv_usec = get_usceconds(title_info->duration);

        bd_info.titles[i].chapter_count = title_info->chapter_count;
        bd_info.titles[i].clip_count    = title_info->clip_count;
        bd_info.titles[i].angle_count   = title_info->angle_count;
        bd_info.titles[i].mark_count    = title_info->mark_count;

        if (opt_d == 1)
        {
            bd_info.titles[i].clips = calloc(bd_info.titles[i].clip_count, sizeof(*bd_info.titles[i].clips));

            for (int j=0; j < bd_info.titles[i].clip_count; j++)
            {
                bd_info.titles[i].clips[j].video_count     = title_info->clips[j].video_stream_count;
                bd_info.titles[i].clips[j].audio_count     = title_info->clips[j].audio_stream_count;
                bd_info.titles[i].clips[j].pg_count        = title_info->clips[j].pg_stream_count;
                bd_info.titles[i].clips[j].ig_count        = title_info->clips[j].ig_stream_count;
                bd_info.titles[i].clips[j].sec_video_count = title_info->clips[j].sec_video_stream_count;
                bd_info.titles[i].clips[j].sec_audio_count = title_info->clips[j].sec_audio_stream_count;

                bd_info.titles[i].clips[j].duration.tv_sec  = get_seconds(title_info->clips[j].out_time - title_info->clips[j].in_time);
                bd_info.titles[i].clips[j].duration.tv_usec = get_usceconds(title_info->clips[j].out_time - title_info->clips[j].in_time);

                bd_info.titles[i].clips[j].video_streams = calloc(bd_info.titles[i].clips[j].video_count, sizeof(*bd_info.titles[i].clips[j].video_streams));
                bd_info.titles[i].clips[j].audio_streams = calloc(bd_info.titles[i].clips[j].audio_count, sizeof(*bd_info.titles[i].clips[j].audio_streams));
                bd_info.titles[i].clips[j].subtitle_streams = calloc(bd_info.titles[i].clips[j].pg_count, sizeof(*bd_info.titles[i].clips[j].subtitle_streams));

                for (int k=0; k < bd_info.titles[i].clips[j].video_count; k++)
                {
                    switch (title_info->clips[j].video_streams[k].coding_type)
                    {
                        case BLURAY_STREAM_TYPE_VIDEO_MPEG1:
                            bd_info.titles[i].clips[j].video_streams[k].format = video_type[0];
                            break;
                        case BLURAY_STREAM_TYPE_VIDEO_MPEG2:
                            bd_info.titles[i].clips[j].video_streams[k].format = video_type[1];
                            break;
                        case BLURAY_STREAM_TYPE_VIDEO_VC1:
                            bd_info.titles[i].clips[j].video_streams[k].format = video_type[2];
                            break;
                        case BLURAY_STREAM_TYPE_VIDEO_H264:
                            bd_info.titles[i].clips[j].video_streams[k].format = video_type[3];
                            break;
                        default:
                            bd_info.titles[i].clips[j].video_streams[k].format = video_type[4];
                            break;
                    }

                    switch (title_info->clips[j].video_streams[k].aspect)
                    {
                        case BLURAY_ASPECT_RATIO_4_3:
                            bd_info.titles[i].clips[j].video_streams[k].aspect_ratio = aspect_ratio[0];
                            break;
                        case BLURAY_ASPECT_RATIO_16_9:
                            bd_info.titles[i].clips[j].video_streams[k].aspect_ratio = aspect_ratio[1];
                            break;
                        default:
                            bd_info.titles[i].clips[j].video_streams[k].aspect_ratio = aspect_ratio[2];
                            break;
                    }

                    switch (title_info->clips[j].video_streams[k].format)
                    {
                        case BLURAY_VIDEO_FORMAT_480I:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[0];
                            break;
                        case BLURAY_VIDEO_FORMAT_480P:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[1];
                            break;
                        case BLURAY_VIDEO_FORMAT_576I:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[2];
                            break;
                        case BLURAY_VIDEO_FORMAT_576P:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[3];
                            break;
                        case BLURAY_VIDEO_FORMAT_720P:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[4];
                            break;
                        case BLURAY_VIDEO_FORMAT_1080I:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[5];
                            break;
                        case BLURAY_VIDEO_FORMAT_1080P:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[6];
                            break;
                        default:
                            bd_info.titles[i].clips[j].video_streams[k].resolution = resolution[7];
                            break;
                    }

                    switch (title_info->clips[j].video_streams[k].rate)
                    {
                        case BLURAY_VIDEO_RATE_24000_1001:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[0];
                            break;
                        case BLURAY_VIDEO_RATE_24:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[1];
                            break;
                        case BLURAY_VIDEO_RATE_25:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[2];
                            break;
                        case BLURAY_VIDEO_RATE_30000_1001:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[3];
                            break;
                        case BLURAY_VIDEO_RATE_50:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[4];
                            break;
                        case BLURAY_VIDEO_RATE_60000_1001:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[5];
                            break;
                        default:
                            bd_info.titles[i].clips[j].video_streams[k].fps = video_rate[6];
                            break;
                    }
                }

                for (int k=0; k < bd_info.titles[i].clips[j].audio_count; k++)
                {
                    bd_info.titles[i].clips[j].audio_streams[k].language_code = strdup((char *) title_info->clips[j].audio_streams[k].lang);
                    bd_info.titles[i].clips[j].audio_streams[k].language_name = get_language_name(bd_info.titles[i].clips[j].audio_streams[k].language_code);

                    switch (title_info->clips[j].audio_streams[k].coding_type)
                    {
                        case BLURAY_STREAM_TYPE_AUDIO_MPEG1:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[0];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_MPEG2:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[1];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_LPCM:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[2];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_AC3:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[3];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_DTS:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[4];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_TRUHD:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[5];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_AC3PLUS:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[6];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_DTSHD:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[7];
                            break;
                        case BLURAY_STREAM_TYPE_AUDIO_DTSHD_MASTER:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[8];
                            break;
                        default:
                            bd_info.titles[i].clips[j].audio_streams[k].format = audio_type[9];
                            break;
                    }

                    switch (title_info->clips[j].audio_streams[k].format)
                    {
                        case BLURAY_AUDIO_FORMAT_MONO:
                            bd_info.titles[i].clips[j].audio_streams[k].channels = audio_channels[0];
                            break;
                        case BLURAY_AUDIO_FORMAT_STEREO:
                            bd_info.titles[i].clips[j].audio_streams[k].channels = audio_channels[1];
                            break;
                        case BLURAY_AUDIO_FORMAT_MULTI_CHAN:
                            bd_info.titles[i].clips[j].audio_streams[k].channels = audio_channels[2];
                            break;
                        default:
                            bd_info.titles[i].clips[j].audio_streams[k].channels = audio_channels[3];
                            break;
                    }

                    switch (title_info->clips[j].audio_streams[k].rate)
                    {
                        case BLURAY_AUDIO_RATE_48:
                            bd_info.titles[i].clips[j].audio_streams[k].samplerate = audio_samplerate[0];
                            break;
                        case BLURAY_AUDIO_RATE_96:
                            bd_info.titles[i].clips[j].audio_streams[k].samplerate = audio_samplerate[1];
                            break;
                        case BLURAY_AUDIO_RATE_192:
                            bd_info.titles[i].clips[j].audio_streams[k].samplerate = audio_samplerate[2];
                            break;
                        default:
                            bd_info.titles[i].clips[j].audio_streams[k].samplerate = audio_samplerate[3];
                            break;
                    }
                }

                for (int k=0; k < bd_info.titles[i].clips[j].pg_count; k++)
                {
                    bd_info.titles[i].clips[j].subtitle_streams[k].language_code = strdup((char *) title_info->clips[j].pg_streams[k].lang);
                    bd_info.titles[i].clips[j].subtitle_streams[k].language_name = get_language_name(bd_info.titles[i].clips[j].subtitle_streams[k].language_code);
                }
            }
        }

        if (opt_c == 1)
        {
            bd_info.titles[i].chapters = calloc(bd_info.titles[i].chapter_count, sizeof(*bd_info.titles[i].chapters));

            for (int j=0; j < bd_info.titles[i].chapter_count; j++)
            {
                bd_info.titles[i].chapters[j].duration.tv_sec  = get_seconds(title_info->chapters[j].duration);
                bd_info.titles[i].chapters[j].duration.tv_usec = get_usceconds(title_info->chapters[j].duration);

                bd_info.titles[i].chapters[j].clip = title_info->chapters[j].clip_ref;
            }
        }

        bd_free_title_info(title_info);
    }

    print_human_readable(&bd_info);

    for (int i=0; i < bd_info.title_count; i++)
    {
        if (bd_info.titles[i].clips)
        {
            for (int j=0; j < bd_info.titles[i].clip_count; j++)
            {
                if (bd_info.titles[i].clips[j].video_streams)
                {
                    free(bd_info.titles[i].clips[j].video_streams);
                }

                if (bd_info.titles[i].clips[j].audio_streams)
                {
                    for (int k=0; k < bd_info.titles[i].clips[j].audio_count; k++)
                    {
                        free((char *) bd_info.titles[i].clips[j].audio_streams[k].language_code);
                    }
                    free(bd_info.titles[i].clips[j].audio_streams);
                }

                if (bd_info.titles[i].clips[j].subtitle_streams)
                {
                    for (int k=0; k < bd_info.titles[i].clips[j].pg_count; k++)
                    {
                        free((char *) bd_info.titles[i].clips[j].subtitle_streams[k].language_code);
                    }
                    free(bd_info.titles[i].clips[j].subtitle_streams);
                }
            }

            free(bd_info.titles[i].clips);
        }

        if (bd_info.titles[i].chapters)
        {
            free(bd_info.titles[i].chapters);
        }
    }

    free(bd_info.titles);

    bd_close(bd);
}
