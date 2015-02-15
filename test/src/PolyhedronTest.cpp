/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include "CollectionUtils.h"

#include "Polyhedron.h"
#include "MathUtils.h"
#include "TestUtils.h"

typedef Polyhedron<double> Polyhedron3d;
typedef Polyhedron3d::Vertex Vertex;
typedef Polyhedron3d::VertexList VertexList;
typedef Polyhedron3d::Edge Edge;
typedef Polyhedron3d::HalfEdge HalfEdge;
typedef Polyhedron3d::EdgeList EdgeList;
typedef Polyhedron3d::Face Face;
typedef Polyhedron3d::FaceList FaceList;

typedef std::pair<Vec3d, Vec3d> EdgeInfo;
typedef std::vector<EdgeInfo> EdgeInfoList;

bool hasVertices(const VertexList& vertices, Vec3d::List points);
bool hasEdges(const EdgeList& edges, EdgeInfoList edgeInfos);
bool hasTriangleOf(const FaceList& faces, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3);
bool hasQuadOf(const FaceList& faces, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3, const Vec3d& p4);

TEST(PolyhedronTest, initWith4Points) {
    const Vec3d p1( 0.0, 0.0, 8.0);
    const Vec3d p2( 8.0, 0.0, 0.0);
    const Vec3d p3(-8.0, 0.0, 0.0);
    const Vec3d p4( 0.0, 8.0, 0.0);
    
    const Polyhedron3d p(p1, p2, p3, p4);
    ASSERT_TRUE(p.closed());
    
    ASSERT_EQ(4u, p.vertexCount());
    ASSERT_EQ(6u, p.edgeCount());
    ASSERT_EQ(4u, p.faceCount());
    
    const VertexList& vertices = p.vertices();
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    ASSERT_TRUE(hasVertices(vertices, points));

    const EdgeList& edges = p.edges();
    
    EdgeInfoList edgeInfos;
    edgeInfos.push_back(std::make_pair(p2, p3));
    edgeInfos.push_back(std::make_pair(p3, p4));
    edgeInfos.push_back(std::make_pair(p4, p2));
    edgeInfos.push_back(std::make_pair(p1, p3));
    edgeInfos.push_back(std::make_pair(p1, p2));
    edgeInfos.push_back(std::make_pair(p4, p1));
    
    ASSERT_TRUE(hasEdges(edges, edgeInfos));
    
    const FaceList& faces = p.faces();
    
    ASSERT_TRUE(hasTriangleOf(faces, p2, p3, p4));
    ASSERT_TRUE(hasTriangleOf(faces, p1, p3, p2));
    ASSERT_TRUE(hasTriangleOf(faces, p1, p2, p4));
    ASSERT_TRUE(hasTriangleOf(faces, p1, p4, p3));
}
/*
TEST(PolyhedronTest, testImpossibleSplit) {
    const Vec3d p1( 0.0, 4.0, 8.0);
    const Vec3d p2( 8.0, 0.0, 0.0);
    const Vec3d p3(-8.0, 0.0, 0.0);
    const Vec3d p4( 0.0, 8.0, 0.0);
    const Vec3d p5( 0.0, 4.0, 4.0);
    
    Polyhedron3d p(p1, p2, p3, p4);
    Polyhedron3d::Seam seam = p.split(Polyhedron3d::SplitByVisibilityCriterion(p5));
    ASSERT_TRUE(seam.empty());
}

TEST(PolyhedronTest, testSimpleSplit) {
    const Vec3d p1( 0.0, 4.0, 8.0);
    const Vec3d p2( 8.0, 0.0, 0.0);
    const Vec3d p3(-8.0, 0.0, 0.0);
    const Vec3d p4( 0.0, 8.0, 0.0);
    const Vec3d p5( 0.0, 4.0, 12.0);
    
    Polyhedron3d p(p1, p2, p3, p4);
    Polyhedron3d::Seam seam = p.split(Polyhedron3d::SplitByVisibilityCriterion(p5));
    ASSERT_EQ(3u, seam.size());
    
    ASSERT_FALSE(p.closed());
    ASSERT_EQ(3u, p.vertexCount());
    ASSERT_EQ(3u, p.edgeCount());
    ASSERT_EQ(1u, p.faceCount());

    ASSERT_TRUE(hasTriangleOf(p.faces(), p2, p3, p4));
}

TEST(PolyhedronTest, testWeaveSimpleCap) {
    const Vec3d p1( 0.0, 4.0, 8.0);
    const Vec3d p2( 8.0, 0.0, 0.0);
    const Vec3d p3(-8.0, 0.0, 0.0);
    const Vec3d p4( 0.0, 8.0, 0.0);
    const Vec3d p5( 0.0, 4.0, 12.0);
    
    Polyhedron3d p(p1, p2, p3, p4);
    Polyhedron3d::Seam seam = p.split(Polyhedron3d::SplitByVisibilityCriterion(p5));
    
    p.weaveCap(seam, p5);
    ASSERT_TRUE(p.closed());
    ASSERT_EQ(4u, p.vertexCount());
    ASSERT_EQ(6u, p.edgeCount());
    ASSERT_EQ(4u, p.faceCount());
}
*/
TEST(PolyhedronTest, testSimpleConvexHull) {
    const Vec3d p1( 0.0, 4.0, 8.0);
    const Vec3d p2( 8.0, 0.0, 0.0);
    const Vec3d p3(-8.0, 0.0, 0.0);
    const Vec3d p4( 0.0, 8.0, 0.0);
    const Vec3d p5( 0.0, 4.0, 12.0);
    
    Polyhedron3d p(p1, p2, p3, p4);
    p.addPoint(p5);
    
    ASSERT_TRUE(p.closed());
    ASSERT_EQ(4u, p.vertexCount());
    ASSERT_EQ(6u, p.edgeCount());
    ASSERT_EQ(4u, p.faceCount());

    const VertexList& vertices = p.vertices();
    Vec3d::List points;
    points.push_back(p5);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    ASSERT_TRUE(hasVertices(vertices, points));
    
    const EdgeList& edges = p.edges();
    
    EdgeInfoList edgeInfos;
    edgeInfos.push_back(std::make_pair(p2, p3));
    edgeInfos.push_back(std::make_pair(p3, p4));
    edgeInfos.push_back(std::make_pair(p4, p2));
    edgeInfos.push_back(std::make_pair(p5, p3));
    edgeInfos.push_back(std::make_pair(p5, p2));
    edgeInfos.push_back(std::make_pair(p4, p5));
    
    ASSERT_TRUE(hasEdges(edges, edgeInfos));
    
    const FaceList& faces = p.faces();
    
    ASSERT_TRUE(hasTriangleOf(faces, p2, p3, p4));
    ASSERT_TRUE(hasTriangleOf(faces, p5, p3, p2));
    ASSERT_TRUE(hasTriangleOf(faces, p5, p2, p4));
    ASSERT_TRUE(hasTriangleOf(faces, p5, p4, p3));
}

