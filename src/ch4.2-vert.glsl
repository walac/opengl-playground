#version 410

layout (location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;

out vec4 varColor;

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
    float i = gl_InstanceID + tf;
    float a = sin(0.35 * i) * 8.0;
    float b = sin(0.52 * i) * 8.0;
    float c = sin(0.70 * i) * 8.0;

    mat4 localtRotX = buildRotateX(1.75*i);
    mat4 localtRotY = buildRotateY(1.75*i);
    mat4 localtRotZ = buildRotateZ(1.75*i);
    mat4 localTrans = buildTranslate(a, b, c);

    mat4 newM_matrix = m_matrix * localTrans * localtRotX * localtRotY * localtRotZ;
    mat4 mv_matrix = v_matrix * newM_matrix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.05, 0.5, 0.5);
}
