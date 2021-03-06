#include "stdafx.h"
#include "../rectangles/Rectangle.h"

BOOST_AUTO_TEST_SUITE(VerifyCRectangleCreation)

BOOST_AUTO_TEST_CASE(EmptyRectangle)
{
    CRectangle rectangle(0, 0, 0, 0);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 0);
    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 0);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 0);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 0);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 0);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 0);
}

BOOST_AUTO_TEST_CASE(WithPositiveParams)
{
    CRectangle rectangle(7, 9, 3, 4);
    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 7);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 9);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 13);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 10);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 4);
}

BOOST_AUTO_TEST_CASE(WithNegativeParams)
{
    CRectangle rectangle(-7, -9, 3, 4);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), -9);
    BOOST_CHECK_EQUAL(rectangle.GetLeft(), -7);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), -5);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), -4);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 4);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(VerifyRectangleTransformations)

BOOST_AUTO_TEST_CASE(VerifySetters)
{
    CRectangle rectangle(0, 0, 0, 0);

    rectangle.SetTop(20);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 20);

    rectangle.SetLeft(30);
    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 30);

    rectangle.SetRight(60);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 60);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 30);

    rectangle.SetBottom(40);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 40);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 20);

    rectangle.SetHeight(50);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 50);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 70);

    rectangle.SetWidth(50);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 50);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 80);

}

BOOST_AUTO_TEST_CASE(VerifyMoveMethod)
{
    CRectangle rectangle(2, 3, 4, 5);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 2);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 4);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 5);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 6);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 8);

    rectangle.Move(2, 2);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 4);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 5);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 4);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 5);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 8);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 10);

    rectangle.Move(-2, -2);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 2);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 4);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 5);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 6);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 8);

    rectangle.Move(-3, -3);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), -1);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 0);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 4);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 5);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 5);
}

BOOST_AUTO_TEST_CASE(VerifyScaleMethod)
{
    CRectangle rectangle(2, 3, 4, 5);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 2);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 4);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 5);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 6);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 8);

    rectangle.Scale(2, 2);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 2);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 8);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 10);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 10);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 13);

    rectangle.Scale(-2, -2);

    BOOST_CHECK_EQUAL(rectangle.GetLeft(), 2);
    BOOST_CHECK_EQUAL(rectangle.GetTop(), 3);
    BOOST_CHECK_EQUAL(rectangle.GetWidth(), 8);
    BOOST_CHECK_EQUAL(rectangle.GetHeight(), 10);
    BOOST_CHECK_EQUAL(rectangle.GetRight(), 10);
    BOOST_CHECK_EQUAL(rectangle.GetBottom(), 13);
}

BOOST_AUTO_TEST_CASE(VerifyIntersectMethod)
{
    CRectangle rec1(2, 3, 4, 5);
    CRectangle rec2(3, 2, 5, 3);

    BOOST_CHECK(rec1.Intersect(rec2));
    BOOST_CHECK_EQUAL(rec1.GetLeft(), 3);
    BOOST_CHECK_EQUAL(rec1.GetTop(), 3);
    BOOST_CHECK_EQUAL(rec1.GetWidth(), 3);
    BOOST_CHECK_EQUAL(rec1.GetHeight(), 2);
    BOOST_CHECK_EQUAL(rec1.GetRight(), 6);
    BOOST_CHECK_EQUAL(rec1.GetBottom(), 5);

    rec1.Move(-10, -10);
    BOOST_CHECK(!rec1.Intersect(rec2));
    BOOST_CHECK_EQUAL(rec1.GetWidth(), 0);
    BOOST_CHECK_EQUAL(rec1.GetHeight(), 0);
}

BOOST_AUTO_TEST_CASE(VerifyGetAreaMethod)
{
    CRectangle rec(0, 0, 4, 5);
    BOOST_CHECK_EQUAL(rec.GetArea(), 20);
}

BOOST_AUTO_TEST_CASE(VerifyGetPerimeterMethod)
{
    CRectangle rec(0, 0, 4, 5);
    BOOST_CHECK_EQUAL(rec.GetPerimeter(), 18);
}

BOOST_AUTO_TEST_SUITE_END()