//
//  LyGLRender.h
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "LyGLFrameBufferCache.h"
#import "LyGL2DTexture.h"
#import "LyGLContext.h"
#import "GLProgram.h"

extern NSString *const kGLRenderFragmentShaderString;

#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(text) @ STRINGIZE2(text)

@interface LyGLRender : NSObject
{
    GLProgram *_filterProgram;
    GLint _filterInputTextureUniform;
    GLint _filterPositionAttribute, _filterTextureCoordinateAttribute;
    LyGLContext* _glContext;
}


// 定义接口 基础类未实现 [变脸礼物 离屏渲染流程]
- (void) doRender : (CVPixelBufferRef) ref backgroundTexture:(LyGL2DTexture*) texture inVertex:(NSArray*) inVertex;

// 定义接口 基础类未实现 [连麦yuv 离屏渲染流程]
- (void) doRender : (CVPixelBufferRef) ref backgroundTexture:(LyGL2DTexture*) texture rawYuv420: (unsigned char*) rawdata;

// 定义接口 基础类未实现 [Urho3d 图像合成]
- (void) doRender : (CVPixelBufferRef) ref backgroundTexture:(LyGL2DTexture*) texture rawRGBA: (unsigned char*) rawdata;

// init
- (id)initWithVertexShaderFromString:(NSString *)vertexShaderString fragmentShaderFromString:(NSString *)fragmentShaderString
        contexName:(NSString*)contextNanme;

- (LyGLFrameBuffer*) fetchFramebufferForSize:(CGSize)framebufferSize;

// 离屏渲染
- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer *)framebuffer;
- (UIImage *)imageWithRenderingCGImage:(CGImageRef)spriteImage;

// 变脸礼物 离屏渲染流程
- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer *)framebuffer inVertex:(NSArray*)inVertex;

// 连麦yuv 离屏渲染流程
- (void)offScreenRenderYUV:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer*)framebuffer rawYuv420:(unsigned char*) rawdata;

// urho3d 图像合成
- (void)offScreenRenderRGBA:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer*)framebuffer rawRGBA:(unsigned char*) rawdata;

//主屏渲染
- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer onMainScreen:(CGSize)size;
- (void)renderTexture:(LyGL2DTexture *)aTexture onMainScreen:(CGSize)size;

@end
