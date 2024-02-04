#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(float xin, float yin)
    : x(xin), y(yin)
{
    
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float &rhs) const
{
    return Vec2(x * rhs, y * rhs);
}

Vec2 Vec2::operator/(const float &rhs) const
{
    return Vec2(x / rhs, y / rhs);
}

void Vec2::operator+=(const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator*=(const float &rhs)
{
    x *= rhs;
    y *= rhs;
}

void Vec2::operator/=(const float &rhs)
{
    x /= rhs;
    y /= rhs;
}

bool Vec2::operator==(const Vec2 &rhs) const
{
    return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
    return (x != rhs.x || y != rhs.y);
}

float Vec2::dist(const Vec2 &rhs) const
{
    float dx = x - rhs.x;
    float dy = y - rhs.y;
    return sqrt(dx * dx + dy * dy);
}

float Vec2::length() const
{
    return sqrt(x * x + y * y);
}

void Vec2::normalize()
{
    float len = length();
    if (len != 0)
    {
        x /= len;
        y /= len;
    }
}