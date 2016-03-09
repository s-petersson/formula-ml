#include "Texture.h"

#include <iostream>
#include <fstream>
using namespace std;

struct TargaHeader
{
    unsigned char data1[12];
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char data2;
};

Texture create_texture(GLuint width, GLuint height, unsigned char * pixels, GLuint texture_unit, bool wrap) {
    Texture texture;

    texture.width = width;
    texture.height = height;
    // Set the unique texture unit in which to store the data.
    glActiveTexture(GL_TEXTURE0 + texture_unit);

    // Generate an ID for the texture.
    glGenTextures(1, &texture.id);

    // Bind the texture as a 2D texture.
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // Load the image data into the texture unit.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Set the texture color to either wrap around or clamp to the edge.
    if (wrap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    // Set the texture filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate mipmaps for the texture.
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

void delete_texture(Texture t) {
    glDeleteTextures(1, &t.id);
}

Texture load_tga(const string& loc, GLuint texture_unit, bool wrap) {

    int width, height, bpp, imageSize;
    TargaHeader targaFileHeader;
    unsigned char* targaImage;
    
    //Open a file stream
    std::ifstream file;
    file.open(loc, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("File not found when loading texture: ." + loc);
    }
    file.seekg(0, std::ios::beg);
    file.read((char *)&targaFileHeader, sizeof(TargaHeader));
    
            
    width = (int)targaFileHeader.width;
    height = (int)targaFileHeader.height;
    bpp = (int)targaFileHeader.bpp;

    // Check that it is 32 bit and not 24 bit.
    if (bpp != 32)
    {
        throw std::runtime_error("Image format not supported.");
    }

    // Calculate the size of the 32 bit image data.
    imageSize = width * height * 4;

    // Allocate memory for the targa image data.
    targaImage = new unsigned char[imageSize];
   
    file.read((char*)targaImage, imageSize * sizeof(char));
    
    file.close();
    Texture t = create_texture(width, height, targaImage, texture_unit, wrap);
    delete[] targaImage;
    targaImage = nullptr;
    return t;
}
