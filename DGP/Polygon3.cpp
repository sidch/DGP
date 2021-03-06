//============================================================================
//
// DGP: Digital Geometry Processing toolkit
// Copyright (C) 2016, Siddhartha Chaudhuri
//
// This software is covered by a BSD license. Portions derived from other
// works are covered by their respective licenses. For full licensing
// information see the LICENSE.txt file.
//
//============================== Original License ============================
//
// Copyright (c) 2009 by John W. Ratcliff mailto:jratcliffscarab@gmail.com
//
// Portions of this source has been released with the PhysXViewer application, as well as
// Rocket, CreateDynamics, ODF, and as a number of sample code snippets.
//
// If you find this code useful or you are feeling particularily generous I would
// ask that you please go to http://www.amillionpixels.us and make a donation
// to Troy DeMolay.
//
// DeMolay is a youth group for young men between the ages of 12 and 21.
// It teaches strong moral principles, as well as leadership skills and
// public speaking.  The donations page uses the 'pay for pixels' paradigm
// where, in this case, a pixel is only a single penny.  Donations can be
// made for as small as $4 or as high as a $100 block.  Each person who donates
// will get a link to their own site as well as acknowledgement on the
// donations blog located here http://www.amillionpixels.blogspot.com/
//
// If you wish to contact me you can use the following methods:
//
// Skype ID: jratcliff63367
// Yahoo: jratcliff63367
// AOL: jratcliff1961
// email: jratcliffscarab@gmail.com
//
//
// The MIT license:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//============================================================================

#include "Polygon3.hpp"
#include <cmath>
#include <limits>

