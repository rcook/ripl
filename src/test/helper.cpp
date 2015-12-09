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
        if (width != image.cols)
        {
            return false;
        }

        if (height != image.rows)
        {
            return false;
        }

        if (width * height != data.size())
        {
            return false;
        }

        if (image.cols * image.rows != image.size)
        {
            return false;
        }

        if (!image.data)
        {
            return false;
        }

        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != image.data[i])
            {
                return false;
            }
        }

        return true;
    }
}}} // namespace ripl::test::helper
