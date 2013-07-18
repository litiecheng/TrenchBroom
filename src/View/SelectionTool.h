/*
 Copyright (C) 2010-2013 Kristian Duske
 
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

#ifndef __TrenchBroom__SelectionTool__
#define __TrenchBroom__SelectionTool__

#include "View/Tool.h"

namespace TrenchBroom {
    namespace Controller {
        class ControllerFacade;
    }
    
    namespace View {
        class SelectionTool : public Tool<MousePolicy, MouseDragPolicy> {
        public:
            SelectionTool(BaseTool* next, Controller::ControllerFacade& controller);
        private:
            Controller::ControllerFacade& m_controller;
        private:
            bool doMouseUp(const InputState& inputState);
            bool doMouseDoubleClick(const InputState& inputState);
            bool doStartMouseDrag(const InputState& inputState);
            bool doMouseDrag(const InputState& inputState);
            void doEndMouseDrag(const InputState& inputState);
            void doCancelMouseDrag(const InputState& inputState);
        };
    }
}

#endif /* defined(__TrenchBroom__SelectionTool__) */