namespace DGP {

Polygon3::Polygon3()
: max_index(-1)
{}

void
Polygon3::clear()
{
  vertices.clear();
  max_index = -1;
  bounds = AxisAlignedBox3();
}

void
Polygon3::addVertex(Vector3 const & p)
{
  addVertex(p, max_index + 1);
}

void
Polygon3::addVertex(Vector3 const & p, long index)
{
  // update bounding box...
  bounds.merge(p);

  vertices.push_back(IndexedVertex(p, index));
  if (index > max_index) max_index = index;
}

long
Polygon3::numVertices() const
{
  return (long)vertices.size();
}

Polygon3::IndexedVertex const &
Polygon3::getVertex(long poly_index) const
{
  debugAssertM(poly_index >= 0 && poly_index < numVertices(), "Polygon3: Vertex index out of bounds");

  return vertices[(size_t)poly_index];
}

static bool
Polygon3_insideTriangle2(Vector2 const & A, Vector2 const & B, Vector2 const & C, Vector2 const & P)
{
  Real ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
  Real cCROSSap, bCROSScp, aCROSSbp;

  ax  = C.x() - B.x();  ay  = C.y() - B.y();
  bx  = A.x() - C.x();  by  = A.y() - C.y();
  cx  = B.x() - A.x();  cy  = B.y() - A.y();
  apx = P.x() - A.x();  apy = P.y() - A.y();
  bpx = P.x() - B.x();  bpy = P.y() - B.y();
  cpx = P.x() - C.x();  cpy = P.y() - C.y();

  aCROSSbp = ax * bpy - ay * bpx;
  cCROSSap = cx * apy - cy * apx;
  bCROSScp = bx * cpy - by * cpx;

  return ((aCROSSbp >= 0) && (bCROSScp >= 0) && (cCROSSap >= 0));
}

bool
Polygon3::snip(size_t u, size_t v, size_t w, size_t n, std::vector<size_t> const & indices) const
{
  static Real const EPSILON = 1e-10f;

  Vector2 const & A = proj_vertices[indices[u]];
  Vector2 const & B = proj_vertices[indices[v]];
  Vector2 const & C = proj_vertices[indices[w]];

  if (EPSILON > (((B.x() - A.x()) * (C.y() - A.y())) - ((B.y() - A.y()) * (C.x() - A.x()))))
  {
    // DGP_DEBUG << "Polygon3: Epsilon test failed: A = " << A << ", B = " << B << ", C = " << C << ", eps = " << EPSILON;
    return false;
  }

  for (size_t p = 0; p < n; ++p)
  {
    if ((p == u) || (p == v) || (p == w))
      continue;

    Vector2 const & P = proj_vertices[indices[p]];
    if (Polygon3_insideTriangle2(A, B, C, P))
    {
      // DGP_DEBUG << "Polygon3: Triangle test failed: A = " << A << ", B = " << B << ", C = " << C << ", P = " << P;
      return false;
    }
  }

  return true;
}

// Original comment:
//   Triangulation happens in 2d. We could inverse transform the polygon around the normal direction, or we just use the two
//   most signficant axes. Here we find the two longest axes and use them to triangulate.  Inverse transforming them would
//   introduce more doubling point error and isn't worth it.
//
// SC says:
//   This doesn't work: the vertices can be collinear when projected onto the plane of the two longest axes of the bounding box.
//   Example (from real data):
//
//     v[0] = (-13.7199, 4.45725, -8.00059)
//     v[1] = (-0.115787, 12.3116, -4.96109)
//     v[2] = (0.88992, 12.8922, -3.80342)
//     v[3] = (-0.115787, 12.3116, -2.64576)
//     v[4] = (-13.7199, 4.45725, 0.393742)
//     v[5] = (-13.7199, 4.45725, -0.856258)
//     v[6] = (-12.5335, 5.14221, -3.80342)
//     v[7] = (-13.7199, 4.45725, -6.75059)
//
//   Instead, we will project onto the plane of the polygon.
long
Polygon3::triangulate(std::vector<long> & tri_indices) const
{
  if (vertices.size() < 3)
  {
    tri_indices.clear();
  }
  else if (vertices.size() == 3)
  {
    tri_indices.resize(3);
    tri_indices[0] = vertices[0].index;
    tri_indices[1] = vertices[1].index;
    tri_indices[2] = vertices[2].index;
  }
  else if (vertices.size() > 3)
  {
    tri_indices.clear();

    size_t n = vertices.size();
    proj_vertices.resize(n);

    Vector3 normal = getNormal();
    Vector3 axis0, axis1;
    normal.createOrthonormalBasis(axis0, axis1);

    Vector3 v0 = vertices[0].position;  // a reference point for the plane of the polygon
    for (size_t i = 0; i < n; ++i)
    {
      Vector3 v = vertices[i].position - v0;
      proj_vertices[i] = Vector2(v.dot(axis0), v.dot(axis1));
    }

    std::vector<size_t> indices(n);
    bool flipped = false;
    if (projArea() > 0)
    {
      for (size_t v = 0; v < n; ++v)
        indices[v] = v;
    }
    else
    {
      for (size_t v = 0; v < n; ++v)
        indices[v] = (n - 1) - v;

      flipped = true;
    }

    size_t nv = n;
    size_t count = 2 * nv;
    for (size_t v = nv - 1; nv > 2; )
    {
      if ((count--) <= 0)
        break;

      size_t u = v;
      if (nv <= u) u = 0;

      v = u + 1;
      if (nv <= v) v = 0;

      size_t w = v + 1;
      if (nv <= w) w = 0;

      if (snip(u, v, w, nv, indices))
      {
        size_t a = indices[u];
        size_t b = indices[v];
        size_t c = indices[w];
        if (flipped)
        {
          tri_indices.push_back(vertices[c].index);
          tri_indices.push_back(vertices[b].index);
          tri_indices.push_back(vertices[a].index);
        }
        else
        {
          tri_indices.push_back(vertices[a].index);
          tri_indices.push_back(vertices[b].index);
          tri_indices.push_back(vertices[c].index);
        }

        size_t s = v, t = v + 1;
        for ( ; t < nv; ++s, ++t)
          indices[s] = indices[t];

        nv--;
        count = 2 * nv;
      }
    }
  }

  return (long)tri_indices.size() / 3;
}

Real
Polygon3::projArea() const
{
  size_t n = proj_vertices.size();
  Real a = 0;
  for (size_t p = n - 1, q = 0; q < n; p = q++)
  {
    Vector2 const & pval = proj_vertices[p];
    Vector2 const & qval = proj_vertices[q];
    a += pval.x() * qval.y() - qval.x() * pval.y();
  }

  return 0.5f * a;
}

Real
Polygon3::area() const
{
  size_t n = vertices.size();
  if (n < 3)
    return 0;

  if (n == 3)
    return 0.5f * (vertices[2].position - vertices[1].position).cross(vertices[0].position - vertices[1].position).length();

  static Real const EPSILON = 1e-10f;

  Vector3 normal = getNormal();
  if (normal.squaredLength() < EPSILON)
    return 0;

  Real a = 0;
  Vector3 const & last = vertices[n - 1].position;
  Vector3 prev = last;
  for (size_t p = 0; p < n; ++p)
  {
    Vector3 const & v = vertices[p].position;
    a += (prev.cross(v)).dot(normal);
    prev = v;
  }

  return std::fabs(0.5f * a);
}

Vector3
Polygon3::getNormal() const
{
  size_t n = vertices.size();
  if (n < 3)
    return Vector3::zero();

  static Real const EPSILON = 1e-10f;

  for (size_t i = 0; i < n; ++i)
  {
    size_t i1 = (i + 1) % n;
    size_t i2 = (i + 2) % n;
    Vector3 normal = (vertices[i2].position - vertices[i1].position).cross(vertices[i].position - vertices[i1].position);

    if (normal.squaredLength() >= EPSILON)
      return normal.unit();
  }

  return Vector3::zero();
}

} // namespace DGP
