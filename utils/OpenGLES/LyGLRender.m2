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

// 禁止掉未使用变量警告

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"


#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(text) @ STRINGIZE2(text)

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

static const GLfloat rotate0TextureCoordinates[] = {
    0.0f, 0.0f, // lefBottom
    1.0f, 0.0f, // rightBottom
    0.0f, 1.0f, // leftTop
    1.0f, 1.0f, // rightTop
};


static const GLfloat rotate90TextureCoordinates[] = {
    1.0f, 0.0f, // rightBottom
    1.0f, 1.0f, // rightTop
    0.0f, 0.0f, // leftBottom
    0.0f, 1.0f, // leftTop
};

static const GLfloat rotate180TextureCoordinates[] = {
    0.0f, 1.0f, // leftTop
    1.0f, 1.0f, // rightTop
    0.0f, 0.0f, // leftBottom
    1.0f, 0.0f, // rightBottom
};


static const GLfloat rotate270TextureCoordinates[] = {
    1.0f, 1.0f, // rightTop
    1.0f, 0.0f, // rightBottom
    0.0f, 1.0f, // leftTop
    0.0f, 0.0f, // leftBottom
};

static const GLfloat imageVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};



NSString *const kGLRenderVertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 uniform int useGLTransform;
 uniform lowp vec2 transformWH;
 
 
 varying vec2 textureCoordinate;
 
 
 void main()
 {
     if (useGLTransform == 0) {
         gl_Position = vec4(position.xyz, 1.0);
     } else {
         
         vec3 convertMatrix = vec3(position.x * 2.0 / transformWH.x - 1.0, position.y * 2.0 / transformWH.y - 1.0, position.z);
         gl_Position =  vec4(convertMatrix, 1.0);
     }
     
     textureCoordinate = inputTextureCoordinate.xy;
 }
 );

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
    GLint _filterPositionAttribute, _filterTextureCoordinateAttribute;
    GLint _filterInputTextureUniform;
    GLint _filterTransformWithHeight;         // 使用OpenGL 坐标转换 宽 使用OpenGL 坐标转换 高
    GLint _filterUseGLTransformUniform; // 使用OpenGL进行坐标转换
    GLuint _filterTextureGlassesId;
    GLuint _filterTextureEyeFaint;
    GLuint _filterTextureMouthId;
    GLuint _filterTextureMouthBlink;
    
    dispatch_semaphore_t _glRenderSemaphore;
    
}
@end

@implementation LyGLRender

- (id)init
{
    return [self initWithVertexShaderFromString:kGLRenderVertexShaderString fragmentShaderFromString:kGLRenderFragmentShaderString];
}

- (id)initWithFragmentShaderFromString:(NSString *)fragmentShaderString
{
    return [self initWithVertexShaderFromString:kGLRenderVertexShaderString fragmentShaderFromString:fragmentShaderString];
}

