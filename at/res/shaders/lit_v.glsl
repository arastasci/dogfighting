#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    vec4 m = model * vec4(aPos, 1.0);
    FragPos = vec3(m);
    gl_Position = projection * view * m;
    TexCoords = aTexCoords;
    mat3 normalMat = transpose(inverse(mat3(model)));
    Normal  = normalize(normalMat * aNormal);
}

