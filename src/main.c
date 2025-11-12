#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libheif/heif.h>
#include <jpeglib.h>

// Determines level of jpeg compression. 90 is a standard value.
#define JPEG_QUALITY 90

int main(int argc, char *argv[]) {
    // Determine input and output file path from command line arguement

    // Allocate buffers large enough for filepath + extension
    char input_path[256];
    char output_path[256];

    // Copy argv[1] into the writable buffer
    strcpy(input_path, argv[1]);
    strcpy(output_path, argv[1]);

    // Append extensions
    strcat(input_path, ".heic");
    strcat(output_path, ".jpeg");

    // Create a pointer to the context struct. This struct acts as the "object"
    // of the heif image, containing all relevant information in memory
    struct heif_context* ctx = heif_context_alloc();
    heif_context_read_from_file(ctx, input_path, NULL);

    // From the context we created, access the handle of the image
    // A handle is similar to a pointer to the image data
    struct heif_image_handle* handle;
    heif_context_get_primary_image_handle(ctx, &handle);

    // Decode the image and convert to an intermediate representation
    struct heif_image* img;
    heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, NULL);

    // Determine width and height for jpeg creation
    int width  = heif_image_get_width(img, heif_channel_interleaved);
    int height = heif_image_get_height(img, heif_channel_interleaved);

    // Stride is the numeber of bytes that form each row of the image
    int stride;
    // data is the pointer to the first byte in memory where the image is
    // stored in RGB representation
    const uint8_t* data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

    // Create empty jpeg structs
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // Create a new file for the output in write binary mode
    FILE* outfile = fopen(output_path, "wb");

    // Initialize error handler, compression object, and file destination
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    // Set image data parameters
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; 
    cinfo.in_color_space = JCS_RGB; 

    // Set compression settings
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE);

    // Prepare to except rows of data
    jpeg_start_compress(&cinfo, TRUE);

    // Loop over each row of the image and write data
    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPROW row_pointer = (JSAMPROW)(data + cinfo.next_scanline * stride);
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    // Clean up jpeg resources
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);

    // Clean up libhief resources
    heif_image_release(img);
    heif_image_handle_release(handle);
    heif_context_free(ctx);

    return 0;
}
