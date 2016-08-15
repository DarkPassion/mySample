//
//  LyGL2DTexture.h
//  lyOpenGL
//
//  Created by linyu on 3/2/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "LyGLContext.h"

@interface LyGL2DTexture : NSObject

@property (readonly) GLuint texture;
@property (readonly) CGSize texSize;

- (id)initWithPixelBuffer:(CVPixelBufferRef)pixelbuffer glContext:(LyGLContext*) context;

- (id)initWithCGImage:(CGImageRef)imageref glContext:(LyGLContext*) context;


//
- (void)bindTextureWithCGImage:(CGImageRef)imageref;

@end
