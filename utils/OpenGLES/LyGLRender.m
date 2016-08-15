//
//  LyGLRender.m
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import "LyGLRender.h"
#import "LyGLContext.h"
#import "LyGL2DTexture.h"

#import <CoreImage/CoreImage.h>
#import <UIKit/UIKit.h>
static const GLfloat noRotationTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

static const GLfloat verticalFlipTextureCoordinates[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f,  0.0f,
    1.0f,  0.0f,
};

static const GLfloat imageVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};


NSString *const kGLRenderFragmentShaderString = SHADER_STRING
(
 varying highp vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 
 void main()
 {
     gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
 }
 );


void dataProviderReleaseCallback (void *info, const void *data, size_t size)
{
    free((void *)data);
}

void dataProviderUnlockCallback (void *info, const void *data, size_t size)
{
    LyGLFrameBuffer *framebuffer = (__bridge_transfer LyGLFrameBuffer*)info;
    CVPixelBufferLockBaseAddress(framebuffer.renderTarget, 0);
}


@interface LyGLRender()
{
    dispatch_semaphore_t _glRenderSemaphore;
    LyGLFrameBufferCache* _glFrameBufferCache;
    
}
@end

@implementation LyGLRender

- (id)init
{
    
    
    NSString *const kGLRenderVertexShaderString = SHADER_STRING
    (
     attribute vec4 position;
     attribute vec4 inputTextureCoordinate;
     
     varying vec2 textureCoordinate;
     
     void main()
     {
         gl_Position = position;
         textureCoordinate = inputTextureCoordinate.xy;
     }
     );

    
    return [self initWithVertexShaderFromString:kGLRenderVertexShaderString fragmentShaderFromString:kGLRenderFragmentShaderString contexName:@"com.opengles.render"];
}

- (id)initWithVertexShaderFromString:(NSString *)vertexShaderString fragmentShaderFromString:(NSString *)fragmentShaderString contexName:(NSString *)contextNanme
{
    if (!(self = [super init]))
    {
        return nil;
    }
    
    _glRenderSemaphore = dispatch_semaphore_create(0);
    dispatch_semaphore_signal(_glRenderSemaphore);
    
    _glContext = [[LyGLContext alloc] initWithName:contextNanme];
    _glFrameBufferCache = [[LyGLFrameBufferCache alloc] init];
    _glFrameBufferCache.glContext = _glContext;
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext useImageProcessingContext];
        
        _filterProgram = [_glContext programForVertexShaderString:vertexShaderString fragmentShaderString:fragmentShaderString];
        
        if (!_filterProgram.initialized)
        {
            [self initializeAttributes];
            
            if (![_filterProgram link])
            {
                NSString *progLog = [_filterProgram programLog];
                NSLog(@"Program link log: %@", progLog);
                NSString *fragLog = [_filterProgram fragmentShaderLog];
                NSLog(@"Fragment shader compile log: %@", fragLog);
                NSString *vertLog = [_filterProgram vertexShaderLog];
                NSLog(@"Vertex shader compile log: %@", vertLog);
                _filterProgram = nil;
                NSAssert(NO, @"Filter shader link failed");
            }
        }
        
        _filterPositionAttribute = [_filterProgram attributeIndex:@"position"];
        _filterTextureCoordinateAttribute = [_filterProgram attributeIndex:@"inputTextureCoordinate"];
        _filterInputTextureUniform = [_filterProgram uniformIndex:@"inputImageTexture"]; // This does assume a name of "inputImageTexture" for the fragment shader
        
        [_glContext setActiveShaderProgram:_filterProgram];
        
        glEnableVertexAttribArray(_filterPositionAttribute);
        glEnableVertexAttribArray(_filterTextureCoordinateAttribute);
    });
    
    return self;
}

- (void)initializeAttributes;
{
    [_filterProgram addAttribute:@"position"];
    [_filterProgram addAttribute:@"inputTextureCoordinate"];    
    // Override this, calling back to this super method, in order to add new attributes to your vertex shader
}


//主屏渲染
- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer onMainScreen:(CGSize)size
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithPixelBuffer:pixbuffer glContext:_glContext];
        
        [self renderTexture:aTexture onMainScreen:size];
    });
}

- (void)renderTexture:(LyGL2DTexture *)aTexture onMainScreen:(CGSize)size
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        
        glViewport(0, 0, (int)size.width, (int)size.height);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        

        if (aTexture) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, aTexture.texture);
            glUniform1i(_filterInputTextureUniform, 2);
            
            glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
            glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, verticalFlipTextureCoordinates);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        glFlush();
        
    });
    
}

- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer *)framebuffer
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithPixelBuffer:pixbuffer glContext:_glContext];
        
        [framebuffer activateFramebuffer];
        
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, aTexture.texture);
        glUniform1i(_filterInputTextureUniform, 2);
        
        glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
        glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, verticalFlipTextureCoordinates);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glFlush();
    
    });

}



- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer *)framebuffer inVertex:(NSArray*)inVertex;
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithPixelBuffer:pixbuffer glContext:_glContext];
        
        [framebuffer activateFramebuffer];
        
        [self doRender:pixbuffer backgroundTexture:aTexture inVertex:inVertex];
    });
    
}

- (void)offScreenRenderYUV:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer*)framebuffer rawYuv420:(unsigned char*) rawdata;
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithPixelBuffer:pixbuffer glContext:_glContext];
        
        [framebuffer activateFramebuffer];
        
        [self doRender:pixbuffer backgroundTexture:aTexture rawYuv420:rawdata];
        
    });
}


- (CGImageRef)_newCGImageRefAfterRenderingCGImage:(CGImageRef)spriteImage
{
    __block CGImageRef cgImageFromBytes;
    
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithCGImage:spriteImage glContext:_glContext];
        
        LyGLFrameBuffer *outputFramebuffer = [_glFrameBufferCache fetchFramebufferForSize:aTexture.texSize];
        [outputFramebuffer activateFramebuffer];
        
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, aTexture.texture);
        glUniform1i(_filterInputTextureUniform, 2);
        
        glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
        glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glFlush();
        
        GLubyte * rawImagePixels;
        CGDataProviderRef dataProvider;
        CGSize pixelSize = outputFramebuffer.framebufferSize;
        NSUInteger totalBytesForImage = (int)pixelSize.width * (int)pixelSize.height * 4;
        CGColorSpaceRef defaultRGBColorSpace = CGColorSpaceCreateDeviceRGB();
        
        glFinish();
        if ([_glContext supportsFastTextureUpload]) {
            CVPixelBufferLockBaseAddress(outputFramebuffer.renderTarget, 0);
            rawImagePixels = (GLubyte *)CVPixelBufferGetBaseAddress(outputFramebuffer.renderTarget);
            dataProvider = CGDataProviderCreateWithData((__bridge_retained void*)outputFramebuffer, rawImagePixels, totalBytesForImage, dataProviderUnlockCallback);

        }else{
            rawImagePixels = (GLubyte *)malloc(totalBytesForImage);
            glReadPixels(0, 0, (int)pixelSize.width, (int)pixelSize.height, GL_RGBA, GL_UNSIGNED_BYTE, rawImagePixels);
            dataProvider = CGDataProviderCreateWithData(NULL, rawImagePixels, totalBytesForImage, dataProviderReleaseCallback);
            
        }
        
        if ([_glContext supportsFastTextureUpload])
        {
            cgImageFromBytes = CGImageCreate((int)pixelSize.width, (int)pixelSize.height, 8, 32, CVPixelBufferGetBytesPerRow(outputFramebuffer.renderTarget), defaultRGBColorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst, dataProvider, NULL, NO, kCGRenderingIntentDefault);
        }
        else
        {
            cgImageFromBytes = CGImageCreate((int)pixelSize.width, (int)pixelSize.height, 8, 32, 4 * (int)pixelSize.width, defaultRGBColorSpace, kCGBitmapByteOrderDefault | kCGImageAlphaLast, dataProvider, NULL, NO, kCGRenderingIntentDefault);
        }
        
        // Capture image with current device orientation
        CGDataProviderRelease(dataProvider);
        CGColorSpaceRelease(defaultRGBColorSpace);
        
    });
    
    return cgImageFromBytes;
}


- (UIImage *)imageWithRenderingCGImage:(CGImageRef)spriteImage
{
    if (dispatch_semaphore_wait(_glRenderSemaphore, DISPATCH_TIME_FOREVER) != 0){
        return NULL;
    }
    
    CGImageRef cgimageref = [self _newCGImageRefAfterRenderingCGImage:spriteImage];
    
    dispatch_semaphore_signal(_glRenderSemaphore);
    
    UIImage *image = [UIImage imageWithCGImage:cgimageref];
    
    CGImageRelease(cgimageref);
    
    return image;
}


- (void)setFloat:(GLfloat)floatValue forUniform:(GLint)uniform program:(GLProgram *)shaderProgram{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext setActiveShaderProgram:_filterProgram];
        glUniform1f(uniform, floatValue);
    });
}

- (void) doRender:(CVPixelBufferRef)ref backgroundTexture:(LyGL2DTexture *)texture rawYuv420:(unsigned char *)rawdata
{

}

- (void) doRender:(CVPixelBufferRef)ref backgroundTexture:(LyGL2DTexture *)texture inVertex:(NSArray *)inVertex
{

}


- (LyGLFrameBuffer*) fetchFramebufferForSize:(CGSize)framebufferSize
{
    if (_glFrameBufferCache) {
        return [_glFrameBufferCache fetchFramebufferForSize:framebufferSize];
    }
    
    return nil;
}


@end
