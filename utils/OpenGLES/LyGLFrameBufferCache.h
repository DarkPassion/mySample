//
//  LyGLFrameBufferCache.h
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LyGLFrameBuffer.h"

@interface LyGLFrameBufferCache : NSObject

@property (readwrite, weak) LyGLContext* glContext;

- (LyGLFrameBuffer *)fetchFramebufferForSize:(CGSize)framebufferSize;


@end
