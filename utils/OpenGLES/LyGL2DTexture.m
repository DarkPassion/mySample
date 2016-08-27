//
//  LyGL2DTexture.m
//  lyOpenGL
//
//  Created by linyu on 3/2/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import "LyGL2DTexture.h"
#import "LyGLContext.h"
@import UIKit;

@interface LyGL2DTexture()
{
    CVOpenGLESTextureRef _inputImageTexture;
}

@property (nonatomic, weak)   LyGLContext*    glContext;

@end


@implementation LyGL2DTexture


- (void)bindTextureWithCGImage:(CGImageRef)imageref
{
    size_t width = CGImageGetWidth(imageref);
    size_t height = CGImageGetHeight(imageref);
    
    _texSize = CGSizeMake(width, height);
    [self generateTextureFromCGImage:imageref onlyBind:YES];
}

- (id)initWithPixelBuffer:(CVPixelBufferRef)pixelbuffer glContext:(LyGLContext*) context
{
    self = [super init];
    if (self) {
        _glContext = context;
        size_t width = CVPixelBufferGetWidth(pixelbuffer);
        size_t height = CVPixelBufferGetHeight(pixelbuffer);
        
        _texSize = CGSizeMake(width, height);
        [self generateTextureFromPixelBuffer:pixelbuffer];
    }
    return self;
}

- (id)initWithCGImage:(CGImageRef)imageref glContext:(LyGLContext*) context
{
    self = [super init];
    if (self) {
        _glContext = context;
        size_t width = CGImageGetWidth(imageref);
        size_t height = CGImageGetHeight(imageref);
        
        _texSize = CGSizeMake(width, height);
        [self generateTextureFromCGImage:imageref onlyBind:NO];
    }
    return self;
}


- (void)generateTextureFromCGImage:(CGImageRef)imageref onlyBind:(BOOL)onlybind
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext useImageProcessingContext];
        
        size_t width = CGImageGetWidth(imageref);
        size_t height = CGImageGetHeight(imageref);
        
        GLubyte * spriteData = NULL;
        CFDataRef dataFromImageDataProvider = NULL;
        GLenum format = GL_BGRA;
        BOOL shouldRedrawUsingCoreGraphics = NO;
        if (CGImageGetBytesPerRow(imageref) != CGImageGetWidth(imageref) * 4 ||
            CGImageGetBitsPerPixel(imageref) != 32 ||
            CGImageGetBitsPerComponent(imageref) != 8)
        {
            shouldRedrawUsingCoreGraphics = YES;
        } else {
            /* Check that the bitmap pixel format is compatible with GL */
            CGBitmapInfo bitmapInfo = CGImageGetBitmapInfo(imageref);
            if ((bitmapInfo & kCGBitmapFloatComponents) != 0) {
                /* We don't support float components for use directly in GL */
                shouldRedrawUsingCoreGraphics = YES;
            } else {
                CGBitmapInfo byteOrderInfo = bitmapInfo & kCGBitmapByteOrderMask;
                if (byteOrderInfo == kCGBitmapByteOrder32Little) {
                    /* Little endian, for alpha-first we can use this bitmap directly in GL */
                    CGImageAlphaInfo alphaInfo = bitmapInfo & kCGBitmapAlphaInfoMask;
                    if (alphaInfo != kCGImageAlphaPremultipliedFirst && alphaInfo != kCGImageAlphaFirst &&
                        alphaInfo != kCGImageAlphaNoneSkipFirst) {
                        shouldRedrawUsingCoreGraphics = YES;
                    }
                } else if (byteOrderInfo == kCGBitmapByteOrderDefault || byteOrderInfo == kCGBitmapByteOrder32Big) {
                    /* Big endian, for alpha-last we can use this bitmap directly in GL */
                    CGImageAlphaInfo alphaInfo = bitmapInfo & kCGBitmapAlphaInfoMask;
                    if (alphaInfo != kCGImageAlphaPremultipliedLast && alphaInfo != kCGImageAlphaLast &&
                        alphaInfo != kCGImageAlphaNoneSkipLast) {
                        shouldRedrawUsingCoreGraphics = YES;
                    } else {
                        /* Can access directly using GL_RGBA pixel format */
                        format = GL_RGBA;
                    }
                }
            }
        }
        
        
        
        if (shouldRedrawUsingCoreGraphics){
            // For resized or incompatible image: redraw
            spriteData = (GLubyte *) calloc(1, (width * height * 4));
            CGColorSpaceRef genericRGBColorspace = CGColorSpaceCreateDeviceRGB();
            CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, genericRGBColorspace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
            CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, width, height), imageref);
            
            CGContextRelease(spriteContext);
            CGColorSpaceRelease(genericRGBColorspace);
        }else{
            
            dataFromImageDataProvider = CGDataProviderCopyData(CGImageGetDataProvider(imageref));
            spriteData = (GLubyte *)CFDataGetBytePtr(dataFromImageDataProvider);
        }
        
        if (onlybind == NO) {
            glGenTextures(1, &_texture);
        }
        
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)width, (int)height, 0, format, GL_UNSIGNED_BYTE, spriteData);
        
        if (shouldRedrawUsingCoreGraphics)
        {
            free(spriteData);
        }
        else
        {
            if (dataFromImageDataProvider)
            {
                CFRelease(dataFromImageDataProvider);
            }
        }
    });
}


