

#include <stdio.h>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_PNG_Image.H>


// Dump the pixel data of a PNG image in C/C++ parsable hex -erco 1.00 07/12/2012
//http://seriss.com/people/erco/fltk/#Fl_PNG_Hex_Dump
int main(int argc, char *argv[]) {
    if (argc < 2 ) {
        printf("Usage: %s file.png\n", argv[0]);
        return(1);
    }
    const char *filename = argv[1];
    fl_register_images();
    Fl_PNG_Image  img(filename);
    // Make a 'safe' filename to show as the C variable
    char *safe_filename = strdup(filename);
    for ( char *s=safe_filename; *s; s++ ) if ( strchr("-_.",*s) ) *s = '_'; 
    // Print C/C++ style variable declaration
    printf("const unsigned char %s[] = {\n", safe_filename);
    printf("/* W=%d H=%d D=%d */\n", img.w(), img.h(), img.d());
    const unsigned char *p = img.array;
    int mod = img.d()==1 ? 16 :
              img.d()==2 ? 8 : 4;
    // Walk the scanlines
    for ( int Y=0; Y<img.h(); Y++ ) {
        printf("/* Y=%d */\n", Y);
        // Walk the pixels
        int X;
        for ( X=0; X<img.w(); X++ ) {
            if ( X % mod == 0 ) printf("\t");
            // Walk the channels
            for ( int D=0; D<img.d(); D++ ) {
                printf("0x%02x,", *p++);
            }
            if ( (X % mod) == (mod-1) ) printf(" /* X=%d-%d */\n", X-(mod-1), X);
            else              printf(" ");
        }
        if ( (--X % mod) != (mod-1) ) printf(" /* X=%d-%d */\n", ((X/mod)*mod),X);
    }
    printf("};\n");
    return(0);
}
