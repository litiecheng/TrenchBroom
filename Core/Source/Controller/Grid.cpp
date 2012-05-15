/*
 Copyright (C) 2010-2012 Kristian Duske
 
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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Grid.h"
#include "Utilities/VecMath.h"

namespace TrenchBroom {
    namespace Controller {
        unsigned int Grid::size() const {
            return m_size;
        }
        
        unsigned int Grid::actualSize() const {
            if (m_snap)
                return 1 << m_size;
            return 1;
        }

        float Grid::snap(float f) {
            int actSize = actualSize();
            return actSize * Math::fround(f / actSize);
        }

        Vec3f Grid::moveDelta(const BBox& bounds, const BBox& worldBounds, const Vec3f& delta, const Vec3f& direction) {
            Vec3f actualDelta;
            for (int i = 0; i < 3; i++) {
                float low  = snap(bounds.min[i] + delta[i]) - bounds.min[i];
                if (low > 0 != direction[i] > 0)
                    low = 0;
                float high = snap(bounds.max[i] + delta[i]) - bounds.max[i];
                if (high > 0 != direction[i] > 0)
                    high = 0;

                float dist = fabsf(high) < fabsf(low) ? high : low;
                actualDelta[i] = dist;
            }
            
            return actualDelta;
        }
        
    }
}

