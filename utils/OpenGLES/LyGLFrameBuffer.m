//
//  LyGLFrameBuffer.m
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import "LyGLFrameBuffer.h"

@interface LyGLFrameBuffer()
{
    GLuint _framebuffer;
    
    CVOpenGLESTextureRef _renderTexture;
    GLuint _texture;
}

@property (nonatomic, weak) LyGLContext* glContext;


@end

@implementation LyGLFrameBuffer


- (id)initWithSize:(CGSize)framebufferSize glContext: (LyGLContext*) glcontext;
{
    self = [super init];
    if (self) {
        _glContext = glcontext;
        _framebufferSize = framebufferSize;
        [self generateTexture];
    }
    return self;
}


- (void)generateTexture
{
    runSynchronouslyOnGLProcessingQueue(_glContext,  ^{
        [_glContext useImageProcessingContext];
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        
        if ([_glContext supportsFastTextureUpload]){
            CVOpenGLESTextureCacheRef coreVideoTextureCache = [_glContext coreVideoTextureCache];
            // Code originally sourced from http://allmybrain.com/2011/12/08/rendering-to-a-texture-with-ios-5-texture-cache-api/
            
            CFDictionaryRef empty; // empty value for attr value.
            CFMutableDictionaryRef attrs;
            empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks); // our empty IOSurface properties dictionary
            attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
            
            CVReturn err = CVPixelBufferCreate(kCFAllocatorDefault, (int)_framebufferSize.width, (int)_framebufferSize.height, kCVPixelFormatType_32BGRA, attrs, &_renderTarget);
            if (err)
            {
                NSLog(@"FBO size: %f, %f", _framebufferSize.width, _framebufferSize.height);
                NSAssert(NO, @"Error at CVPixelBufferCreate %d", err);
            }
            
            err = CVOpenGLESTextureCacheCreateTextureFromImage (kCFAllocatorDefault, coreVideoTextureCache, _renderTarget,
                                                                NULL, // texture attributes
                                                                GL_TEXTURE_2D,
                                                                GL_RGBA,
                                                                (int)_framebufferSize.width,
                                                                (int)_framebufferSize.height,
                                                                GL_BGRA,
                                                                GL_UNSIGNED_BYTE,
                                                                0,
                                                                &_renderTexture);
            if (err)
            {
                NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
            }
            
            CFRelease(attrs);
            CFRelease(empty);
            
            glBindTexture(CVOpenGLESTextureGetTarget(_renderTexture), CVOpenGLESTextureGetName(_renderTexture));
            _texture = CVOpenGLESTextureGetName(_renderTexture);
            
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CVOpenGLESTextureGetName(_renderTexture), 0);
        }else{
            
            glActiveTexture(GL_TEXTURE1);
            glGenTextures(1, &_texture);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            
            glBindTexture(GL_TEXTURE_2D, _texture);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)_framebufferSize.width, (int)_framebufferSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
        }
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"Incomplete filter FBO: %d", status);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    });

}

- (void)activateFramebuffer
{
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CVOpenGLESTextureGetName(_renderTexture), 0);
    
    glViewport(0, 0, (int)_framebufferSize.width, (int)_framebufferSize.height);
}

- (void)dealloc
{
    [self destroyFramebuffer];
}

- (void)destroyFramebuffer;
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext useImageProcessingContext];
        
        if (_framebuffer)
        {
            glDeleteFramebuffers(1, &_framebuffer);
            _framebuffer = 0;
        }
                
        if (_renderTarget)
        {
            CFRelease(_renderTarget);
            _renderTarget = NULL;
        }
        
        if (_renderTexture)
        {
            CFRelease(_renderTexture);
            _renderTexture = NULL;
        }
        
    });

}
@end
