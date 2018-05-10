#pragma once

#include "util/data/mat4.hpp"
#include "util/data/vec3.hpp"
#include "util/data/size3d.hpp"


namespace Base
{
    /* Axis aligned bounding box in model space. */
    struct AABB
    {
        Vec3f minPos, maxPos;

        AABB() {}

        AABB(const Vec3f& pos)
        {
            minPos = maxPos = pos;
        }
        
        AABB(const AABB& box)
        {
            minPos = box.minPos;
            maxPos = box.maxPos;
        }

        void add(const Vec3f& pos)
        {
            minPos = Vec3f::min(minPos, pos);
            maxPos = Vec3f::max(maxPos, pos);
        }

        void add(const AABB& box)
        {
            minPos = Vec3f::min(minPos, box.minPos);
            maxPos = Vec3f::max(maxPos, box.maxPos);
        }

        Size3Df getSize() const
        {
            return Size3Df(maxPos - minPos);
        }
    };

    /* Transformed bounding box in the world space. */
    struct BoundingBox
    {
        Vec4f points[8];

        BoundingBox() {}

        BoundingBox(const AABB& axisAlignedBox, const Mat4f& M)
        {
            const Vec3f& minP = axisAlignedBox.minPos;
            const Vec3f& maxP = axisAlignedBox.maxPos;
            points[0] = M * Vec4f(minP.x, minP.y, minP.z, 1.f);
            points[1] = M * Vec4f(maxP.x, minP.y, minP.z, 1.f);
            points[2] = M * Vec4f(minP.x, maxP.y, minP.z, 1.f);
            points[3] = M * Vec4f(maxP.x, maxP.y, minP.z, 1.f);
            points[4] = M * Vec4f(minP.x, minP.y, maxP.z, 1.f);
            points[5] = M * Vec4f(maxP.x, minP.y, maxP.z, 1.f);
            points[6] = M * Vec4f(minP.x, maxP.y, maxP.z, 1.f);
            points[7] = M * Vec4f(maxP.x, maxP.y, maxP.z, 1.f);
        }

        inline Vec4f  operator [] (int i) const
        {
            return points[i];
        }

        inline Vec4f& operator [] (int i)
        {
            return points[i];
        }
    };

    // TODO?
    struct BoundingSphere
    {

    };
}