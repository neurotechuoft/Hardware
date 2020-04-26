#include <stdio.h>
#include <stdlib.h>
#include <lsl_c.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "mcp3008.h"

#define USAGE "usage: %s [-f freq in hz] [-n stream name] [-u uid] [-t type] "\
              "[0-7] [[0-7]...]\n"

struct _args
{
  unsigned int num_channels;
  int freq;
  const char *name;
  const char *type;
  const char *uid;
  unsigned short channels[];
}
args_struct;

int parseargs(int argc, char* argv[], struct _args *args) {
    int ret = 0;

    // Set some default values
    args->num_channels = 0;
    args->freq = 500;
    args->name = "SendDataC";
    args->uid = "325wqer4354";
    args->type = "EKG";

    int opt;

    // Parse the options passed to the program
    while ((opt = getopt(argc, argv, "f:n:u:t:h")) != -1 && ret == 0) {
        switch (opt) {
            case 'f':
                args->freq = atoi(optarg);
                break;
            case 'n':
                args->name = optarg;
                break;
            case 'u':
                args->uid = optarg;
                break;
            case 't':
                args->type = optarg;
                break;
            case 'h':
            case '?':
                ret = 1;
                break;
        }
    }

    for (int i = optind; i < argc; i++) {
        printf ("%s\n", argv[i]);
        int channel = atoi(argv[i]);
        if (channel >= MCP_NUM_CHANNELS) {
            // Fail when the argument is an invaild channel
            printf ("got here\n");
            ret = 1;
        } else {
            args->channels[args->num_channels] = channel;
            args->num_channels++;
        }
    }

    if (args->num_channels == 0) {
        ret = 1;
    }

    if (ret) {
        fprintf(stderr, USAGE, argv[0]);
    }

    return ret;
}

int runprog(struct _args args) {
    int ret = 0;

    spi s;
    ret = spi_init(&s, "/dev/spidev0.0");
    if (ret) {
        perror("Failed to initialize spi device");
        goto cleanup;
    }

    lsl_streaminfo info = lsl_create_streaminfo(args.name,
                                                args.type,
                                                args.num_channels,
                                                args.freq,
                                                cft_int16,
                                                args.uid);

    // Leaving this here as it might be used later -ECF 2020-04-22
    //lsl_xml_ptr desc = lsl_get_desc(info);

    /* make a new outles (chunking: default, buffering: 360 seconds) */
    lsl_outlet outlet = lsl_create_outlet(info, 0, 360);
    do printf("Waiting for consumers\n");
    while(!lsl_wait_for_consumers(outlet, 120));

    printf("Now sending data...\n");

    /* send data until the last consumer has disconnected */
    while (lsl_have_consumers(outlet)) {
        int cursample[args.num_channels];
        for (int c=0; c < args.num_channels; c++) {
            cursample[c] = spi_getadc(&s, args.channels[c]);
        }
        lsl_push_sample_i(outlet, cursample);
    }
    printf("Lost the last consumer, shutting down\n");
    lsl_destroy_outlet(outlet);

cleanup:
    spi_close(&s);
    return ret;
}

int main(int argc, char* argv[]) {
    struct _args args;
    int ret;
    ret = parseargs(argc, argv, &args);

    if (!ret) {
        ret = runprog(args);
    }

    return ret;
}
