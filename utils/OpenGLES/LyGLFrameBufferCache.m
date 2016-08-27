//
//  LyGLFrameBufferCache.m
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import "LyGLFrameBufferCache.h"


#define kMAXFramebufferCacheNum 3

static NSString * FrameBufferCachedKeyforSize(CGSize size) {
    NSString * name = [NSString stringWithFormat:@"%.1fx%.1f-NOFB", size.width, size.height];
    return name;
}


@interface LyGLFrameBufferCache()
{
    NSMutableDictionary *_framebufferCache;
}
@end

@implementation LyGLFrameBufferCache

- (id)init{
    self = [super init];
    if (self) {
        _framebufferCache = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (LyGLFrameBuffer *)fetchFramebufferForSize:(CGSize)framebufferSize
{
    NSString *key = FrameBufferCachedKeyforSize(framebufferSize);
    
    LyGLFrameBuffer *framebuffer = [_framebufferCache objectForKey:key];
    
    if (framebuffer) return framebuffer;
    
    // re-new
    framebuffer = [[LyGLFrameBuffer alloc] initWithSize:framebufferSize glContext:_glContext];
    
    if (_framebufferCache.count > kMAXFramebufferCacheNum ) {
        [_framebufferCache removeAllObjects];
    }
    
    [_framebufferCache setObject:framebuffer forKey:key];
    
    return framebuffer;
}

- (void) dealloc
{
    [_framebufferCache removeAllObjects];
    _framebufferCache = nil;
}

@end