- (void)generateTextureFromPixelBuffer:(CVPixelBufferRef)pixelbuffer
{
    runSynchronouslyOnGLProcessingQueue(_glContext, ^{
        [_glContext useImageProcessingContext];
        
        CVOpenGLESTextureCacheRef coreVideoTextureCache = [_glContext coreVideoTextureCache];
        
        if (_inputImageTexture)
        {
            CFRelease(_inputImageTexture);
            _inputImageTexture = NULL;
        }
        CVOpenGLESTextureCacheFlush(coreVideoTextureCache, 0);
        
        CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                                    coreVideoTextureCache,
                                                                    pixelbuffer,
                                                                    NULL,
                                                                    GL_TEXTURE_2D,
                                                                    GL_RGBA,
                                                                    _texSize.width,
                                                                    _texSize.height,
                                                                    GL_BGRA,
                                                                    GL_UNSIGNED_BYTE,
                                                                    0,
                                                                    &_inputImageTexture);
        
        if (err)
        {
            NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
        }
        
        _texture = CVOpenGLESTextureGetName(_inputImageTexture);
        
        glBindTexture(CVOpenGLESTextureGetTarget(_inputImageTexture), CVOpenGLESTextureGetName(_inputImageTexture));
        _texture = CVOpenGLESTextureGetName(_inputImageTexture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    });
}

-(void)dealloc{
    [self destoryTexture];
}

- (void)destoryTexture
{
    if (_inputImageTexture) {
        CFRelease(_inputImageTexture);
        _inputImageTexture = NULL;
    }else if (_texture){
        glDeleteTextures(1, &_texture);
    }
}


//- (CVPixelBufferRef)pixelBufferFromCGImage:(CGImageRef)imageref{
//
//    CVPixelBufferRef pxbuffer = NULL;
//    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
//                             [NSNumber numberWithBool:YES], kCVPixelBufferCGImageCompatibilityKey,
//                             [NSNumber numberWithBool:YES], kCVPixelBufferCGBitmapContextCompatibilityKey,
//                             nil];
//
//    size_t width =  CGImageGetWidth(imageref);
//    size_t height = CGImageGetHeight(imageref);
//    size_t bytesPerRow = CGImageGetBytesPerRow(imageref);
//
//    CFDataRef  dataFromImageDataProvider = CGDataProviderCopyData(CGImageGetDataProvider(imageref));
//    GLubyte  *imageData = (GLubyte *)CFDataGetBytePtr(dataFromImageDataProvider);
//
//    CVPixelBufferCreateWithBytes(kCFAllocatorDefault,width,height,kCVPixelFormatType_32ARGB,imageData,bytesPerRow,NULL,NULL,(__bridge CFDictionaryRef)options,&pxbuffer);
//
//    CFRelease(dataFromImageDataProvider);
//
//    return pxbuffer;
//}
@end