- (id)initWithVertexShaderFromString:(NSString *)vertexShaderString fragmentShaderFromString:(NSString *)fragmentShaderString
{
    if (!(self = [super init]))
    {
        return nil;
    }
    
    _glRenderSemaphore = dispatch_semaphore_create(0);
    dispatch_semaphore_signal(_glRenderSemaphore);
    
    runSynchronouslyOnGLProcessingQueue(^{
        [LyGLContext useImageProcessingContext];
        
        _filterProgram = [[LyGLContext sharedGLContext] programForVertexShaderString:vertexShaderString fragmentShaderString:fragmentShaderString];
        
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
        
        glEnable(GL_TEXTURE_2D);
        _filterPositionAttribute = [_filterProgram attributeIndex:@"position"];
        _filterTextureCoordinateAttribute = [_filterProgram attributeIndex:@"inputTextureCoordinate"];
        _filterInputTextureUniform = [_filterProgram uniformIndex:@"inputImageTexture"]; // This does assume a name of "inputImageTexture" for the fragment shader
        _filterTextureGlassesId = 0;
        _filterTextureEyeFaint = 0;
        _filterTextureMouthBlink = 0;
        _filterTextureMouthId = 0;
        
        if (true) {
            NSString* imagepath = [[NSBundle mainBundle] pathForResource:@"yj1" ofType:@"png"];
            int width, height;
            _filterTextureGlassesId = [self setupTexture:imagepath textWidth:&width textHeight:&height];
            
            imagepath = [[NSBundle mainBundle] pathForResource:@"xy" ofType:@"png"];
            
            _filterTextureEyeFaint = [self setupTexture:imagepath textWidth:&width textHeight:&height];
            
            imagepath = [[NSBundle mainBundle] pathForResource:@"zb1" ofType:@"png"];
            
            _filterTextureMouthId = [self setupTexture:imagepath textWidth:&width textHeight:&height];
            
            imagepath = [[NSBundle mainBundle] pathForResource:@"ks" ofType:@"png"];
            _filterTextureMouthBlink = [self setupTexture:imagepath textWidth:&width textHeight:&height];
            
            
            _filterUseGLTransformUniform = [_filterProgram uniformIndex:@"useGLTransform"];
            _filterTransformWithHeight = [_filterProgram uniformIndex:@"transformWH"];
        }
    
        [LyGLContext setActiveShaderProgram:_filterProgram];
        
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


- (GLuint)setupTexture:(NSString *)fileName textWidth:(int*) textWidth textHeight: (int*) textHeight{
    
    // 1
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName);
        return -1;
    }
    
    // 2
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGColorSpaceRef ref = CGImageGetColorSpace(spriteImage);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    

    const size_t components = CGImageGetBitsPerPixel(spriteImage) / CGImageGetBitsPerComponent(spriteImage);

    NSLog(@"zhangzhifan settexture filename %@ ", fileName);
    
    NSLog(@"zhangzhifan setuptexture ref %@ \n components [%zu]", ref, components);

   
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData,
                                                       width,
                                                       height,
                                                       8,
                                                       width*4,
                                                       colorSpace,
                                                       kCGImageAlphaPremultipliedLast);
    
    CGColorSpaceRelease(colorSpace);
    
    // 3
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
  
    // test image format RGBA
    if (false) {
        unsigned char* raw_data = (unsigned char*)spriteData;
        for (int i = 0; i < height / 2; i++) {
            unsigned char* row = raw_data + i * width * 4;
            for (int j = 0; j < width / 2; j++) {
                unsigned char * p = row + j * 4;
                p[0] = 0xFF;
                p[1] = 0xFF;
                p[2] = 0xFF;
                p[4] = 0x00;
            }
        }
    }
    // 4
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    free(spriteData);
    
    if (textWidth) {
        *textWidth = (int)width;
    }
    
    if (textHeight) {
        *textHeight = (int)height;
    }
    
    return texName;
    
}

//主屏渲染
- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer onMainScreen:(CGSize)size
{
    runSynchronouslyOnGLProcessingQueue(^{
        [LyGLContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithPixelBuffer:pixbuffer];
        
        [self renderTexture:aTexture onMainScreen:size];
    });
}

