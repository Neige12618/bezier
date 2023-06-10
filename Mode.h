//
// Created by Saatus on 2023/6/10.
//

#ifndef BEZIER_MODE_H
#define BEZIER_MODE_H


const unsigned int Select = 0x20000000;
const unsigned int Pick = 0x40000000;

enum Mode {
    Bezier = 0x00000001,
    NURBS = 0x00000002,
    BSpline = 0x00000004,
    BezierSelected = 0x20000001,
    NURBSSelected = 0x20000002,
    BSplineSelected = 0x20000004,
    BezierPicked = 0x40000001,
    NURBSPicked = 0x40000002,
    BSplinePicked = 0x40000004,
};

constexpr inline bool selected(Mode mode) {
    return (mode & Select) == Select;
}

constexpr inline bool picked(Mode mode) {
    return (mode & Pick) == Pick;
}





#endif //BEZIER_MODE_H
