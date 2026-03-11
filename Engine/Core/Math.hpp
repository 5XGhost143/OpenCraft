#pragma once
#include "Types.hpp"
#include <cstring>
#include <cmath>

struct Mat4 {
    float m[16] = {};

    static Mat4 identity() {
        Mat4 r; r.m[0]=r.m[5]=r.m[10]=r.m[15]=1.f; return r;
    }

    static Mat4 perspective(float fovY, float aspect, float near, float far) {
        Mat4 r;
        float f = 1.f / std::tan(fovY * 0.5f);
        r.m[0]  = f / aspect;
        r.m[5]  = f;
        r.m[10] = (far + near) / (near - far);
        r.m[11] = -1.f;
        r.m[14] = (2.f * far * near) / (near - far);
        return r;
    }

    static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {
        Vec3 f = (center - eye).normalized();
        Vec3 r = f.cross(up).normalized();
        Vec3 u = r.cross(f);
        Mat4 res = identity();
        res.m[0]=r.x; res.m[4]=r.y; res.m[8] =r.z;
        res.m[1]=u.x; res.m[5]=u.y; res.m[9] =u.z;
        res.m[2]=-f.x;res.m[6]=-f.y;res.m[10]=-f.z;
        res.m[12]=-r.dot(eye);
        res.m[13]=-u.dot(eye);
        res.m[14]= f.dot(eye);
        return res;
    }

    static Mat4 translation(Vec3 t) {
        Mat4 r = identity();
        r.m[12]=t.x; r.m[13]=t.y; r.m[14]=t.z;
        return r;
    }

    static Mat4 scale(Vec3 s) {
        Mat4 r = identity();
        r.m[0]=s.x; r.m[5]=s.y; r.m[10]=s.z;
        return r;
    }

    Mat4 operator*(const Mat4& b) const {
        Mat4 res;
        for (int col=0;col<4;++col)
            for (int row=0;row<4;++row) {
                float sum=0;
                for (int k=0;k<4;++k) sum+=m[k*4+row]*b.m[col*4+k];
                res.m[col*4+row]=sum;
            }
        return res;
    }
};