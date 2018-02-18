/*  -*- Objective-C -*-
 *  main.m
 *  emacs
 *
 *  Created by Eric Gallager on 11/22/14.
 */

#if (defined(__OBJC__) || defined(__GNUC__) || defined(__APPLE__)) && \
    !defined(__STRICT_ANSI__)
# import "main.h"
#else
# include "main.h"
#endif /* (__OBJC__ || __GNUC__ || __APPLE__) && !__STRICT_ANSI__ */

@implementation dummy_main_class

+ (void)dummy_main_method
{
#ifdef DEBUG
    NSLog(@"dummy_main_method called\n");
#endif /* DEBUG */
    return;
}

@end


int main(int argc, char *argv[])
{
#ifdef DEBUG
    printf("Program is running from path '%s' with '%i' argument(s).\n",
           argv[0], argc);
#else
    printf("Hello world.\n");
#endif /* DEBUG */
#ifdef __OBJC__
    [dummy_main_class dummy_main_method];
#endif /* __OBJC__ */
    return NSApplicationMain(argc, (const char **)argv);
}

/* EOF */
