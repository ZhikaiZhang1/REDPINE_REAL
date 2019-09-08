#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pulse/simple.h>
#include <pulse/error.h>
//#define BUFSIZE 1024

pa_simple *pulse_init (uint32_t sample_rate, uint8_t nbr_channels) {
    /* The Sample format to use */
    pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = sample_rate,
        .channels = nbr_channels
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, "default", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        printf(": pa_simple_new() failed: %x\n", error);
        //goto finish;
    }

    return s;

}

#if 0

    FILE *fp;
    fp = fopen ("pcm_samples_a2.bin", "rb");
    for (;;) {
        uint8_t buf[BUFSIZE];
        ssize_t r;
#if 0
        pa_usec_t latency;
        if ((latency = pa_simple_get_latency(s, &error)) == (pa_usec_t) -1) {
            fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(error));
            goto finish;
        }
        fprintf(stderr, "%0.0f usec    \r", (float)latency);
#endif
        /* Read some data ... */
        r = fread(buf, 1, sizeof(buf), fp);
        if (r == 0) /* EOF */
            break;
        else 
          printf ("r:%d, ", r);

        /* ... and play it */
        if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
            //fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            printf ("pa_simple_write() failed: 0x%x\n", error);
            goto finish;
        }
    }
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        //fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    ret = 0;
finish:
    if (s)
        pa_simple_free(s);
    return ret;
}

#endif
