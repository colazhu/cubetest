/***********************************************************************
    created:    Wed Jun 22 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "Core/Module.h"
#include "CEGUI/TplWRFactoryRegisterer.h"

// includes for types we create
#include "Core/Button.h"
#include "Core/Default.h"
#include "Core/Editbox.h"
#include "Core/FrameWindow.h"
#include "Core/ItemEntry.h"
#include "Core/ListHeader.h"
#include "Core/ListHeaderSegment.h"
#include "Core/Listbox.h"
#include "Core/Menubar.h"
#include "Core/MenuItem.h"
#include "Core/MultiColumnList.h"
#include "Core/MultiLineEditbox.h"
#include "Core/PopupMenu.h"
#include "Core/ProgressBar.h"
#include "Core/ScrollablePane.h"
#include "Core/Scrollbar.h"
#include "Core/Slider.h"
#include "Core/Static.h"
#include "Core/StaticImage.h"
#include "Core/StaticText.h"
#include "Core/TabButton.h"
#include "Core/TabControl.h"
#include "Core/Titlebar.h"
#include "Core/ToggleButton.h"
#include "Core/Tooltip.h"
#include "Core/ItemListbox.h"
#include "Core/Tree.h"

//----------------------------------------------------------------------------//
extern "C"
CEGUI::FactoryModule& getWindowRendererFactoryModule()
{
    static CEGUI::CoreWindowRendererModule mod;
    return mod;
}

namespace CEGUI
{
//----------------------------------------------------------------------------//
CoreWindowRendererModule::CoreWindowRendererModule()
{
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardButton>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardDefault>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardEditbox>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardFrameWindow>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardItemEntry>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardListHeader>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardListHeaderSegment>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardListbox>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardMenubar>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardMenuItem>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardMultiColumnList>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardMultiLineEditbox>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardPopupMenu>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardProgressBar>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardScrollablePane>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardScrollbar>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardSlider>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardStatic>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardStaticImage>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardStaticText>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardTabButton>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardTabControl>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardTitlebar>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardToggleButton>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardTooltip>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardItemListbox>());
    d_registry.push_back(CEGUI_NEW_AO TplWRFactoryRegisterer<FalagardTree>());
}

//----------------------------------------------------------------------------//
CoreWindowRendererModule::~CoreWindowRendererModule()
{
    FactoryRegistry::iterator i = d_registry.begin();
    for ( ; i != d_registry.end(); ++i)
        CEGUI_DELETE_AO (*i);
}

//----------------------------------------------------------------------------//

}
