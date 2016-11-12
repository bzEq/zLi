#include "CIE.hh"
#include "math.hh"

namespace zLi {

// https://en.wikipedia.org/wiki/CIE_1931_color_space
const Float CIE::XYZ2RGB[3][3] = {
    {0.41847, -0.15866, -0.082835},
    {-0.091169, 0.25243, 0.015708},
    {0.00092090, -0.0025498, 0.17860},
};

const Float CIE::RGB2XYZ[3][3] = {
    {2.7688, 1.75169, 1.13011},
    {0.999994, 4.59063, 0.0600501},
    {0, 0.0565065, 5.59413},
};
}
