#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libheif/heif.h>
#include <jpeglib.h>

int main(int argc, char *argv[]) {
    //----Determine input and output file path from command line arguement----

    // Allocate buffers large enough for filepath + extension
    char input_path[256];
    char output_path[256];

    // Copy argv[1] into the writable buffer
    strcpy(input_path, argv[1]);
    strcpy(output_path, argv[1]);

    // Append extensions
    strcat(input_path, ".heic");
    strcat(output_path, ".jpeg");

    //----Decode HEIC image----

    // Create a pointer to the context struct. This struct acts as the "object"
    // of the heif image, containing all relevant information in memory
    struct heif_context* ctx = heif_context_alloc();

    // Read HEIF image from a file. Set read option to NULL (only option per docs)
    heif_context_read_from_file(ctx, input_path, NULL);

    // From the context we created, access the handle of the image
    // Handle seems to be essentially a pointer to the image data
    struct heif_image_handle* handle;
    // This function assigns the pointer.
    heif_context_get_primary_image_handle(ctx, &handle); // &handle is pointer to pointer. 

    // Decode the image and convert to the raw pixel representation
    struct heif_image* img;
    // Use RGB colorspace and chroma for simplicity
    // Set decoding options to NULL since no cropping or flipping or mirroring
    heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, NULL);

    // Determine width and height for jpeg creation
    // Because of the way we decoded the image, we must use the interleaved channel
    int width  = heif_image_get_width(img, heif_channel_interleaved);
    int height = heif_image_get_height(img, heif_channel_interleaved);

    // Stride is the numeber of bytes that form each row of the image
    int stride;
    // data is the pointer to the first byte in memory where the image is
    // stored in RGB representation
    const uint8_t* data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

    //----Write to JPEG----

    // Create empty jpeg structs
    struct jpeg_compress_struct cinfo; //jpeg object
    struct jpeg_error_mgr jerr; //error handler

    // Create a new file for the output in write mode
    FILE* outfile = fopen(output_path, "w");

    // Initialize error handler, compression object, and file destination
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    // Set image data parameters
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; // R, G, B
    cinfo.in_color_space = JCS_RGB; 

    // Set compression settings to default values
    jpeg_set_defaults(&cinfo); 

    // Begin compression cycle
    jpeg_start_compress(&cinfo, TRUE); // True since complete datastream

    // Loop over each row of the image and write data. cinfo.next_scanline 
    // automatically stores the next row to write as a number. Starts at 0
    // and counts up to the height of the image
    while (cinfo.next_scanline < cinfo.image_height) { // For each row
        // row_pointer is a pointer to the first byte in the scanline (row)
        // At each loop, we make a new pointer by using the the next row 
        // number to write (cinfo.next_scanline) multiplied by the width of 
        // the row
        JSAMPROW row_pointer = (JSAMPROW)(data + cinfo.next_scanline * stride);
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    //----Clean----

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