TEST(PolyhedronTest, testSimpleConvexHullWithCoplanarFaces) {
    const Vec3d p1( 0.0, 0.0, 8.0);
    const Vec3d p2( 8.0, 0.0, 0.0);
    const Vec3d p3(-8.0, 0.0, 0.0);
    const Vec3d p4( 0.0, 8.0, 0.0);
    const Vec3d p5( 0.0, 0.0, 12.0);
    
    Polyhedron3d p(p1, p2, p3, p4);
    p.addPoint(p5);
    
    ASSERT_TRUE(p.closed());
    ASSERT_EQ(4u, p.vertexCount());
    ASSERT_EQ(6u, p.edgeCount());
    ASSERT_EQ(4u, p.faceCount());
    
    const VertexList& vertices = p.vertices();
    Vec3d::List points;
    points.push_back(p5);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    ASSERT_TRUE(hasVertices(vertices, points));
    
    const EdgeList& edges = p.edges();
    
    EdgeInfoList edgeInfos;
    edgeInfos.push_back(std::make_pair(p2, p3));
    edgeInfos.push_back(std::make_pair(p3, p4));
    edgeInfos.push_back(std::make_pair(p4, p2));
    edgeInfos.push_back(std::make_pair(p5, p3));
    edgeInfos.push_back(std::make_pair(p5, p2));
    edgeInfos.push_back(std::make_pair(p4, p5));
    
    ASSERT_TRUE(hasEdges(edges, edgeInfos));
    
    const FaceList& faces = p.faces();
    
    ASSERT_TRUE(hasTriangleOf(faces, p2, p3, p4));
    ASSERT_TRUE(hasTriangleOf(faces, p5, p3, p2));
    ASSERT_TRUE(hasTriangleOf(faces, p5, p2, p4));
    ASSERT_TRUE(hasTriangleOf(faces, p5, p4, p3));
}

