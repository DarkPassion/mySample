//
//  LyGLContext.m
//  lyOpenGL
//
//  Created by linyu on 3/1/16.
//  Copyright © 2016 linyu. All rights reserved.
//

#import "LyGLContext.h"

void runSynchronouslyOnGLProcessingQueue(LyGLContext * glctx, void (^block)(void))
{
    EAGLContext* context = [EAGLContext currentContext];
    
    dispatch_queue_t videoProcessingQueue = [glctx contextQueue];
    if (dispatch_get_specific([glctx contextKey])){
        block();
    }else{
        dispatch_sync(videoProcessingQueue, block);
    }
    
    if ([EAGLContext currentContext] != context) {
        [EAGLContext setCurrentContext:context];
    }
}

void runAsynchronouslyOnGLProcessingQueue(LyGLContext * glctx, void (^block)(void))
{
    dispatch_queue_t videoProcessingQueue = [glctx contextQueue];
    if (dispatch_get_specific([glctx contextKey])){
        block();
    }else{
        dispatch_async(videoProcessingQueue, block);
    }
}



@interface LyGLContext()
{
    NSMutableDictionary *_shaderProgramCache;
    void *openGLESContextQueueKey;

}

@end


@implementation LyGLContext

@synthesize context = _context;
@synthesize coreVideoTextureCache = _coreVideoTextureCache;


- (id)init;
{
    if (self = [super init])
    {
        _contextQueue = dispatch_queue_create(NULL, NULL);
        dispatch_queue_set_specific(_contextQueue, &openGLESContextQueueKey, (__bridge void *)self, NULL);
    }
    return self;
}

- (id)initWithName:(NSString *) dispatchname
{
    if (self == [super init])
    {
        _contextQueue = dispatch_queue_create([dispatchname UTF8String], NULL);
        dispatch_queue_set_specific(_contextQueue, &openGLESContextQueueKey, (__bridge void *)self, NULL);

    }
    
    return self;
}

- (dispatch_queue_t)sharedContextQueue;
{
    return self.contextQueue;
}

- (EAGLContext *)context;
{
    if (_context == nil)
    {
        _context = [self createContext];
        [EAGLContext setCurrentContext:_context];
        
        // Set up a few global settings for the image processing pipeline
        glDisable(GL_DEPTH_TEST);
    }
    
    return _context;
}

- (void *)contextKey {
    return &openGLESContextQueueKey;
}

- (EAGLContext *)createContext;
{
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    NSAssert(context != nil, @"Unable to create an OpenGL ES 2.0 context. The GPUImage framework requires OpenGL ES 2.0 support to work.");
    return context;
}

- (GLProgram *)programForVertexShaderString:(NSString *)vertexShaderString fragmentShaderString:(NSString *)fragmentShaderString
{
    NSString *lookupKeyForShaderProgram = [NSString stringWithFormat:@"V: %@ - F: %@", vertexShaderString, fragmentShaderString];
    GLProgram *programFromCache = [_shaderProgramCache objectForKey:lookupKeyForShaderProgram];
    
    if (programFromCache == nil)
    {
        programFromCache = [[GLProgram alloc] initWithVertexShaderString:vertexShaderString fragmentShaderString:fragmentShaderString];
        [_shaderProgramCache setObject:programFromCache forKey:lookupKeyForShaderProgram];
    }
    
    return programFromCache;
}

- (void)setActiveShaderProgram:(GLProgram *)shaderProgram;
{
    [self setContextShaderProgram:shaderProgram];
}

- (void)setContextShaderProgram:(GLProgram *)shaderProgram;
{
    EAGLContext *imageProcessingContext = [self context];
    if ([EAGLContext currentContext] != imageProcessingContext)
    {
        [EAGLContext setCurrentContext:imageProcessingContext];
    }
    
    [shaderProgram use];
   
}

- (void)useImageProcessingContext;
{
    [self useAsCurrentContext];
}

- (BOOL)supportsFastTextureUpload;
{
#if TARGET_IPHONE_SIMULATOR
    return NO;
#else
    
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-pointer-compare"
    return (CVOpenGLESTextureCacheCreate != NULL);
#pragma clang diagnostic pop
    
#endif
}


- (void)useAsCurrentContext;
{
    EAGLContext *imageProcessingContext = [self context];
    if ([EAGLContext currentContext] != imageProcessingContext)
    {
        [EAGLContext setCurrentContext:imageProcessingContext];
    }
}

- (CVOpenGLESTextureCacheRef)coreVideoTextureCache;
{
    if (_coreVideoTextureCache == NULL)
    {
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, [self context], NULL, &_coreVideoTextureCache);
        if (err)
        {
            NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreate %d", err);
        }
    }
    return _coreVideoTextureCache;
}

- (void) dealloc
{
    if (_coreVideoTextureCache) {
        CFRelease(_coreVideoTextureCache);
    }

}

@end
