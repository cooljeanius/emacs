/*  -*- Objective-C -*-
 *  main.h
 *  emacs/dummy
 *
 *  Created by Eric Gallager on 11/22/14.
 */

#if (defined(__OBJC__) || defined(__GNUC__) || defined(__APPLE__)) && \
    !defined(__STRICT_ANSI__)
# import <Cocoa/Cocoa.h>
#else
# include <Cocoa/Cocoa.h>
#endif /* (__OBJC__ || __GNUC__ || __APPLE__) && !__STRICT_ANSI__ */

@interface dummy_main_class : NSObject
{
    int dummy_ivar;
}

+ (void)dummy_main_method;

@end

/* EOF */