TEST(PolyhedronTest, testSimpleConvexHullOfCube) {
    const Vec3d p1( -8.0, -8.0, -8.0);
    const Vec3d p2( -8.0, -8.0, +8.0);
    const Vec3d p3( -8.0, +8.0, -8.0);
    const Vec3d p4( -8.0, +8.0, +8.0);
    const Vec3d p5( +8.0, -8.0, -8.0);
    const Vec3d p6( +8.0, -8.0, +8.0);
    const Vec3d p7( +8.0, +8.0, -8.0);
    const Vec3d p8( +8.0, +8.0, +8.0);

    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);
    points.push_back(p6);
    points.push_back(p7);
    points.push_back(p8);
    
    Polyhedron3d p(points);
    
    ASSERT_TRUE(p.closed());
    ASSERT_EQ(8u, p.vertexCount());
    ASSERT_EQ(12u, p.edgeCount());
    ASSERT_EQ(6u, p.faceCount());
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
    
    const EdgeList& edges = p.edges();
    
    EdgeInfoList edgeInfos;
    edgeInfos.push_back(std::make_pair(p1, p2));
    edgeInfos.push_back(std::make_pair(p1, p3));
    edgeInfos.push_back(std::make_pair(p1, p5));
    edgeInfos.push_back(std::make_pair(p2, p4));
    edgeInfos.push_back(std::make_pair(p2, p6));
    edgeInfos.push_back(std::make_pair(p3, p4));
    edgeInfos.push_back(std::make_pair(p3, p7));
    edgeInfos.push_back(std::make_pair(p4, p8));
    edgeInfos.push_back(std::make_pair(p5, p6));
    edgeInfos.push_back(std::make_pair(p5, p7));
    edgeInfos.push_back(std::make_pair(p6, p8));
    edgeInfos.push_back(std::make_pair(p7, p8));
    
    ASSERT_TRUE(hasEdges(edges, edgeInfos));
    
    const FaceList& faces = p.faces();
    
    ASSERT_TRUE(hasQuadOf(faces, p1, p5, p6, p2));
    ASSERT_TRUE(hasQuadOf(faces, p3, p1, p2, p4));
    ASSERT_TRUE(hasQuadOf(faces, p7, p3, p4, p8));
    ASSERT_TRUE(hasQuadOf(faces, p5, p7, p8, p6));
    ASSERT_TRUE(hasQuadOf(faces, p3, p7, p5, p1));
    ASSERT_TRUE(hasQuadOf(faces, p2, p6, p8, p4));
}

TEST(PolyhedronTest, initEmpty) {
    Polyhedron3d p;
    ASSERT_TRUE(p.empty());
}

TEST(PolyhedronTest, initEmptyAndAddOnePoint) {
    const Vec3d p1( -8.0, -8.0, -8.0);

    Polyhedron3d p;
    p.addPoint(p1);
    
    ASSERT_FALSE(p.empty());
    ASSERT_TRUE(p.point());
    ASSERT_FALSE(p.edge());
    ASSERT_FALSE(p.polygon());
    ASSERT_FALSE(p.polyhedron());

    Vec3d::List points;
    points.push_back(p1);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}


