/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/

#ifndef _CEGUIOpenGL3Model3DShaderVert_h_
#define _CEGUIOpenGL3Model3DdShaderVert_h_

namespace CEGUI
{

const char
    Model3DShaderVert_Model3D[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        #endif
        "attribute vec3 aPos;\n"
        "attribute vec3 aNormal;\n"
        "attribute vec2 aTexCoords;\n"
        "attribute vec3 aTangent;\n"
        "attribute vec3 aBitangent;\n"
        "attribute mat3 aBoneIDs;\n"
        "attribute mat3 aBoneWeights;\n"
    
        "varying vec3 Normal;\n"
        "varying vec3 FragPos;\n"
        "varying vec2 TexCoords;\n"
        "varying mat3 TBN;\n"
    
        "uniform mat4 model;\n"
    
        "uniform mat4 global;\n"
        "uniform mat4 world;\n"

        "uniform mat4 bone[100];\n"
    
        "void main(void)\n"
        "{\n"
            "mat4 globalMatrix = mat4(0.0);\n"
            "if (aBoneWeights[0][0] == 0.0) {\n"
            "    globalMatrix = global;\n"
            "}\n"
            "else {\n"
            "    for (int i = 0; i < 3; i++) {\n"
            "        for (int j = 0; j < 3; j++) {\n"
            "            if (aBoneWeights[i][j] == 0.0) continue;\n"
            "            globalMatrix += bone[int(aBoneIDs[i][j])] * aBoneWeights[i][j];\n"
            "        }\n"
            "    }\n"
            "}\n"

            "vec3 T = normalize(vec3(globalMatrix * model * vec4(aTangent, 0.0)));\n"
            "vec3 B = normalize(vec3(globalMatrix * model * vec4(aBitangent, 0.0)));\n"
            "vec3 N = normalize(vec3(globalMatrix * model * vec4(aNormal, 0.0)));\n"
            "TBN = mat3(T, B, N);\n"

            "TexCoords = aTexCoords;\n"
            
            "FragPos = vec3(model * globalMatrix * vec4(aPos, 1.0));\n"
    
            "vec4 NormalL = globalMatrix * vec4(aNormal, 0.0);\n"

            "vec4 finalTrans = globalMatrix * vec4(aPos, 1.0);\n"
    
            "gl_Position = world * finalTrans;\n"
            "Normal = (model * NormalL).xyz;\n"
        "}",

    Model3DShaderVert_Framebuffer[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        #endif
        "attribute vec3 aPos;\n"
        "attribute vec2 aTexCoords;\n"
    
        "varying vec2 TexCoords;\n"
    
        "void main(void)\n"
        "{\n"
            "TexCoords = aTexCoords;\n"
    
            "gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "}",
    
    Model3DShaderVert_GLSL[] =
    
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        #endif
        "attribute vec3 aPos;\n"
        "attribute vec3 aNormal;\n"
        "attribute vec2 aTexCoords;\n"
    
        "varying vec3 Normal;\n"
        "varying vec2 TexCoords;\n"
    
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
    
        "void main()\n"
        "{\n"
            "gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "vec4 NormalL = model * vec4(aNormal, 0.0);\n"
            "Normal = NormalL.xyz;\n"
            "TexCoords = aTexCoords;\n"
        "}\n",
    Model3DShaderVert_Picking[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        #endif
        "attribute vec3 aPos;\n"
    
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 global;\n"
    
        "void main(void)\n"
        "{\n"
            "mat4 world = projection * view * model;\n"
            "vec4 finalTrans = global * vec4(aPos, 1.0);\n"
            "gl_Position = world * finalTrans;\n"
        "}\n",
    Model3DShaderVert_PostProcess[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        #endif
        "attribute vec3 aPos;\n"
        "attribute vec2 aTexCoords;\n"
        "varying vec2 TexCoords;\n"
    
        "void main()\n"
        "{\n"
            "gl_Position = vec4(aPos, 1.0f);\n"
            "TexCoords = aTexCoords;\n"
        "}\n";



}

#endif
