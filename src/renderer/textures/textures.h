#pragma once

#include <iostream>

#include <glad/glad.h>
#include "stb_image/stb_image.h"

namespace
{
    class Textures
    {
    public:
        // Usages
        static unsigned int loadTexture2D(const char* File, bool GammaCorrected, bool Flip)
        {
            stbi_set_flip_vertically_on_load(Flip);
            unsigned int Texture;
            glGenTextures(1, &Texture);
            glBindTexture(GL_TEXTURE_2D, Texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int Width, Height, nrComponents;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            unsigned char* data = stbi_load(File, &Width, &Height, &nrComponents, 0);
            if (data)
            {
                GLenum Format;
                if (nrComponents == 1)
                    Format = GL_RED;
                else if (nrComponents == 3)
                    Format = GL_RGB;
                else if (nrComponents == 4)
                    Format = GL_RGBA;

                if (Format == GL_RGB || Format == GL_RED)
                {
                    if (GammaCorrected)
                    {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, Width, Height, 0, Format, GL_UNSIGNED_BYTE, data);
                    }
                    else
                    {
                        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, data);
                    }
                }
                if (Format == GL_RGBA)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, Width, Height, 0, Format, GL_UNSIGNED_BYTE, data);
                }

                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(data);
            }
            else {
                std::cout << "Failed To Load Texture At:" << Texture << std::endl;
                stbi_image_free(data);
            }


            return Texture;
        }

        static void bindTexture2D(unsigned int Texture, unsigned int Slot)
        {
            glActiveTexture(Slot);
            glBindTexture(GL_TEXTURE_2D, Texture);
        }

        static unsigned int loadHDR(const char* File)
        {
            stbi_set_flip_vertically_on_load(true);
            int Width, Height, NrComponents;

            float* data = stbi_loadf(File, &Width, &Height, &NrComponents, 0);
            unsigned int hdrTexture;
            if (data)
            {
                glGenTextures(1, &hdrTexture);
                glBindTexture(GL_TEXTURE_2D, hdrTexture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, data);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed To Load HDR Image" << std::endl;
            }

            return hdrTexture;
        }
    };
}