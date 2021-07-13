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

#ifndef _CEGUIOpenGL3Model3DShaderFrag_h_
#define _CEGUIOpenGL3Model3DShaderFrag_h_

namespace CEGUI
{

const char
    Model3DShaderFrag_Model3D[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        "precision highp float;\n"
        #endif
    
        "varying vec3 Normal;\n"
        "varying vec3 FragPos;\n"
        "varying vec2 TexCoords;\n"
        "varying mat3 TBN;\n"

        "struct MaterialProperty {\n"
            "vec4 color;\n"
            "sampler2D texture;\n"
            "bool useTexture;\n"
        "};\n"
    
        "struct Material {\n"
            "MaterialProperty ambient;\n"
            "MaterialProperty diffuse;\n"
            "MaterialProperty specular;\n"
            "MaterialProperty emissive;\n"
            "MaterialProperty normals;\n"
            "float shininess;\n"
            "float strength;\n"
            "float transparent;\n"
        "};\n"
    
    
        "struct PointLight {\n"
            "vec3 position;\n"
    
            "float constant;\n"
            "float linear;\n"
            "float quadratic;\n"
    
            "vec3 ambient;\n"
            "vec3 diffuse;\n"
            "vec3 specular;\n"
        "};\n"
    
        "uniform vec3 viewPos;\n"
        "uniform PointLight pointLights[9];\n"
        "uniform Material material;\n"
        "uniform bool isActived;\n"
        "uniform vec4 colorAttached;\n"
        "uniform int numPointLights;\n"
        "uniform float Uattenuation;\n"

        "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
        "{\n"
            "vec3 ambientTex = texture2D(material.ambient.texture, TexCoords).rgb;\n"
            "vec3 diffuseTex = texture2D(material.diffuse.texture, TexCoords).rgb;\n"
            "vec3 specularTex = texture2D(material.specular.texture, TexCoords).rgb;\n"
    
            "vec3 ambient = ambientTex * float(material.ambient.useTexture) + vec3(material.ambient.color) * float(!material.ambient.useTexture);\n"
            "vec3 diffuse = diffuseTex * float(material.diffuse.useTexture) + vec3(material.diffuse.color) * float(!material.diffuse.useTexture);\n"
            "vec3 specular = specularTex * float(material.specular.useTexture) + vec3(material.specular.color) * float(!material.specular.useTexture);\n"

            "vec3 lightDir = normalize(light.position - fragPos);\n"
            "float diff = max(dot(normal, lightDir), 0.0);\n"
            "vec3 reflectDir = reflect(-lightDir, normal);\n"
            "float spec = 0.0;\n"
            "spec = 5.0*pow(max(dot(viewDir, reflectDir), 0.0), material.shininess == 0.0 ? 0.0 : material.shininess);\n"
//             "vec3 halfwayDir = normalize(lightDir + viewDir);\n"
//             "float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);\n"
//            "float distance = length(light.position - fragPos);\n"
//            "float angle = 1/max(dot(viewDir, reflectDir), 0.0);\n"
//            "float specAttenuation = pow(att.E, -pow((angle/att.U),2.0)/att.D) * att.F * att.U;\n"
//            "spec = spec + specAttenuation;\n"
//            "float attenuation =  pow(att.E, -pow((angle/att.U),2.0)/att.D) * att.F * att.U;\n"
//            "float attenuation =  1/(att.E+att.D*distance+(1/att.U*distance));\n"
//            "float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);\n"
//            "float attenuation = Uattenuation / distance+0.5;\n"
    
            "vec3 ambientCalc = light.ambient * ambient;"
            "vec3 diffuseCalc  = light.diffuse  * diff * diffuse;\n"
            "vec3 specularCalc = light.specular * spec * specular * material.strength;\n"
//            "ambient  *= attenuation;\n"
//            "diffuse  *= attenuation;\n"
//            "specular *= attenuation;\n"
            "return vec3(ambientCalc + diffuseCalc + specularCalc);\n"
    
        "}\n"
    
        "void main(void)\n"
        "{\n"
            "vec3 result = vec3(0.0);\n"
            "vec3 normalColor = texture2D(material.normals.texture, TexCoords).rgb;\n"
            "normalColor = normalize(normalColor * 2.0 - 1.0);\n"
            "vec3 normOrigin = normalize(Normal);\n"
            "vec3 norm = normalize(TBN * normalColor) * float(material.normals.useTexture) + normOrigin * float(!material.normals.useTexture);\n"
            "vec3 viewDir = normalize(viewPos - FragPos);\n"
    
            "float alpha = texture2D(material.diffuse.texture, TexCoords).a * float(material.diffuse.useTexture) + material.transparent * float(!material.diffuse.useTexture);\n"
    
            "for(int i = 0; i < numPointLights; i++)\n"
                "result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n"
    
    
    /*
            "float AmbientOcclusion = 1.0;\n"
            "if (isUseTexMat.AO) AmbientOcclusion = texture2D(texMat.AO, TexCoords).r;"
            "result *= AmbientOcclusion;\n"
     */
            "vec3 emissiveTex = texture2D(material.emissive.texture, TexCoords).rgb;\n"
            "vec3 emissive = emissiveTex * float(material.emissive.useTexture) + vec3(material.emissive.color) * float(!material.emissive.useTexture);\n"
            "result += emissive;\n"
            "gl_FragColor =  vec4(result.x, result.y, result.z, alpha) + colorAttached;\n"
        "}",

    Model3DShaderFrag_Framebuffer[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        "precision highp float;\n"
        #endif
    
        "varying vec2 TexCoords;\n"
    
        "uniform sampler2D screenTexture;\n"
        "void main()\n"
        "{\n"
            "vec3 col = texture2D(screenTexture, TexCoords).rgb;\n"
            "col.x = col.x+0.3;\n"
            "gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);\n"
        "}",
    
    Model3DShaderFrag_GLSL[] =
    
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        "precision highp float;\n"
        #endif

        "varying vec3 Normal;\n"
        "varying vec2 TexCoords;\n"
    
        "uniform sampler2D planeTexture;\n"
        "void main()\n"
        "{\n"
            "vec4 col = texture2D(planeTexture, TexCoords).rgba;\n"
            "vec3 norm = normalize(Normal);\n"
            "gl_FragColor = vec4(col.r, col.g, col.b, col.a);\n;"
        "}\n",
    Model3DShaderFrag_Picking[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        "precision highp float;\n"
        #endif

        "uniform float drawIndex;\n"
        "uniform float objectIndex;\n"

        "void main(void)\n"
        "{\n"
            "gl_FragColor = vec4(1.0, objectIndex, drawIndex, 1.0);\n"
        "}\n",
    Model3DShaderFrag_PostProcess[] =
        #if TARGET_OS_MAC
        "#version 120\n"
        #else
        "#version 100\n"
        "precision highp float;\n"
        #endif
    
        "uniform sampler2D hdrBuffer;\n"
        "uniform float exposure;\n"

        "varying vec2 TexCoords;\n"
        "void main(void)\n"
        "{\n"
            "const float gamma = 2.2;\n"
            "vec3 hdrColor = texture2D(hdrBuffer, TexCoords).rgb;\n"
    
            // reinhard
            //"vec3 result = hdrColor / (hdrColor + vec3(1.0));\n"
            // exposure
            "vec3 result = vec3(1.0) - exp(-hdrColor * exposure);\n"
            // also gamma correct while we're at it
            "result = pow(result, vec3(1.0 / gamma));\n"
            "gl_FragColor = vec4(hdrColor, 1.0f);\n"
        "}\n";
}

#endif