TEST(PolyhedronTest, initEmptyAndAddTwoIdenticalPoints) {
    const Vec3d p1( -8.0, -8.0, -8.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p1);
    
    ASSERT_FALSE(p.empty());
    ASSERT_TRUE(p.point());
    ASSERT_FALSE(p.edge());
    ASSERT_FALSE(p.polygon());
    ASSERT_FALSE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

TEST(PolyhedronTest, initEmptyAndAddTwoPoints) {
    const Vec3d p1(0.0, 0.0, 0.0);
    const Vec3d p2(3.0, 0.0, 0.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p2);
    
    ASSERT_FALSE(p.empty());
    ASSERT_FALSE(p.point());
    ASSERT_TRUE(p.edge());
    ASSERT_FALSE(p.polygon());
    ASSERT_FALSE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

TEST(PolyhedronTest, initEmptyAndAddThreeColinearPoints) {
    const Vec3d p1(0.0, 0.0, 0.0);
    const Vec3d p2(3.0, 0.0, 0.0);
    const Vec3d p3(6.0, 0.0, 0.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p2);
    p.addPoint(p3);
    
    ASSERT_FALSE(p.empty());
    ASSERT_FALSE(p.point());
    ASSERT_TRUE(p.edge());
    ASSERT_FALSE(p.polygon());
    ASSERT_FALSE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p3);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

TEST(PolyhedronTest, initEmptyAndAddThreePoints) {
    const Vec3d p1(0.0, 0.0, 0.0);
    const Vec3d p2(3.0, 0.0, 0.0);
    const Vec3d p3(6.0, 5.0, 0.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p2);
    p.addPoint(p3);
    
    ASSERT_FALSE(p.empty());
    ASSERT_FALSE(p.point());
    ASSERT_FALSE(p.edge());
    ASSERT_TRUE(p.polygon());
    ASSERT_FALSE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

TEST(PolyhedronTest, initEmptyAndAddThreePointsAndOneInnerPoint) {
    const Vec3d p1(0.0, 0.0, 0.0);
    const Vec3d p2(6.0, 0.0, 0.0);
    const Vec3d p3(3.0, 6.0, 0.0);
    const Vec3d p4(3.0, 3.0, 0.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p2);
    p.addPoint(p3);
    p.addPoint(p4);
    
    ASSERT_FALSE(p.empty());
    ASSERT_FALSE(p.point());
    ASSERT_FALSE(p.edge());
    ASSERT_TRUE(p.polygon());
    ASSERT_FALSE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

TEST(PolyhedronTest, initEmptyAndAddFourCoplanarPoints) {
    const Vec3d p1(0.0, 0.0, 0.0);
    const Vec3d p2(6.0, 0.0, 0.0);
    const Vec3d p3(3.0, 3.0, 0.0);
    const Vec3d p4(3.0, 6.0, 0.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p2);
    p.addPoint(p3);
    p.addPoint(p4);
    
    ASSERT_FALSE(p.empty());
    ASSERT_FALSE(p.point());
    ASSERT_FALSE(p.edge());
    ASSERT_TRUE(p.polygon());
    ASSERT_FALSE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p4);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

TEST(PolyhedronTest, initEmptyAndAddFourPoints) {
    const Vec3d p1(0.0, 0.0, 0.0);
    const Vec3d p2(6.0, 0.0, 0.0);
    const Vec3d p3(3.0, 6.0, 0.0);
    const Vec3d p4(3.0, 3.0, 6.0);
    
    Polyhedron3d p;
    p.addPoint(p1);
    p.addPoint(p2);
    p.addPoint(p3);
    p.addPoint(p4);
    
    ASSERT_FALSE(p.empty());
    ASSERT_FALSE(p.point());
    ASSERT_FALSE(p.edge());
    ASSERT_FALSE(p.polygon());
    ASSERT_TRUE(p.polyhedron());
    
    Vec3d::List points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    
    const VertexList& vertices = p.vertices();
    ASSERT_TRUE(hasVertices(vertices, points));
}

bool hasVertices(const VertexList& vertices, Vec3d::List points) {
    VertexList::ConstIterator vIt = vertices.iterator();
    while (vIt.hasNext()) {
        const Vertex* vertex = vIt.next();
        Vec3d::List::iterator pIt = VectorUtils::find(points, vertex->position());
        if (pIt == points.end())
            return false;
        points.erase(pIt);
    }
    return points.empty();
}

EdgeInfoList::iterator findEdgeInfo(EdgeInfoList& edgeInfos, const Edge* edge);

bool hasEdges(const EdgeList& edges, EdgeInfoList edgeInfos) {
    EdgeList::ConstIterator eIt = edges.iterator();
    while (eIt.hasNext()) {
        const Edge* edge = eIt.next();
        EdgeInfoList::iterator it = findEdgeInfo(edgeInfos, edge);
        if (it == edgeInfos.end())
            return false;
        edgeInfos.erase(it);
    }
    return edgeInfos.empty();
}

EdgeInfoList::iterator findEdgeInfo(EdgeInfoList& edgeInfos, const Edge* edge) {
    Vertex* v1 = edge->firstVertex();
    Vertex* v2 = edge->secondVertex();
    
    EdgeInfoList::iterator it, end;
    for (it = edgeInfos.begin(), end = edgeInfos.end(); it != end; ++it) {
        const EdgeInfo& info = *it;
        if ((info.first == v1->position() && info.second == v2->position()) ||
            (info.first == v2->position() && info.second == v1->position()))
            return it;
    }
    return end;
}

bool isTriangleOf(const Face* face, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3);
bool hasTriangleOf(const FaceList& faces, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3) {
    FaceList::ConstIterator fIt = faces.iterator();
    while (fIt.hasNext()) {
        const Face* face = fIt.next();
        if (isTriangleOf(face, p1, p2, p3))
            return true;
    }
    return false;
}

bool isQuadOf(const Face* face, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3, const Vec3d& p4);
bool hasQuadOf(const FaceList& faces, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3, const Vec3d& p4) {
    FaceList::ConstIterator fIt = faces.iterator();
    while (fIt.hasNext()) {
        const Face* face = fIt.next();
        if (isQuadOf(face, p1, p2, p3, p4))
            return true;
    }
    return false;
}

bool isTriangleOf(const Face* face, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3) {
    typedef Polyhedron3d::HalfEdgeList BoundaryList;
    
    const BoundaryList& boundary = face->boundary();
    if (boundary.size() != 3)
        return false;
    
    BoundaryList::ConstIterator it = boundary.iterator();
    while (it.hasNext()) {
        const HalfEdge* e1 = it.next();
        if (e1->origin()->position() == p1) {
            const HalfEdge* e2 = e1->next();
            const HalfEdge* e3 = e2->next();
            return e2->origin()->position() == p2 && e3->origin()->position() == p3;
        }
    }
    return false;
}

bool isQuadOf(const Face* face, const Vec3d& p1, const Vec3d& p2, const Vec3d& p3, const Vec3d& p4) {
    typedef Polyhedron3d::HalfEdgeList BoundaryList;
    
    const BoundaryList& boundary = face->boundary();
    if (boundary.size() != 4)
        return false;
    
    BoundaryList::ConstIterator it = boundary.iterator();
    while (it.hasNext()) {
        const HalfEdge* e1 = it.next();
        if (e1->origin()->position() == p1) {
            const HalfEdge* e2 = e1->next();
            const HalfEdge* e3 = e2->next();
            const HalfEdge* e4 = e3->next();
            return e2->origin()->position() == p2 && e3->origin()->position() == p3 && e4->origin()->position() == p4;
        }
    }
    return false;
}
