//
//  main.c
//  iwebp
//
//  Created by Takeru Chuganji on 1/23/15.
//  Copyright (c) 2015 Takeru Chuganji. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <webp/decode.h>

static void show_help(void) {
    printf("Usage: iwebp [WEBP_FILE]\n");
}

static int read_webp_file(const char *const file_name, const uint8_t **data, size_t *size) {
    fpos_t file_size;
    FILE *file;
    void *file_data;

    file = fopen(file_name, "rb");
    if (file == NULL) {
        return -1;
    }
    fseek(file, 0, SEEK_END);
    fgetpos(file, &file_size);
    fseek(file, 0, SEEK_SET);

    file_data = malloc((size_t) file_size);
    fread(file_data, (size_t) file_size, 1, file);
    fclose(file);

    *data = file_data;
    *size = (size_t) file_size;
    return 0;
}

static char *bool_to_char(int b) {
    return (b == 0) ? "false" : "true";
}

static int show_webp_info(const char *const file_name) {
    const uint8_t *data;
    size_t size;
    if (read_webp_file(file_name, &data, &size) != 0) {
        printf("IO Error.");
        return -1;
    }
    WebPBitstreamFeatures features;
    if (WebPGetFeatures(data, size, &features) != VP8_STATUS_OK) {
        printf("WebP Error.");
        return -1;
    }
    printf("width: %d\n", features.width);
    printf("height: %d\n", features.height);
    printf("has_alpha: %s\n", bool_to_char(features.has_alpha));
    printf("has_animation: %s\n", bool_to_char(features.has_animation));
    char *format;
    switch (features.format) {
        case 1:
            format = "lossy";
            break;
        case 2:
            format = "lossless";
            break;
        default:
            format = "undefined (/mixed)";
            break;
    }
    printf("format: %s\n", format);
    printf("no_incremental_decoding: %s\n", bool_to_char(features.no_incremental_decoding));
    printf("padding: %d %d\n", features.pad[0], features.pad[1]);
    free((void *) data);
    return 0;
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        show_help();
        return 0;
    }
    const char *file_name = argv[1];
    return show_webp_info(file_name);
}
