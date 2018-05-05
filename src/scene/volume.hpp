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
            minPos.x = min(minPos.x, pos.x);
            minPos.y = min(minPos.y, pos.y);
            minPos.z = min(minPos.z, pos.z);
            maxPos.x = max(maxPos.x, pos.x);
            maxPos.y = max(maxPos.y, pos.y);
            maxPos.z = max(maxPos.z, pos.z);
        }

        void add(const AABB& box)
        {
            add(box.minPos);
            add(box.maxPos);
        }

        Size3Df getSize() const
        {
            return Size3Df(maxPos - minPos);
        }
    };

    /* Transformed bounding box in the world space. */
    struct BoundingBox
    {
        Vec3f points[8];

        BoundingBox() {}

        BoundingBox(const AABB& axisAlignedBox, const Mat4f& M)
        {
            const Vec3f& minP = axisAlignedBox.minPos;
            const Vec3f& maxP = axisAlignedBox.maxPos;
            points[0] = M * Vec3f(minP.x, minP.y, minP.z);
            points[1] = M * Vec3f(maxP.x, minP.y, minP.z);
            points[2] = M * Vec3f(minP.x, maxP.y, minP.z);
            points[3] = M * Vec3f(maxP.x, maxP.y, minP.z);
            points[4] = M * Vec3f(minP.x, minP.y, maxP.z);
            points[5] = M * Vec3f(maxP.x, minP.y, maxP.z);
            points[6] = M * Vec3f(minP.x, maxP.y, maxP.z);
            points[7] = M * Vec3f(maxP.x, maxP.y, maxP.z);
        }

        inline Vec3f  operator [] (int i) const { return points[i]; }
        inline Vec3f& operator [] (int i)       { return points[i]; }
    };

    // TODO?
    struct BoundingSphere
    {

    };
}