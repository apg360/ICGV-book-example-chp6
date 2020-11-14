// shader version
#version 420

// input matrix transforms (3D) passed in from our program
layout ( std140, set = 0, binding = 0 ) uniform buffer
{
    mat4 inProjectionMatrix;
    mat4 inViewMatrix;
    mat4 inModelMatrix;
};

// input from your program
layout( location = 0 ) in vec4 inPos;
layout( location = 1 ) in vec3 inColor;

// output to next stage (fragment)
layout( location = 0 ) out vec4 outColor;

// shader entry point
void main()
{
    // combine the matrices
    mat4 mvp = inModelMatrix * inViewMatrix * inProjectionMatrix;
    
    // transform position by matrices
    gl_Position = vec4(inPos.xyz, 1) * mvp;
    
    // pass input color to the next stage
    outColor = vec4(inColor, 1.0);
    // Or a fixed test color (RED)
    // outColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}
