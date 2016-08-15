//
//  LyGLContext.h
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GLProgram.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <AVFoundation/AVFoundation.h>
#import <QuartzCore/QuartzCore.h>

#define glcheck \
do { \
GLenum glerr = glGetError(); \
if (glerr != GL_NO_ERROR) { \
NSLog(@"GLError: %d \n", glerr); \
} \
} while(0)

@interface LyGLContext : NSObject

@property(readonly, strong ,nonatomic) EAGLContext *context;
@property(readonly) CVOpenGLESTextureCacheRef coreVideoTextureCache;
@property(readonly, strong, nonatomic) dispatch_queue_t contextQueue;



- (id)initWithName:(NSString *) dispatchname;

-(void *)contextKey;

-(void)setActiveShaderProgram:(GLProgram *)shaderProgram;

-(void)useImageProcessingContext;

-(BOOL)supportsFastTextureUpload;

- (void)useAsCurrentContext;

- (GLProgram *)programForVertexShaderString:(NSString *)vertexShaderString fragmentShaderString:(NSString *)fragmentShaderString;

@end

void runSynchronouslyOnGLProcessingQueue(LyGLContext * glctx, void (^block)(void));
void runAsynchronouslyOnGLProcessingQueue(LyGLContext * glctx, void (^block)(void));

