#ifndef RAINBOW_SHADER_H
#define RAINBOW_SHADER_H

const char *rainbow_frag = "#version 330                    \n"
"in vec2 fragTexCoord;                                      \n"
"in vec4 fragColor;                                         \n"
"out vec4 finalColor;                                       \n"
"uniform sampler2D texture0;                                \n"
"uniform vec2 resolution;                                   \n"
"void main() {                                              \n"
"    vec4 texel = texture(texture0, fragTexCoord);          \n"
"    if (texel.r < 0.5 && texel.g < 0.5 && texel.b < 0.5) { \n"
"        finalColor = vec4(0.0, 0.0, 0.0, 1.0);             \n"
"        return;                                            \n"
"    }                                                      \n"
"    float t = (fragTexCoord.x + fragTexCoord.y) * 0.5;     \n"
"    float r = 0.5 + 0.5 * cos(6.2831 * (t + 0.0));         \n"
"    float g = 0.5 + 0.5 * cos(6.2831 * (t + 0.33));        \n"
"    float b = 0.5 + 0.5 * cos(6.2831 * (t + 0.67));        \n"
"    finalColor = vec4(r, g, b, 1.0);                       \n"
"}";

#endif
