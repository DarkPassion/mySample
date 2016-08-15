//
//  LyGLFrameBuffer.h
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "LyGLContext.h"


@interface LyGLFrameBuffer : NSObject

@property (readonly) GLuint texture;
@property (readonly) CGSize framebufferSize;
@property (readonly) CVPixelBufferRef renderTarget;


- (id)initWithSize:(CGSize)framebufferSize glContext: (LyGLContext*) glcontext;

- (void)activateFramebuffer;

@end
