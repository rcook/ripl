#include "helper.h"

using namespace std;
using namespace ripl::test::helper;

namespace ripl { namespace test { namespace helper {
    bool compareImageData(
        unsigned width,
        unsigned height,
        const vector<riplGrey>& data,
        const riplGreyMap& image)
    {
        if (width != image.width())
        {
            return false;
        }

        if (height != image.height())
        {
            return false;
        }

        if (width * height != data.size())
        {
            return false;
        }

        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != image[i])
            {
                return false;
            }
        }

        return true;
    }
}}} // namespace ripl::test::helper