- (void)renderTexture:(LyGL2DTexture *)aTexture onMainScreen:(CGSize)size
{
    runSynchronouslyOnGLProcessingQueue(^{
        [LyGLContext setActiveShaderProgram:_filterProgram];
        
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



- (void) renderMyTexture :(int) width height:(int) height inVertex:(NSArray*) inVertex
{
    NSLog(@"zhangzhifan renderMyTexture inVertex %@ ", inVertex);
    
    BOOL showFace = 0;
    
    const int  bigKey = 0;
    const int  leftKey = 1;
    const int  rightKey = 2;
    const int mouthKey = 3;
    const int mouthWaterKey = 4;
    
    
    // 眼睛外圈points 左边动画points 右边动画points
    CGPoint leftTop[5] = {0};
    CGPoint leftBottom[5] = {0};
    CGPoint rightTop[5] = {0};
    CGPoint rightBottom[5] = {0};
    GLfloat anmiation_roll = 0;
  
    // 口水Y轴抖动
    int mouthWaterOffsetY = 0;
    
    // 脸部旋转角度
    int faceRoll = 0;

    // 眼睛之间间距
    int eyeDistence = 0;
  
    
    CGPoint leftEyeCenter;
    CGPoint rightEyeCenter;
    CGPoint leftMouthWater;
    CGPoint rightMouthWater;
    
    
    for (NSDictionary *dic in inVertex) {
        if ([dic objectForKey:SHOW_FACE_KEY]) {
            showFace = [[dic objectForKey:SHOW_FACE_KEY] intValue];
        }
        
        if ([dic objectForKey:BIG_EYE_POINTS_KEY]) {
            int i = 0;
            for (NSString *strPoints in [dic objectForKey:BIG_EYE_POINTS_KEY]) {
                
                switch (i) {
                    case 0:
                        leftEyeCenter = CGPointFromString(strPoints);
                        break;
                    case 1:
                        rightEyeCenter = CGPointFromString(strPoints);
                        break;
                    default:
                        NSLog(@"error renderPixelBuffer inVertex dic %@", dic);
                        showFace = NO;
                        break;
                }
                i++;
            }
        }
      
        if ([dic objectForKey:MOUTH_POINTS_KEY]) {
            int i = 0;
            for (NSString *strPoints in [dic objectForKey:MOUTH_POINTS_KEY]) {
                
                switch (i) {
                    case 0:
                        leftBottom[mouthKey] = CGPointFromString(strPoints);
                        break;
                    case 1:
                        leftTop[mouthKey] = CGPointFromString(strPoints);
                        break;
                    case 2:
                        rightTop[mouthKey] = CGPointFromString(strPoints);
                        break;
                    case 3:
                        rightBottom[mouthKey] = CGPointFromString(strPoints);
                        break;
                    default:
                        NSLog(@"error renderPixelBuffer inVertex dic %@", [dic objectForKey:MOUTH_POINTS_KEY]);
                        break;
                }
                i++;
            }
        }
        
        if ([dic objectForKey:MOUTH_WATER_POINTS_KEY]) {
            int i = 0;
             for (NSString *strPoints in [dic objectForKey:MOUTH_WATER_POINTS_KEY]) {
                
                 switch (i) {
                     case 0:
                         leftMouthWater = CGPointFromString(strPoints);
                         break;
                    case 1:
                         rightMouthWater = CGPointFromString(strPoints);
                         break;
                     default:
                         NSLog(@"error renderPixelBuffer inVertex dic %@", [dic objectForKey:MOUTH_WATER_POINTS_KEY]);
                         break;
                 }
                 i++;
            }
        }
        
        if ([dic objectForKey:FACE_ROLL_KEY]) {
            faceRoll = [[dic objectForKey:FACE_ROLL_KEY] intValue];
        }
        
        if ([dic objectForKey:EYE_ROLL_ANIMATION_KEY]) {
            anmiation_roll = [[dic objectForKey:EYE_ROLL_ANIMATION_KEY] intValue];
        }
        
        if ([dic objectForKey:EYE_DISTENCE_KEY]) {
            eyeDistence = [[dic objectForKey:EYE_DISTENCE_KEY] intValue];
        }
        
        if ([dic objectForKey:MOUTH_WATER_Y_OFFSET_KEY]) {
            mouthWaterOffsetY = [[dic objectForKey:MOUTH_WATER_Y_OFFSET_KEY] intValue];
        }
    }
    
    
    if (true) {
        
        const float pi7f = 3.1415926;
        
        float aDegree = faceRoll * pi7f / 180.0;
        float psin = sinf(aDegree);
        float pcos = cosf(aDegree);
        
        CGPoint leftEyeCenter2;
        CGPoint rightEyeCenter2;
        
        // 1. 将矩形顶点坐标中心放置在坐标原点
        // 2. 将两个眼睛坐标点放缩成矩形顶点坐
        // 3. 矩形顶点进行坐标系旋转p角度
        // 4. 矩形顶点坐标平移向量qvect
        
        // 平移向量计算
        CGPoint qvect = CGPointMake((leftEyeCenter.x + rightEyeCenter.x) / 2.0, (leftEyeCenter.y + rightEyeCenter.y) / 2.0);

        leftEyeCenter2.x = leftEyeCenter2.y = 0.0f;
        rightEyeCenter2.x = rightEyeCenter2.y = 0.0f;
        
        
        int x_offset = eyeDistence * 1.0;
        int y_offset = eyeDistence / 2.0;
        
        leftBottom[bigKey] = CGPointMake(leftEyeCenter2.x - x_offset, leftEyeCenter2.y - y_offset);
        
        leftTop[bigKey] = CGPointMake(leftEyeCenter2.x - x_offset, leftEyeCenter2.y + y_offset);
        
        rightBottom[bigKey] = CGPointMake(rightEyeCenter2.x + x_offset, rightEyeCenter2.y - y_offset);
        
        rightTop[bigKey] = CGPointMake(rightEyeCenter2.x + x_offset, rightEyeCenter2.y + y_offset);
        
        CGPoint p;
        // 旋转角度
        p = leftBottom[bigKey];
        leftBottom[bigKey].x = p.x * pcos + p.y * psin;
        leftBottom[bigKey].y = p.y * pcos - p.x * psin;
        
        p = rightBottom[bigKey];
        rightBottom[bigKey].x = p.x * pcos + p.y * psin;
        rightBottom[bigKey].y = p.y * pcos - p.x * psin;
        
        p = leftTop[bigKey];
        leftTop[bigKey].x = p.x * pcos + p.y * psin;
        leftTop[bigKey].y = p.y * pcos - p.x * psin;
        
        p = rightTop[bigKey];
        rightTop[bigKey].x = p.x * pcos + p.y * psin;
        rightTop[bigKey].y = p.y * pcos - p.x * psin;
        
        
        // 平移向量
        leftBottom[bigKey].x = leftBottom[bigKey].x + qvect.x;
        leftBottom[bigKey].y = leftBottom[bigKey].y + qvect.y;
        
        rightBottom[bigKey].x = rightBottom[bigKey].x + qvect.x;
        rightBottom[bigKey].y = rightBottom[bigKey].y + qvect.y;
        
        leftTop[bigKey].x = leftTop[bigKey].x + qvect.x;
        leftTop[bigKey].y = leftTop[bigKey].y + qvect.y;
        
        rightTop[bigKey].x = rightTop[bigKey].x + qvect.x;
        rightTop[bigKey].y = rightTop[bigKey].y + qvect.y;
       
        
        // 左右转圈动画
        // leftEye
        leftEyeCenter2.x = leftEyeCenter2.y = 0.0f;
        rightEyeCenter2.x = rightEyeCenter2.y = 0.0f;
        
        
        x_offset = x_offset / 2.8;
        y_offset = y_offset / 2.0;
        
        leftBottom[leftKey] = CGPointMake(leftEyeCenter2.x - x_offset, leftEyeCenter2.y - y_offset);
        
        leftTop[leftKey] = CGPointMake(leftEyeCenter2.x - x_offset, leftEyeCenter2.y + y_offset);
        
        rightBottom[leftKey] = CGPointMake(rightEyeCenter2.x + x_offset, rightEyeCenter2.y - y_offset);
        
        rightTop[leftKey] = CGPointMake(rightEyeCenter2.x + x_offset, rightEyeCenter2.y + y_offset);
        
        // 旋转角度
        p = leftBottom[leftKey];
        leftBottom[leftKey].x = p.x * pcos + p.y * psin;
        leftBottom[leftKey].y = p.y * pcos - p.x * psin;
        
        p = rightBottom[leftKey];
        rightBottom[leftKey].x = p.x * pcos + p.y * psin;
        rightBottom[leftKey].y = p.y * pcos - p.x * psin;
        
        p = leftTop[leftKey];
        leftTop[leftKey].x = p.x * pcos + p.y * psin;
        leftTop[leftKey].y = p.y * pcos - p.x * psin;
        
        p = rightTop[leftKey];
        rightTop[leftKey].x = p.x * pcos + p.y * psin;
        rightTop[leftKey].y = p.y * pcos - p.x * psin;
        
        
        // 赋值
        leftBottom[rightKey] = leftBottom[leftKey];
        leftTop[rightKey] = leftTop[leftKey];
        rightBottom[rightKey] = rightBottom[leftKey];
        rightTop[rightKey] = rightTop[leftKey];
        
        
        // 平移向量
        qvect.x  = leftEyeCenter.x;
        qvect.y = leftEyeCenter.y;
        
        leftBottom[leftKey].x = leftBottom[leftKey].x + qvect.x;
        leftBottom[leftKey].y = leftBottom[leftKey].y + qvect.y;
        
        rightBottom[leftKey].x = rightBottom[leftKey].x + qvect.x;
        rightBottom[leftKey].y = rightBottom[leftKey].y + qvect.y;
        
        leftTop[leftKey].x = leftTop[leftKey].x + qvect.x;
        leftTop[leftKey].y = leftTop[leftKey].y + qvect.y;
        
        rightTop[leftKey].x = rightTop[leftKey].x + qvect.x;
        rightTop[leftKey].y = rightTop[leftKey].y + qvect.y;
        
        // 平移向量
        qvect.x = rightEyeCenter.x;
        qvect.y = rightEyeCenter.y;
        
        leftBottom[rightKey].x = leftBottom[rightKey].x + qvect.x;
        leftBottom[rightKey].y = leftBottom[rightKey].y + qvect.y;
        
        rightBottom[rightKey].x = rightBottom[rightKey].x + qvect.x;
        rightBottom[rightKey].y = rightBottom[rightKey].y + qvect.y;
        
        leftTop[rightKey].x = leftTop[rightKey].x + qvect.x;
        leftTop[rightKey].y = leftTop[rightKey].y + qvect.y;
        
        rightTop[rightKey].x = rightTop[rightKey].x + qvect.x;
        rightTop[rightKey].y = rightTop[rightKey].y + qvect.y;
        
        
        // 嘴巴进行放大
        qvect = CGPointMake((leftBottom[mouthKey].x + rightTop[mouthKey].x) / 2.0, (leftBottom[mouthKey].y + rightTop[mouthKey].y) / 2.0);
        
        leftBottom[mouthKey].x = leftBottom[mouthKey].x - qvect.x;
        leftBottom[mouthKey].y = leftBottom[mouthKey].y - qvect.y;
        
        rightBottom[mouthKey].x = rightBottom[mouthKey].x - qvect.x;
        rightBottom[mouthKey].y = rightBottom[mouthKey].y - qvect.y;
        
        leftTop[mouthKey].x = leftTop[mouthKey].x - qvect.x;
        leftTop[mouthKey].y = leftTop[mouthKey].y - qvect.y;
        
        rightTop[mouthKey].x = rightTop[mouthKey].x - qvect.x;
        rightTop[mouthKey].y = rightTop[mouthKey].y - qvect.y;

        
        // 反方向旋转角度
        p = leftBottom[mouthKey];
        leftBottom[mouthKey].x = p.x * pcos - p.y * psin;
        leftBottom[mouthKey].y = p.y * pcos + p.x * psin;
        
        p = rightBottom[mouthKey];
        rightBottom[mouthKey].x = p.x * pcos - p.y * psin;
        rightBottom[mouthKey].y = p.y * pcos + p.x * psin;
        
        p = leftTop[mouthKey];
        leftTop[mouthKey].x = p.x * pcos - p.y * psin;
        leftTop[mouthKey].y = p.y * pcos + p.x * psin;
        
        p = rightTop[mouthKey];
        rightTop[mouthKey].x = p.x * pcos - p.y * psin;
        rightTop[mouthKey].y = p.y * pcos + p.x * psin;
        
        // 缩放
        x_offset = eyeDistence / 4.0;
        y_offset = eyeDistence / 6.0;
        
        // Y 坐标反相
        leftBottom[mouthKey].x = leftBottom[mouthKey].x - x_offset;
        leftBottom[mouthKey].y = leftBottom[mouthKey].y + y_offset;
        
        leftTop[mouthKey].x = leftTop[mouthKey].x - x_offset;
        leftTop[mouthKey].y = leftTop[mouthKey].y - y_offset;
        
        rightBottom[mouthKey].x = rightBottom[mouthKey].x + x_offset;
        rightBottom[mouthKey].y = rightBottom[mouthKey].y + y_offset;
        
        rightTop[mouthKey].x = rightTop[mouthKey].x + x_offset;
        rightTop[mouthKey].y = rightTop[mouthKey].y - y_offset;
        
        
        // 正方向旋转角度
        p = leftBottom[mouthKey];
        leftBottom[mouthKey].x = p.x * pcos + p.y * psin;
        leftBottom[mouthKey].y = p.y * pcos - p.x * psin;
        
        p = rightBottom[mouthKey];
        rightBottom[mouthKey].x = p.x * pcos + p.y * psin;
        rightBottom[mouthKey].y = p.y * pcos - p.x * psin;
        
        p = leftTop[mouthKey];
        leftTop[mouthKey].x = p.x * pcos + p.y * psin;
        leftTop[mouthKey].y = p.y * pcos - p.x * psin;
        
        p = rightTop[mouthKey];
        rightTop[mouthKey].x = p.x * pcos + p.y * psin;
        rightTop[mouthKey].y = p.y * pcos - p.x * psin;
        
        // 平移向量
        leftBottom[mouthKey].x = leftBottom[mouthKey].x + qvect.x;
        leftBottom[mouthKey].y = leftBottom[mouthKey].y + qvect.y;
        
        rightBottom[mouthKey].x = rightBottom[mouthKey].x + qvect.x;
        rightBottom[mouthKey].y = rightBottom[mouthKey].y + qvect.y;
        
        leftTop[mouthKey].x = leftTop[mouthKey].x + qvect.x;
        leftTop[mouthKey].y = leftTop[mouthKey].y + qvect.y;
        
        rightTop[mouthKey].x = rightTop[mouthKey].x + qvect.x;
        rightTop[mouthKey].y = rightTop[mouthKey].y + qvect.y;
        
        
        // 口水动画
        qvect = CGPointMake( (leftMouthWater.x + rightMouthWater.x) / 2.0, (leftMouthWater.y + rightMouthWater.y) / 2.0 );
        
        x_offset = eyeDistence / 10;
        y_offset = eyeDistence / 3 + mouthWaterOffsetY;
        
        leftBottom[mouthWaterKey] = CGPointMake(leftEyeCenter2.x - x_offset, leftEyeCenter2.y - 20);
        
        leftTop[mouthWaterKey] = CGPointMake(leftEyeCenter2.x - x_offset, leftEyeCenter2.y + y_offset);
        
        rightBottom[mouthWaterKey] = CGPointMake(rightEyeCenter2.x + x_offset, rightEyeCenter2.y - 20);
        
        rightTop[mouthWaterKey] = CGPointMake(rightEyeCenter2.x + x_offset, rightEyeCenter2.y + y_offset);
        
        // 旋转角度
        p = leftBottom[mouthWaterKey];
        leftBottom[mouthWaterKey].x = p.x * pcos + p.y * psin;
        leftBottom[mouthWaterKey].y = p.y * pcos - p.x * psin;
        
        p = rightBottom[mouthWaterKey];
        rightBottom[mouthWaterKey].x = p.x * pcos + p.y * psin;
        rightBottom[mouthWaterKey].y = p.y * pcos - p.x * psin;
        
        p = leftTop[mouthWaterKey];
        leftTop[mouthWaterKey].x = p.x * pcos + p.y * psin;
        leftTop[mouthWaterKey].y = p.y * pcos - p.x * psin;
        
        p = rightTop[mouthWaterKey];
        rightTop[mouthWaterKey].x = p.x * pcos + p.y * psin;
        rightTop[mouthWaterKey].y = p.y * pcos - p.x * psin;
        
        
        // 平移向量
        leftBottom[mouthWaterKey].x = leftBottom[mouthWaterKey].x + qvect.x;
        leftBottom[mouthWaterKey].y = leftBottom[mouthWaterKey].y + qvect.y;
        
        rightBottom[mouthWaterKey].x = rightBottom[mouthWaterKey].x + qvect.x;
        rightBottom[mouthWaterKey].y = rightBottom[mouthWaterKey].y + qvect.y;
        
        leftTop[mouthWaterKey].x = leftTop[mouthWaterKey].x + qvect.x;
        leftTop[mouthWaterKey].y = leftTop[mouthWaterKey].y + qvect.y;
        
        rightTop[mouthWaterKey].x = rightTop[mouthWaterKey].x + qvect.x;
        rightTop[mouthWaterKey].y = rightTop[mouthWaterKey].y + qvect.y;

    }
    
    
   
    
    //        GLfloat imageVertices[8] = {
    //            -1.0f, -1.0f, // leftBottom
    //            1.0f, -1.0f,  // rightBottom
    //            -1.0f,  1.0f, // leftTop
    //            1.0f,  1.0f,  // rightTop
    //        };
    
    if (_filterTextureGlassesId && showFace) {
        
        GLfloat imageVertices[8] = {
            0.0f
        };
        
        GLfloat smallEyeVertex[16] = {
            0.0f
        };
        
        GLfloat mouthVertex[8] = {
            0.0f
        };
        
        GLfloat mouthWaterVertex[8]  = {
            0.0f
        };
      
        if (true) {
        
            imageVertices[0] = leftBottom[bigKey].x;
            imageVertices[1] = leftBottom[bigKey].y;
            
            imageVertices[2] = rightBottom[bigKey].x;
            imageVertices[3] = rightBottom[bigKey].y;
            
            imageVertices[4] = leftTop[bigKey].x;
            imageVertices[5] = leftTop[bigKey].y;
            
            imageVertices[6] = rightTop[bigKey].x;
            imageVertices[7] = rightTop[bigKey].y;
            
            
            smallEyeVertex[0] = leftBottom[leftKey].x;
            smallEyeVertex[1] = leftBottom[leftKey].y;
            
            smallEyeVertex[2] = rightBottom[leftKey].x;
            smallEyeVertex[3] = rightBottom[leftKey].y;
            
            smallEyeVertex[4] = leftTop[leftKey].x;
            smallEyeVertex[5] = leftTop[leftKey].y;
            
            smallEyeVertex[6] = rightTop[leftKey].x;
            smallEyeVertex[7] = rightTop[leftKey].y;
            
            smallEyeVertex[8] = leftBottom[rightKey].x;
            smallEyeVertex[9] = leftBottom[rightKey].y;
            
            smallEyeVertex[10] = rightBottom[rightKey].x;
            smallEyeVertex[11] = rightBottom[rightKey].y;
            
            smallEyeVertex[12] = leftTop[rightKey].x;
            smallEyeVertex[13] = leftTop[rightKey].y;
            
            smallEyeVertex[14] = rightTop[rightKey].x;
            smallEyeVertex[15] = rightTop[rightKey].y;
            
            
            mouthVertex[0] = leftBottom[mouthKey].x;
            mouthVertex[1] = leftBottom[mouthKey].y;
            
            mouthVertex[2] = rightBottom[mouthKey].x;
            mouthVertex[3] = rightBottom[mouthKey].y;
            
            mouthVertex[4] = leftTop[mouthKey].x;
            mouthVertex[5] = leftTop[mouthKey].y;
            
            mouthVertex[6] = rightTop[mouthKey].x;
            mouthVertex[7] = rightTop[mouthKey].y;
            
            mouthWaterVertex[0] = leftBottom[mouthWaterKey].x;
            mouthWaterVertex[1] = leftBottom[mouthWaterKey].y;
            
            mouthWaterVertex[2] = rightBottom[mouthWaterKey].x;
            mouthWaterVertex[3] = rightBottom[mouthWaterKey].y;
            
            mouthWaterVertex[4] = leftTop[mouthWaterKey].x;
            mouthWaterVertex[5] = leftTop[mouthWaterKey].y;
            
            mouthWaterVertex[6] = rightTop[mouthWaterKey].x;
            mouthWaterVertex[7] = rightTop[mouthWaterKey].y;
        }
        
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, _filterTextureGlassesId);
        
        
        glUniform1i(_filterInputTextureUniform, 3);
        
        glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
        glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, rotate0TextureCoordinates);
        

        glUniform1i(_filterUseGLTransformUniform, 1); // 使用OpenGL进行坐标变换
        glUniform2f(_filterTransformWithHeight, width, height); // 配置坐标变换相关信息

        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        
        GLfloat roateTxetCoord[8] = {
            0.0f
        };
        
        if (true) {
            // 顶点坐标
            // 纹理坐标
            // leftBottom rightBottom
            // leftTop  rightTop
            
            const GLfloat rotate0TextureCoordinates[] = {
                0.0f, 0.0f, // lefBottom
                1.0f, 0.0f, // rightBottom
                0.0f, 1.0f, // leftTop
                1.0f, 1.0f, // rightTop
            };
            
            
            const GLfloat rotate90TextureCoordinates[] = {
                1.0f, 0.0f, // rightBottom
                1.0f, 1.0f, // rightTop
                0.0f, 0.0f, // leftBottom
                0.0f, 1.0f, // leftTop
            };
            
            const GLfloat rotate180TextureCoordinates[] = {
                0.0f, 1.0f, // leftTop
                1.0f, 1.0f, // rightTop
                0.0f, 0.0f, // leftBottom
                1.0f, 0.0f, // rightBottom
            };
            
            
            const GLfloat rotate270TextureCoordinates[] = {
                1.0f, 1.0f, // rightTop
                1.0f, 0.0f, // rightBottom
                0.0f, 1.0f, // leftTop
                0.0f, 0.0f, // leftBottom
            };
            
            
            
            if (anmiation_roll == 0) {
                roateTxetCoord[0] = rotate0TextureCoordinates[0];
                roateTxetCoord[1] = rotate0TextureCoordinates[1];
                roateTxetCoord[2] = rotate0TextureCoordinates[2];
                roateTxetCoord[3] = rotate0TextureCoordinates[3];
                roateTxetCoord[4] = rotate0TextureCoordinates[4];
                roateTxetCoord[5] = rotate0TextureCoordinates[5];
                roateTxetCoord[6] = rotate0TextureCoordinates[6];
                roateTxetCoord[7] = rotate0TextureCoordinates[7];
            } else if (anmiation_roll == 1) {
                roateTxetCoord[0] = rotate270TextureCoordinates[0];
                roateTxetCoord[1] = rotate270TextureCoordinates[1];
                roateTxetCoord[2] = rotate270TextureCoordinates[2];
                roateTxetCoord[3] = rotate270TextureCoordinates[3];
                roateTxetCoord[4] = rotate270TextureCoordinates[4];
                roateTxetCoord[5] = rotate270TextureCoordinates[5];
                roateTxetCoord[6] = rotate270TextureCoordinates[6];
                roateTxetCoord[7] = rotate270TextureCoordinates[7];
                
            } else if (anmiation_roll == 2) {
                roateTxetCoord[0] = rotate180TextureCoordinates[0];
                roateTxetCoord[1] = rotate180TextureCoordinates[1];
                roateTxetCoord[2] = rotate180TextureCoordinates[2];
                roateTxetCoord[3] = rotate180TextureCoordinates[3];
                roateTxetCoord[4] = rotate180TextureCoordinates[4];
                roateTxetCoord[5] = rotate180TextureCoordinates[5];
                roateTxetCoord[6] = rotate180TextureCoordinates[6];
                roateTxetCoord[7] = rotate180TextureCoordinates[7];
            } else if (anmiation_roll == 3) {
                roateTxetCoord[0] = rotate90TextureCoordinates[0];
                roateTxetCoord[1] = rotate90TextureCoordinates[1];
                roateTxetCoord[2] = rotate90TextureCoordinates[2];
                roateTxetCoord[3] = rotate90TextureCoordinates[3];
                roateTxetCoord[4] = rotate90TextureCoordinates[4];
                roateTxetCoord[5] = rotate90TextureCoordinates[5];
                roateTxetCoord[6] = rotate90TextureCoordinates[6];
                roateTxetCoord[7] = rotate90TextureCoordinates[7];

            } else {
                roateTxetCoord[0] = rotate0TextureCoordinates[0];
                roateTxetCoord[1] = rotate0TextureCoordinates[1];
                roateTxetCoord[2] = rotate0TextureCoordinates[2];
                roateTxetCoord[3] = rotate0TextureCoordinates[3];
                roateTxetCoord[4] = rotate0TextureCoordinates[4];
                roateTxetCoord[5] = rotate0TextureCoordinates[5];
                roateTxetCoord[6] = rotate0TextureCoordinates[6];
                roateTxetCoord[7] = rotate0TextureCoordinates[7];
                NSLog(@"zhangzhifan roateTxetCoord == ");
            }
            
        }
        
        if (true) {
            // left eye animation
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, _filterTextureEyeFaint);
            glUniform1i(_filterInputTextureUniform, 4);
            
            glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, smallEyeVertex);
           
            glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, roateTxetCoord);
            
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        
        if (true) {
            // right eye animation
            
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, _filterTextureEyeFaint);
            glUniform1i(_filterInputTextureUniform, 5);
            
            glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, &smallEyeVertex[8]);
            glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, roateTxetCoord);
            
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        if (true) {
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, _filterTextureMouthId);
            glUniform1i(_filterInputTextureUniform, 6);
            
            glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, mouthVertex);
            glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, rotate0TextureCoordinates);
            
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        if (true) {
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, _filterTextureMouthBlink);
            glUniform1i(_filterInputTextureUniform, 7);
            
            glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, mouthWaterVertex);
            glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, rotate0TextureCoordinates);
            
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        glDisable(GL_BLEND);
        
    }
    
}

