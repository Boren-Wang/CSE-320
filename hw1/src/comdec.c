#include "const.h"
#include "sequitur.h"
#include "debug.h"
#include "myheader.h"
#include <stdlib.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/**
 * Main compression function.
 * Reads a sequence of bytes from a specified input stream, segments the
 * input data into blocks of a specified maximum number of bytes,
 * uses the Sequitur algorithm to compress each block of input to a list
 * of rules, and outputs the resulting compressed data transmission to a
 * specified output stream in the format detailed in the header files and
 * assignment handout.  The output stream is flushed once the transmission
 * is complete.
 *
 * The maximum number of bytes of uncompressed data represented by each
 * block of the compressed transmission is limited to the specified value
 * "bsize".  Each compressed block except for the last one represents exactly
 * "bsize" bytes of uncompressed data and the last compressed block represents
 * at most "bsize" bytes.
 *
 * @param in  The stream from which input is to be read.
 * @param out  The stream to which the block is to be written.
 * @param bsize  The maximum number of bytes read per block.
 * @return  The number of bytes written, in case of success,
 * otherwise EOF.
 */
int compress(FILE *in, FILE *out, int bsize) {
    // To be implemented.
    return EOF;
}

void recursive_print(SYMBOL *head, FILE *out, int* countp){
    if(head==NULL){
        return;
    }
    if(IS_TERMINAL(head)){
        fputc((char)head->value, out);
        *countp += 1;
        recursive_print(head->next, out, countp);
    } else {
        recursive_print(rule_map[head->value], out, countp);
        recursive_print(head->next, out, countp);
    }
}

/**
 * Main decompression function.
 * Reads a compressed data transmission from an input stream, expands it,
 * and and writes the resulting decompressed data to an output stream.
 * The output stream is flushed once writing is complete.
 *
 * @param in  The stream from which the compressed block is to be read.
 * @param out  The stream to which the uncompressed data is to be written.
 * @return  The number of bytes written, in case of success, otherwise EOF.
 */
int decompress(FILE *in, FILE *out) {
    // To be implemented.
    init_symbols();
    init_rules();

    char c = fgetc(in);
    printf("%c\n", c);

    do{ // new transmission
        c = fgetc(in);
        if(c==0x83){ // new block
            do{
                c = fgetc(in);
                SYMBOL* head= new_rule((int)c);
                c = fgetc(in);
                while(c!=85 && c!=84){
                    add_symbol(head, new_symbol((int)c, NULL));
                }
                add_rule(head);
            }while(c!=0x84);
        }
    }while(c!=0x82);

    int count = 0;
    recursive_print(main_rule, out, &count);
    return count;

    return EOF;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    if(argc==1){ // -s
        return -1;
    }

    if(**(argv+1)=='-' && *(*(argv+1)+1)=='h'){ // s -h
        global_options =  0x1;
        return 0;
    }else if(**(argv+1)=='-' && *(*(argv+1)+1)=='c'){ // s -c
        if(argc==2){ // -c
            global_options = 0x04000002;
            return 0;
        }else if(**(argv+2)=='-' && *(*(argv+2)+1)=='b'){ // s -c -b
            // if(argc>3){
            if(argc==4){
                char* cp = *(argv+3);
                int size = 0;
                while(*cp!='\0'){
                    char c = *cp;
                    if(c<'0' || c>'9'){
                        return -1;
                    } else {
                        size*=10;
                        size+=(*cp-'0');
                    }
                    cp++;
                }
                if(size>-1 && size<1025){
                    size = size<<4;
                    global_options = size + 0x2;
                    return 0;
                } else {
                    return -1;
                }
            }else{
                return -1;
            }
        }else{
            return -1;
        }

    }else if(**(argv+1)=='-' && *(*(argv+1)+1)=='d'){
        if(argc==2){
            global_options= 0x00000004;
            return 0;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}