#version 330 core

#include "locations.in"
#include "uniform_blocks.in"

out VertexData {
    vec3 normal;
    int boneid;
} vertex;

void main() {

    gl_Position = vec4(position, 1.0);
    
    vertex.normal = normal;
    vertex.boneid = boneid;
}
