//
// Created by Saatus on 2023/6/10.
//

#ifndef BEZIER_MODE_H
#define BEZIER_MODE_H


const unsigned int Select = 0x20000000;
const unsigned int Pick = 0x40000000;

enum Mode {
    ViewMode,
    EditMode,
};


enum DrawMode {
    Bezier,
    NURBS,
    BSpline,
    BezierSurface,
    NURBSSurface,
    BSplineSurface,
    BezierSelected,
    NURBSSelected,
    BSplineSelected,
    BezierPicked,
    NURBSPicked,
    BSplinePicked,
};



#endif //BEZIER_MODE_H