- (void)renderPixelBuffer:(CVPixelBufferRef)pixbuffer atFramebuffer:(LyGLFrameBuffer *)framebuffer inVertex:(NSArray*)inVertex;
{
    runSynchronouslyOnGLProcessingQueue(^{
        [LyGLContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithPixelBuffer:pixbuffer];
        
        [framebuffer activateFramebuffer];
        
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, aTexture.texture);
        glUniform1i(_filterInputTextureUniform, 2);
        
        glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices);
        glVertexAttribPointer(_filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, noRotationTextureCoordinates);
        
        glUniform1i(_filterUseGLTransformUniform, 0);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        [self renderMyTexture:(int)CVPixelBufferGetWidth(pixbuffer) height:(int)CVPixelBufferGetHeight(pixbuffer) inVertex:inVertex];
        glFlush();
    
    });

}

- (CGImageRef)_newCGImageRefAfterRenderingCGImage:(CGImageRef)spriteImage
{
    __block CGImageRef cgImageFromBytes;
    
    runSynchronouslyOnGLProcessingQueue(^{
        [LyGLContext setActiveShaderProgram:_filterProgram];
        
        LyGL2DTexture *aTexture = [[LyGL2DTexture alloc] initWithCGImage:spriteImage];
        
        LyGLFrameBuffer *outputFramebuffer = [[LyGLFrameBufferCache sharedBufferCache] fetchFramebufferForSize:aTexture.texSize];
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
        if ([LyGLContext supportsFastTextureUpload]) {
            CVPixelBufferLockBaseAddress(outputFramebuffer.renderTarget, 0);
            rawImagePixels = (GLubyte *)CVPixelBufferGetBaseAddress(outputFramebuffer.renderTarget);
            dataProvider = CGDataProviderCreateWithData((__bridge_retained void*)outputFramebuffer, rawImagePixels, totalBytesForImage, dataProviderUnlockCallback);

        }else{
            rawImagePixels = (GLubyte *)malloc(totalBytesForImage);
            glReadPixels(0, 0, (int)pixelSize.width, (int)pixelSize.height, GL_RGBA, GL_UNSIGNED_BYTE, rawImagePixels);
            dataProvider = CGDataProviderCreateWithData(NULL, rawImagePixels, totalBytesForImage, dataProviderReleaseCallback);
            
        }
        
        if ([LyGLContext supportsFastTextureUpload])
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
    runSynchronouslyOnGLProcessingQueue(^{
        [LyGLContext setActiveShaderProgram:_filterProgram];
        glUniform1f(uniform, floatValue);
    });
}

- (void) dealloc
{
    if (_filterTextureGlassesId) {
        glDeleteTextures(1, &_filterTextureGlassesId);
    }
    
    _filterTextureGlassesId = 0;
    
    if (_filterTextureEyeFaint) {
        glDeleteTextures(1, &_filterTextureEyeFaint);
    }
    
    _filterTextureEyeFaint = 0;
    
    if (_filterTextureMouthId) {
        glDeleteTextures(1, &_filterTextureMouthId);
    }
    _filterTextureMouthId = 0;
    
    if (_filterTextureMouthBlink) {
        glDeleteTextures(1, &_filterTextureMouthBlink);
    }
    _filterTextureMouthBlink = 0;
    
    
    NSLog(@"lyGlRender dealloc");
}
@end

#pragma clang diagnostic pop
