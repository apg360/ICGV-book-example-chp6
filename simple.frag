// shader version
#version 420

// input from our vertex shader
layout( location = 0 ) in vec4 inColor;

// final screen output color
layout( location = 0 ) out vec4 outFragColor;

// shader entry point
void main()
{
    // pass input color along without any modifications
    // e.g., Phong lighting calculations could be done here)
    outFragColor = inColor;
}
