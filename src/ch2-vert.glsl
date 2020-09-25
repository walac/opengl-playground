#version 410

mat4 buildTranslate(float x, float y, float z) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
          x,   y,   z, 1.0
    );
}

mat4 buildRotateX(float rad) {
    return mat4(
        1.0, 0.0,       0.0,      0.0,
        0.0, cos(rad), -sin(rad), 0.0,
        0.0, sin(rad),  cos(rad), 0.0,
        0.0, 0.0,       0.0,      1.0
    );
}

mat4 buildRotateY(float rad) {
    return mat4(
         cos(rad),  0.0, sin(rad), 0.0,
         0.0,       1.0, 0.0,      0.0,
        -sin(rad),  0.0, cos(rad), 0.0,
         0.0,       0.0, 0.0,      1.0
    );
}

mat4 buildRotateZ(float rad) {
    return mat4(
        cos(rad),  -sin(rad), 0.0, 0.0,
        sin(rad),   cos(rad), 0.0, 0.0,
        0.0,        0.0,      1.0, 0.0,
        0.0,        0.0,      0.0, 1.0
    );
}

mat4 buildScale(float x, float y, float z) {
    return mat4(
          x, 0.0, 0.0, 0.0,
        0.0,   y, 0.0, 0.0,
        0.0, 0.0,   z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

void main(void) {
    if (gl_VertexID == 0) gl_Position = vec4(0.25, -0.25, 0.0, 1.0);
    else if (gl_VertexID == 1) gl_Position = vec4(-0.25, -0.25, 0.0, 1.0);
    else gl_Position = vec4(0.25, 0.25, 0.0, 1.0);
    gl_Position = buildScale(2.0, 2.0, 2.0) * gl_Position;
}
