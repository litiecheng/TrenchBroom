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

#ifndef TrenchBroom_MoveObjectTool_h
#define TrenchBroom_MoveObjectTool_h

#include "Controller/DragTool.h"

namespace TrenchBroom {
    namespace Controller {
        class Editor;
        
        class MoveObjectTool : public DragTool {
        protected:
            bool doBeginLeftDrag(ToolEvent& event, Vec3f& initialPoint);
            bool doLeftDrag(ToolEvent& event, const Vec3f& delta, const Vec3f& direction, Vec3f& nextRefPoint);
            void doEndLeftDrag(ToolEvent& event);
        public:
            MoveObjectTool(Editor& editor) : DragTool(editor) {}
        };
    }
}

#endif
