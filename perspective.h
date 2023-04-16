#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

void setPerspective(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
void setFrustrum(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);

#endif
