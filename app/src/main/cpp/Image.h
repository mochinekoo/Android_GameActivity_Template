//
// Created by user on 2026/11/08.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_IMAGE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_IMAGE_H


#include <android/asset_manager.h>
#include <GLES3/gl3.h>

class Image {
private:
    GLuint textureID_;
public:

    static Image* Load(AAssetManager* assetManager, const char* path) {
        AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_BUFFER);
        AImageDecoder* imageDecoder = nullptr;
        AImageDecoder_createFromAAsset(asset, &imageDecoder);
        AImageDecoder_setAndroidBitmapFormat(imageDecoder, ANDROID_BITMAP_FORMAT_RGBA_8888);
        const AImageDecoderHeaderInfo* headerInfo = nullptr;
        headerInfo = AImageDecoder_getHeaderInfo(imageDecoder);
        int width = AImageDecoderHeaderInfo_getWidth(headerInfo);
        int height = AImageDecoderHeaderInfo_getHeight(headerInfo);
        unsigned int stride = AImageDecoder_getMinimumStride(imageDecoder);
        std::vector<uint8_t> imageData = std::vector<uint8_t>(height * stride);
        AImageDecoder_decodeImage(imageDecoder, imageData.data(), stride, imageData.size());

        GLuint textureID = 0;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
                GL_TEXTURE_2D, // target
                0, // mip level
                GL_RGBA, // internal format, often advisable to use BGR
                width, // width of the texture
                height, // height of the texture
                0, // border (always 0)
                GL_RGBA, // format
                GL_UNSIGNED_BYTE, // type
                imageData.data() // Data to upload
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        return new Image(textureID);
    };

    Image(GLuint textureID) {
        textureID_ = textureID;
    };

    GLuint getTextureID() const { return textureID_; }
};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_IMAGE_H
