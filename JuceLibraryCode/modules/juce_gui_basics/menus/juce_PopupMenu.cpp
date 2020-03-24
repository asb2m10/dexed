/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

namespace PopupMenuSettings
{
    const int scrollZone = 24;
    const int dismissCommandId = 0x6287345f;

    static bool menuWasHiddenBecauseOfAppChange = false;
}

//==============================================================================
struct PopupMenu::HelperClasses
{

class MouseSourceState;
struct MenuWindow;

static bool canBeTriggered (const PopupMenu::Item& item) noexcept        { return item.isEnabled && item.itemID != 0 && ! item.isSectionHeader; }
static bool hasActiveSubMenu (const PopupMenu::Item& item) noexcept      { return item.isEnabled && item.subMenu != nullptr && item.subMenu->items.size() > 0; }
static const Colour* getColour (const PopupMenu::Item& item) noexcept    { return item.colour != Colour() ? &item.colour : nullptr; }
static bool hasSubMenu (const PopupMenu::Item& item) noexcept            { return item.subMenu != nullptr && (item.itemID == 0 || item.subMenu->getNumItems() > 0); }

//==============================================================================
struct HeaderItemComponent  : public PopupMenu::CustomComponent
{
    HeaderItemComponent (const String& name)  : PopupMenu::CustomComponent (false)
    {
        setName (name);
    }

    void paint (Graphics& g) override;

    void getIdealSize (int& idealWidth, int& idealHeight) override
    {
        getLookAndFeel().getIdealPopupMenuItemSize (getName(), false, -1, idealWidth, idealHeight);
        idealHeight += idealHeight / 2;
        idealWidth += idealWidth / 4;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderItemComponent)
};

//==============================================================================
struct ItemComponent  : public Component
{
    ItemComponent (const PopupMenu::Item& i, int standardItemHeight, MenuWindow& parent)
      : item (i), customComp (i.customComponent)
    {
        if (item.isSectionHeader)
            customComp = *new HeaderItemComponent (item.text);

        if (customComp != nullptr)
        {
            setItem (*customComp, &item);
            addAndMakeVisible (*customComp);
        }

        parent.addAndMakeVisible (this);

        updateShortcutKeyDescription();

        int itemW = 80;
        int itemH = 16;
        getIdealSize (itemW, itemH, standardItemHeight);
        setSize (itemW, jlimit (1, 600, itemH));

        addMouseListener (&parent, false);
    }

    ~ItemComponent() override;

    void getIdealSize (int& idealWidth, int& idealHeight, const int standardItemHeight)
    {
        if (customComp != nullptr)
            customComp->getIdealSize (idealWidth, idealHeight);
        else
            getLookAndFeel().getIdealPopupMenuItemSize (getTextForMeasurement(),
                                                        item.isSeparator,
                                                        standardItemHeight,
                                                        idealWidth, idealHeight);
    }

    void paint (Graphics& g) override
    {
        if (customComp == nullptr)
            getLookAndFeel().drawPopupMenuItem (g, getLocalBounds(),
                                                item.isSeparator,
                                                item.isEnabled,
                                                isHighlighted,
                                                item.isTicked,
                                                hasSubMenu (item),
                                                item.text,
                                                item.shortcutKeyDescription,
                                                item.image.get(),
                                                getColour (item));
    }

    void resized() override
    {
        if (auto* child = getChildComponent (0))
            child->setBounds (getLocalBounds().reduced (getLookAndFeel().getPopupMenuBorderSize(), 0));
    }

    void setHighlighted (bool shouldBeHighlighted)
    {
        shouldBeHighlighted = shouldBeHighlighted && item.isEnabled;

        if (isHighlighted != shouldBeHighlighted)
        {
            isHighlighted = shouldBeHighlighted;

            if (customComp != nullptr)
                customComp->setHighlighted (shouldBeHighlighted);

            repaint();
        }
    }

    PopupMenu::Item item;

private:
    // NB: we use a copy of the one from the item info in case we're using our own section comp
    ReferenceCountedObjectPtr<CustomComponent> customComp;
    bool isHighlighted = false;

    void updateShortcutKeyDescription()
    {
        if (item.commandManager != nullptr
             && item.itemID != 0
             && item.shortcutKeyDescription.isEmpty())
        {
            String shortcutKey;

            for (auto& keypress : item.commandManager->getKeyMappings()
                                    ->getKeyPressesAssignedToCommand (item.itemID))
            {
                auto key = keypress.getTextDescriptionWithIcons();

                if (shortcutKey.isNotEmpty())
                    shortcutKey << ", ";

                if (key.length() == 1 && key[0] < 128)
                    shortcutKey << "shortcut: '" << key << '\'';
                else
                    shortcutKey << key;
            }

            item.shortcutKeyDescription = shortcutKey.trim();
        }
    }

    String getTextForMeasurement() const
    {
        return item.shortcutKeyDescription.isNotEmpty() ? item.text + "   " + item.shortcutKeyDescription
                                                        : item.text;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ItemComponent)
};

//==============================================================================
struct MenuWindow  : public Component
{
    MenuWindow (const PopupMenu& menu, MenuWindow* parentWindow,
                Options opts, bool alignToRectangle, bool shouldDismissOnMouseUp,
                ApplicationCommandManager** manager, float parentScaleFactor = 1.0f)
       : Component ("menu"),
         parent (parentWindow),
         options (std::move (opts)),
         managerOfChosenCommand (manager),
         componentAttachedTo (options.getTargetComponent()),
         dismissOnMouseUp (shouldDismissOnMouseUp),
         windowCreationTime (Time::getMillisecondCounter()),
         lastFocusedTime (windowCreationTime),
         timeEnteredCurrentChildComp (windowCreationTime),
         scaleFactor (parentWindow != nullptr ? parentScaleFactor : 1.0f)
    {
        setWantsKeyboardFocus (false);
        setMouseClickGrabsKeyboardFocus (false);
        setAlwaysOnTop (true);

        setLookAndFeel (parent != nullptr ? &(parent->getLookAndFeel())
                                          : menu.lookAndFeel.get());
        auto& lf = getLookAndFeel();

        parentComponent = lf.getParentComponentForMenuOptions (options);
        const_cast<Options&>(options) = options.withParentComponent (parentComponent);

        if (parentComponent == nullptr && parentWindow == nullptr && lf.shouldPopupMenuScaleWithTargetComponent (options))
            if (auto* targetComponent = options.getTargetComponent())
                scaleFactor = Component::getApproximateScaleFactorForComponent (targetComponent);

        setOpaque (lf.findColour (PopupMenu::backgroundColourId).isOpaque()
                     || ! Desktop::canUseSemiTransparentWindows());

        for (size_t i = 0; i < menu.items.size(); ++i)
        {
            auto& item = menu.items[i];

            if (i + 1 < menu.items.size() || ! item.isSeparator)
                items.add (new ItemComponent (item, options.getStandardItemHeight(), *this));
        }

        auto targetArea = options.getTargetScreenArea() / scaleFactor;

        calculateWindowPos (targetArea, alignToRectangle);
        setTopLeftPosition (windowPos.getPosition());
        updateYPositions();

        if (auto visibleID = options.getItemThatMustBeVisible())
        {
            auto targetPosition = parentComponent != nullptr ? parentComponent->getLocalPoint (nullptr, targetArea.getTopLeft())
                                                             : targetArea.getTopLeft();

            auto y = targetPosition.getY() - windowPos.getY();
            ensureItemIsVisible (visibleID, isPositiveAndBelow (y, windowPos.getHeight()) ? y : -1);
        }

        resizeToBestWindowPos();

        if (parentComponent != nullptr)
        {
            parentComponent->addChildComponent (this);
        }
        else
        {
            addToDesktop (ComponentPeer::windowIsTemporary
                          | ComponentPeer::windowIgnoresKeyPresses
                          | lf.getMenuWindowFlags());

            Desktop::getInstance().addGlobalMouseListener (this);
        }

        getActiveWindows().add (this);
        lf.preparePopupMenuWindow (*this);

        getMouseState (Desktop::getInstance().getMainMouseSource()); // forces creation of a mouse source watcher for the main mouse
    }

    ~MenuWindow() override;

    //==============================================================================
    void paint (Graphics& g) override
    {
        if (isOpaque())
            g.fillAll (Colours::white);

        getLookAndFeel().drawPopupMenuBackground (g, getWidth(), getHeight());
    }

    void paintOverChildren (Graphics& g) override
    {
        auto& lf = getLookAndFeel();

        if (parentComponent != nullptr)
            lf.drawResizableFrame (g, getWidth(), getHeight(),
                                   BorderSize<int> (getLookAndFeel().getPopupMenuBorderSize()));

        if (canScroll())
        {
            if (isTopScrollZoneActive())
                lf.drawPopupMenuUpDownArrow (g, getWidth(), PopupMenuSettings::scrollZone, true);

            if (isBottomScrollZoneActive())
            {
                g.setOrigin (0, getHeight() - PopupMenuSettings::scrollZone);
                lf.drawPopupMenuUpDownArrow (g, getWidth(), PopupMenuSettings::scrollZone, false);
            }
        }
    }

    //==============================================================================
    // hide this and all sub-comps
    void hide (const PopupMenu::Item* item, bool makeInvisible)
    {
        if (isVisible())
        {
            WeakReference<Component> deletionChecker (this);

            activeSubMenu.reset();
            currentChild = nullptr;

            if (item != nullptr
                 && item->commandManager != nullptr
                 && item->itemID != 0)
            {
                *managerOfChosenCommand = item->commandManager;
            }

            auto resultID = options.hasWatchedComponentBeenDeleted() ? 0 : getResultItemID (item);

            exitModalState (resultID);

            if (makeInvisible && deletionChecker != nullptr)
                setVisible (false);

            if (resultID != 0
                 && item != nullptr
                 && item->action != nullptr)
                MessageManager::callAsync (item->action);
        }
    }

    static int getResultItemID (const PopupMenu::Item* item)
    {
        if (item == nullptr)
            return 0;

        if (auto* cc = item->customCallback.get())
            if (! cc->menuItemTriggered())
                return 0;

        return item->itemID;
    }

    void dismissMenu (const PopupMenu::Item* item)
    {
        if (parent != nullptr)
        {
            parent->dismissMenu (item);
        }
        else
        {
            if (item != nullptr)
            {
                // need a copy of this on the stack as the one passed in will get deleted during this call
                auto mi (*item);
                hide (&mi, false);
            }
            else
            {
                hide (nullptr, false);
            }
        }
    }

    float getDesktopScaleFactor() const override    { return scaleFactor * Desktop::getInstance().getGlobalScaleFactor(); }

    //==============================================================================
    bool keyPressed (const KeyPress& key) override
    {
        if (key.isKeyCode (KeyPress::downKey))
        {
            selectNextItem (1);
        }
        else if (key.isKeyCode (KeyPress::upKey))
        {
            selectNextItem (-1);
        }
        else if (key.isKeyCode (KeyPress::leftKey))
        {
            if (parent != nullptr)
            {
                Component::SafePointer<MenuWindow> parentWindow (parent);
                ItemComponent* currentChildOfParent = parentWindow->currentChild;

                hide (nullptr, true);

                if (parentWindow != nullptr)
                    parentWindow->setCurrentlyHighlightedChild (currentChildOfParent);

                disableTimerUntilMouseMoves();
            }
            else if (componentAttachedTo != nullptr)
            {
                componentAttachedTo->keyPressed (key);
            }
        }
        else if (key.isKeyCode (KeyPress::rightKey))
        {
            disableTimerUntilMouseMoves();

            if (showSubMenuFor (currentChild))
            {
                if (isSubMenuVisible())
                    activeSubMenu->selectNextItem (1);
            }
            else if (componentAttachedTo != nullptr)
            {
                componentAttachedTo->keyPressed (key);
            }
        }
        else if (key.isKeyCode (KeyPress::returnKey))
        {
            triggerCurrentlyHighlightedItem();
        }
        else if (key.isKeyCode (KeyPress::escapeKey))
        {
            dismissMenu (nullptr);
        }
        else
        {
            return false;
        }

        return true;
    }

    void inputAttemptWhenModal() override
    {
        WeakReference<Component> deletionChecker (this);

        for (auto* ms : mouseSourceStates)
        {
            ms->timerCallback();

            if (deletionChecker == nullptr)
                return;
        }

        if (! isOverAnyMenu())
        {
            if (componentAttachedTo != nullptr)
            {
                // we want to dismiss the menu, but if we do it synchronously, then
                // the mouse-click will be allowed to pass through. That's good, except
                // when the user clicks on the button that originally popped the menu up,
                // as they'll expect the menu to go away, and in fact it'll just
                // come back. So only dismiss synchronously if they're not on the original
                // comp that we're attached to.
                auto mousePos = componentAttachedTo->getMouseXYRelative();

                if (componentAttachedTo->reallyContains (mousePos, true))
                {
                    postCommandMessage (PopupMenuSettings::dismissCommandId); // dismiss asynchronously
                    return;
                }
            }

            dismissMenu (nullptr);
        }
    }

    void handleCommandMessage (int commandId) override
    {
        Component::handleCommandMessage (commandId);

        if (commandId == PopupMenuSettings::dismissCommandId)
            dismissMenu (nullptr);
    }

    //==============================================================================
    void mouseMove  (const MouseEvent& e) override    { handleMouseEvent (e); }
    void mouseDown  (const MouseEvent& e) override    { handleMouseEvent (e); }
    void mouseDrag  (const MouseEvent& e) override    { handleMouseEvent (e); }
    void mouseUp    (const MouseEvent& e) override    { handleMouseEvent (e); }

    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel) override
    {
        alterChildYPos (roundToInt (-10.0f * wheel.deltaY * PopupMenuSettings::scrollZone));
    }

    void handleMouseEvent (const MouseEvent& e)
    {
        getMouseState (e.source).handleMouseEvent (e);
    }

    bool windowIsStillValid()
    {
        if (! isVisible())
            return false;

        if (componentAttachedTo != options.getTargetComponent())
        {
            dismissMenu (nullptr);
            return false;
        }

        if (auto* currentlyModalWindow = dynamic_cast<MenuWindow*> (Component::getCurrentlyModalComponent()))
            if (! treeContains (currentlyModalWindow))
                return false;

        return true;
    }

    static Array<MenuWindow*>& getActiveWindows()
    {
        static Array<MenuWindow*> activeMenuWindows;
        return activeMenuWindows;
    }

    MouseSourceState& getMouseState (MouseInputSource source)
    {
        MouseSourceState* mouseState = nullptr;

        for (auto* ms : mouseSourceStates)
        {
            if      (ms->source == source)                        mouseState = ms;
            else if (ms->source.getType() != source.getType())    ms->stopTimer();
        }

        if (mouseState == nullptr)
        {
            mouseState = new MouseSourceState (*this, source);
            mouseSourceStates.add (mouseState);
        }

        return *mouseState;
    }

    //==============================================================================
    bool isOverAnyMenu() const
    {
        return parent != nullptr ? parent->isOverAnyMenu()
                                 : isOverChildren();
    }

    bool isOverChildren() const
    {
        return isVisible()
                && (isAnyMouseOver() || (activeSubMenu != nullptr && activeSubMenu->isOverChildren()));
    }

    bool isAnyMouseOver() const
    {
        for (auto* ms : mouseSourceStates)
            if (ms->isOver())
                return true;

        return false;
    }

    bool treeContains (const MenuWindow* const window) const noexcept
    {
        auto* mw = this;

        while (mw->parent != nullptr)
            mw = mw->parent;

        while (mw != nullptr)
        {
            if (mw == window)
                return true;

            mw = mw->activeSubMenu.get();
        }

        return false;
    }

    bool doesAnyJuceCompHaveFocus()
    {
        bool anyFocused = Process::isForegroundProcess();

        if (anyFocused && Component::getCurrentlyFocusedComponent() == nullptr)
        {
            // because no component at all may have focus, our test here will
            // only be triggered when something has focus and then loses it.
            anyFocused = ! hasAnyJuceCompHadFocus;

            for (int i = ComponentPeer::getNumPeers(); --i >= 0;)
            {
                if (ComponentPeer::getPeer (i)->isFocused())
                {
                    anyFocused = true;
                    hasAnyJuceCompHadFocus = true;
                    break;
                }
            }
        }

        return anyFocused;
    }

    //==============================================================================
    Rectangle<int> getParentArea (Point<int> targetPoint, Component* relativeTo = nullptr)
    {
        if (relativeTo != nullptr)
            targetPoint = relativeTo->localPointToGlobal (targetPoint);

        auto parentArea = Desktop::getInstance().getDisplays().findDisplayForPoint (targetPoint * scaleFactor)
                              #if JUCE_MAC || JUCE_ANDROID
                               .userArea;
                              #else
                               .totalArea; // on windows, don't stop the menu overlapping the taskbar
                              #endif

        if (parentComponent == nullptr)
            return parentArea;

        return parentComponent->getLocalArea (nullptr,
                                              parentComponent->getScreenBounds()
                                                    .reduced (getLookAndFeel().getPopupMenuBorderSize())
                                                    .getIntersection (parentArea));
    }

    void calculateWindowPos (Rectangle<int> target, const bool alignToRectangle)
    {
        auto parentArea = getParentArea (target.getCentre()) / scaleFactor;

        if (parentComponent != nullptr)
            target = parentComponent->getLocalArea (nullptr, target).getIntersection (parentArea);

        auto maxMenuHeight = parentArea.getHeight() - 24;

        int x, y, widthToUse, heightToUse;
        layoutMenuItems (parentArea.getWidth() - 24, maxMenuHeight, widthToUse, heightToUse);

        if (alignToRectangle)
        {
            x = target.getX();

            auto spaceUnder = parentArea.getBottom() - target.getBottom();
            auto spaceOver = target.getY() - parentArea.getY();
            auto bufferHeight = 30;

            if (options.getPreferredPopupDirection() == Options::PopupDirection::upwards)
                y = (heightToUse < spaceOver - bufferHeight  || spaceOver >= spaceUnder) ? target.getY() - heightToUse
                                                                                         : target.getBottom();
            else
                y = (heightToUse < spaceUnder - bufferHeight || spaceUnder >= spaceOver) ? target.getBottom()
                                                                                         : target.getY() - heightToUse;
        }
        else
        {
            bool tendTowardsRight = target.getCentreX() < parentArea.getCentreX();

            if (parent != nullptr)
            {
                if (parent->parent != nullptr)
                {
                    const bool parentGoingRight = (parent->getX() + parent->getWidth() / 2
                                                    > parent->parent->getX() + parent->parent->getWidth() / 2);

                    if (parentGoingRight && target.getRight() + widthToUse < parentArea.getRight() - 4)
                        tendTowardsRight = true;
                    else if ((! parentGoingRight) && target.getX() > widthToUse + 4)
                        tendTowardsRight = false;
                }
                else if (target.getRight() + widthToUse < parentArea.getRight() - 32)
                {
                    tendTowardsRight = true;
                }
            }

            auto biggestSpace = jmax (parentArea.getRight() - target.getRight(),
                                      target.getX() - parentArea.getX()) - 32;

            if (biggestSpace < widthToUse)
            {
                layoutMenuItems (biggestSpace + target.getWidth() / 3, maxMenuHeight, widthToUse, heightToUse);

                if (numColumns > 1)
                    layoutMenuItems (biggestSpace - 4, maxMenuHeight, widthToUse, heightToUse);

                tendTowardsRight = (parentArea.getRight() - target.getRight()) >= (target.getX() - parentArea.getX());
            }

            x = tendTowardsRight ? jmin (parentArea.getRight() - widthToUse - 4, target.getRight())
                                 : jmax (parentArea.getX() + 4, target.getX() - widthToUse);

            if (getLookAndFeel().getPopupMenuBorderSize() == 0) // workaround for dismissing the window on mouse up when border size is 0
                x += tendTowardsRight ? 1 : -1;

            y = target.getCentreY() > parentArea.getCentreY() ? jmax (parentArea.getY(), target.getBottom() - heightToUse)
                                                              : target.getY();
        }

        x = jmax (parentArea.getX() + 1, jmin (parentArea.getRight()  - (widthToUse  + 6), x));
        y = jmax (parentArea.getY() + 1, jmin (parentArea.getBottom() - (heightToUse + 6), y));

        windowPos.setBounds (x, y, widthToUse, heightToUse);

        // sets this flag if it's big enough to obscure any of its parent menus
        hideOnExit = parent != nullptr
                      && parent->windowPos.intersects (windowPos.expanded (-4, -4));
    }

    void layoutMenuItems (const int maxMenuW, const int maxMenuH, int& width, int& height)
    {
        numColumns = options.getMinimumNumColumns();
        contentHeight = 0;

        auto maximumNumColumns = options.getMaximumNumColumns() > 0 ? options.getMaximumNumColumns() : 7;

        for (;;)
        {
            auto totalW = workOutBestSize (maxMenuW);

            if (totalW > maxMenuW)
            {
                numColumns = jmax (1, numColumns - 1);
                workOutBestSize (maxMenuW); // to update col widths
                break;
            }

            if (totalW > maxMenuW / 2
                 || contentHeight < maxMenuH
                 || numColumns >= maximumNumColumns)
                break;

            ++numColumns;
        }

        auto actualH = jmin (contentHeight, maxMenuH);

        needsToScroll = contentHeight > actualH;

        width = updateYPositions();
        height = actualH + getLookAndFeel().getPopupMenuBorderSize() * 2;
    }

    int workOutBestSize (const int maxMenuW)
    {
        int totalW = 0;
        contentHeight = 0;
        int childNum = 0;

        for (int col = 0; col < numColumns; ++col)
        {
            int colW = options.getStandardItemHeight(), colH = 0;

            auto numChildren = jmin (items.size() - childNum,
                                     (items.size() + numColumns - 1) / numColumns);

            for (int i = numChildren; --i >= 0;)
            {
                colW = jmax (colW, items.getUnchecked (childNum + i)->getWidth());
                colH += items.getUnchecked (childNum + i)->getHeight();
            }

            colW = jmin (maxMenuW / jmax (1, numColumns - 2), colW + getLookAndFeel().getPopupMenuBorderSize() * 2);

            columnWidths.set (col, colW);
            totalW += colW;
            contentHeight = jmax (contentHeight, colH);

            childNum += numChildren;
        }

        // width must never be larger than the screen
        auto minWidth = jmin (maxMenuW, options.getMinimumWidth());

        if (totalW < minWidth)
        {
            totalW = minWidth;

            for (int col = 0; col < numColumns; ++col)
                columnWidths.set (0, totalW / numColumns);
        }

        return totalW;
    }

    void ensureItemIsVisible (const int itemID, int wantedY)
    {
        jassert (itemID != 0);

        for (int i = items.size(); --i >= 0;)
        {
            if (auto* m = items.getUnchecked (i))
            {
                if (m->item.itemID == itemID
                     && windowPos.getHeight() > PopupMenuSettings::scrollZone * 4)
                {
                    auto currentY = m->getY();

                    if (wantedY > 0 || currentY < 0 || m->getBottom() > windowPos.getHeight())
                    {
                        if (wantedY < 0)
                            wantedY = jlimit (PopupMenuSettings::scrollZone,
                                              jmax (PopupMenuSettings::scrollZone,
                                                    windowPos.getHeight() - (PopupMenuSettings::scrollZone + m->getHeight())),
                                              currentY);

                        auto parentArea = getParentArea (windowPos.getPosition(), parentComponent) / scaleFactor;
                        auto deltaY = wantedY - currentY;

                        windowPos.setSize (jmin (windowPos.getWidth(), parentArea.getWidth()),
                                           jmin (windowPos.getHeight(), parentArea.getHeight()));

                        auto newY = jlimit (parentArea.getY(),
                                            parentArea.getBottom() - windowPos.getHeight(),
                                            windowPos.getY() + deltaY);

                        deltaY -= newY - windowPos.getY();

                        childYOffset -= deltaY;
                        windowPos.setPosition (windowPos.getX(), newY);

                        updateYPositions();
                    }

                    break;
                }
            }
        }
    }

    void resizeToBestWindowPos()
    {
        auto r = windowPos;

        if (childYOffset < 0)
        {
            r = r.withTop (r.getY() - childYOffset);
        }
        else if (childYOffset > 0)
        {
            auto spaceAtBottom = r.getHeight() - (contentHeight - childYOffset);

            if (spaceAtBottom > 0)
                r.setSize (r.getWidth(), r.getHeight() - spaceAtBottom);
        }

        setBounds (r);
        updateYPositions();
    }

    void alterChildYPos (int delta)
    {
        if (canScroll())
        {
            childYOffset += delta;

            if (delta < 0)
                childYOffset = jmax (childYOffset, 0);
            else if (delta > 0)
                childYOffset = jmin (childYOffset,
                                     contentHeight - windowPos.getHeight() + getLookAndFeel().getPopupMenuBorderSize());

            updateYPositions();
        }
        else
        {
            childYOffset = 0;
        }

        resizeToBestWindowPos();
        repaint();
    }

    int updateYPositions()
    {
        int x = 0;
        int childNum = 0;

        for (int col = 0; col < numColumns; ++col)
        {
            auto numChildren = jmin (items.size() - childNum,
                                     (items.size() + numColumns - 1) / numColumns);

            auto colW = columnWidths[col];
            auto y = getLookAndFeel().getPopupMenuBorderSize() - (childYOffset + (getY() - windowPos.getY()));

            for (int i = 0; i < numChildren; ++i)
            {
                auto* c = items.getUnchecked (childNum + i);
                c->setBounds (x, y, colW, c->getHeight());
                y += c->getHeight();
            }

            x += colW;
            childNum += numChildren;
        }

        return x;
    }

    void setCurrentlyHighlightedChild (ItemComponent* child)
    {
        if (currentChild != nullptr)
            currentChild->setHighlighted (false);

        currentChild = child;

        if (currentChild != nullptr)
        {
            currentChild->setHighlighted (true);
            timeEnteredCurrentChildComp = Time::getApproximateMillisecondCounter();
        }
    }

    bool isSubMenuVisible() const noexcept          { return activeSubMenu != nullptr && activeSubMenu->isVisible(); }

    bool showSubMenuFor (ItemComponent* childComp)
    {
        activeSubMenu.reset();

        if (childComp != nullptr
             && hasActiveSubMenu (childComp->item))
        {
            activeSubMenu.reset (new HelperClasses::MenuWindow (*(childComp->item.subMenu), this,
                                                                options.withTargetScreenArea (childComp->getScreenBounds())
                                                                       .withMinimumWidth (0)
                                                                       .withTargetComponent (nullptr)
                                                                       .withParentComponent (parentComponent),
                                                                false, dismissOnMouseUp, managerOfChosenCommand, scaleFactor));

            activeSubMenu->setVisible (true); // (must be called before enterModalState on Windows to avoid DropShadower confusion)
            activeSubMenu->enterModalState (false);
            activeSubMenu->toFront (false);
            return true;
        }

        return false;
    }

    void triggerCurrentlyHighlightedItem()
    {
        if (currentChild != nullptr
             && canBeTriggered (currentChild->item)
             && (currentChild->item.customComponent == nullptr
                  || currentChild->item.customComponent->isTriggeredAutomatically()))
        {
            dismissMenu (&currentChild->item);
        }
    }

    void selectNextItem (int delta)
    {
        disableTimerUntilMouseMoves();

        auto start = jmax (0, items.indexOf (currentChild));

        for (int i = items.size(); --i >= 0;)
        {
            start += delta;

            if (auto* mic = items.getUnchecked ((start + items.size()) % items.size()))
            {
                if (canBeTriggered (mic->item) || hasActiveSubMenu (mic->item))
                {
                    setCurrentlyHighlightedChild (mic);
                    break;
                }
            }
        }
    }

    void disableTimerUntilMouseMoves()
    {
        disableMouseMoves = true;

        if (parent != nullptr)
            parent->disableTimerUntilMouseMoves();
    }

    bool canScroll() const noexcept                 { return childYOffset != 0 || needsToScroll; }
    bool isTopScrollZoneActive() const noexcept     { return canScroll() && childYOffset > 0; }
    bool isBottomScrollZoneActive() const noexcept  { return canScroll() && childYOffset < contentHeight - windowPos.getHeight(); }

    //==============================================================================
    MenuWindow* parent;
    const Options options;
    OwnedArray<ItemComponent> items;
    ApplicationCommandManager** managerOfChosenCommand;
    WeakReference<Component> componentAttachedTo;
    Component* parentComponent = nullptr;
    Rectangle<int> windowPos;
    bool hasBeenOver = false, needsToScroll = false;
    bool dismissOnMouseUp, hideOnExit = false, disableMouseMoves = false, hasAnyJuceCompHadFocus = false;
    int numColumns = 0, contentHeight = 0, childYOffset = 0;
    Component::SafePointer<ItemComponent> currentChild;
    std::unique_ptr<MenuWindow> activeSubMenu;
    Array<int> columnWidths;
    uint32 windowCreationTime, lastFocusedTime, timeEnteredCurrentChildComp;
    OwnedArray<MouseSourceState> mouseSourceStates;
    float scaleFactor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuWindow)
};

//==============================================================================
class MouseSourceState  : public Timer
{
public:
    MouseSourceState (MenuWindow& w, MouseInputSource s)
        : window (w), source (s), lastScrollTime (Time::getMillisecondCounter())
    {
        startTimerHz (20);
    }

    void handleMouseEvent (const MouseEvent& e);

    void timerCallback() override
    {
       #if JUCE_WINDOWS
        // touch and pen devices on Windows send an offscreen mouse move after mouse up events
        // but we don't want to forward these on as they will dismiss the menu
        if ((source.isTouch() || source.isPen()) && ! isValidMousePosition())
            return;
       #endif

        if (window.windowIsStillValid())
            handleMousePosition (source.getScreenPosition().roundToInt());
    }

    bool isOver() const
    {
        return window.reallyContains (window.getLocalPoint (nullptr, source.getScreenPosition()).roundToInt(), true);
    }

    MenuWindow& window;
    MouseInputSource source;

private:
    Point<int> lastMousePos;
    double scrollAcceleration = 0;
    uint32 lastScrollTime, lastMouseMoveTime = 0;
    bool isDown = false;

    void handleMousePosition (Point<int> globalMousePos)
    {
        auto localMousePos = window.getLocalPoint (nullptr, globalMousePos);
        auto timeNow = Time::getMillisecondCounter();

        if (timeNow > window.timeEnteredCurrentChildComp + 100
             && window.reallyContains (localMousePos, true)
             && window.currentChild != nullptr
             && ! (window.disableMouseMoves || window.isSubMenuVisible()))
        {
            window.showSubMenuFor (window.currentChild);
        }

        highlightItemUnderMouse (globalMousePos, localMousePos, timeNow);

        const bool overScrollArea = scrollIfNecessary (localMousePos, timeNow);
        const bool isOverAny = window.isOverAnyMenu();

        if (window.hideOnExit && window.hasBeenOver && ! isOverAny)
            window.hide (nullptr, true);
        else
            checkButtonState (localMousePos, timeNow, isDown, overScrollArea, isOverAny);
    }

    void checkButtonState (Point<int> localMousePos, const uint32 timeNow,
                           const bool wasDown, const bool overScrollArea, const bool isOverAny)
    {
        isDown = window.hasBeenOver
                    && (ModifierKeys::currentModifiers.isAnyMouseButtonDown()
                         || ComponentPeer::getCurrentModifiersRealtime().isAnyMouseButtonDown());

        if (! window.doesAnyJuceCompHaveFocus())
        {
            if (timeNow > window.lastFocusedTime + 10)
            {
                PopupMenuSettings::menuWasHiddenBecauseOfAppChange = true;
                window.dismissMenu (nullptr);
                // Note: This object may have been deleted by the previous call.
            }
        }
        else if (wasDown && timeNow > window.windowCreationTime + 250
                   && ! (isDown || overScrollArea))
        {
            if (window.reallyContains (localMousePos, true))
                window.triggerCurrentlyHighlightedItem();
            else if ((window.hasBeenOver || ! window.dismissOnMouseUp) && ! isOverAny)
                window.dismissMenu (nullptr);

            // Note: This object may have been deleted by the previous call.
        }
        else
        {
            window.lastFocusedTime = timeNow;
        }
    }

    void highlightItemUnderMouse (Point<int> globalMousePos, Point<int> localMousePos, const uint32 timeNow)
    {
        if (globalMousePos != lastMousePos || timeNow > lastMouseMoveTime + 350)
        {
            const bool isMouseOver = window.reallyContains (localMousePos, true);

            if (isMouseOver)
                window.hasBeenOver = true;

            if (lastMousePos.getDistanceFrom (globalMousePos) > 2)
            {
                lastMouseMoveTime = timeNow;

                if (window.disableMouseMoves && isMouseOver)
                    window.disableMouseMoves = false;
            }

            if (window.disableMouseMoves || (window.activeSubMenu != nullptr && window.activeSubMenu->isOverChildren()))
                return;

            const bool isMovingTowardsMenu = isMouseOver && globalMousePos != lastMousePos
                                                && isMovingTowardsSubmenu (globalMousePos);

            lastMousePos = globalMousePos;

            if (! isMovingTowardsMenu)
            {
                auto* c = window.getComponentAt (localMousePos);

                if (c == &window)
                    c = nullptr;

                auto* itemUnderMouse = dynamic_cast<ItemComponent*> (c);

                if (itemUnderMouse == nullptr && c != nullptr)
                    itemUnderMouse = c->findParentComponentOfClass<ItemComponent>();

                if (itemUnderMouse != window.currentChild
                      && (isMouseOver || (window.activeSubMenu == nullptr) || ! window.activeSubMenu->isVisible()))
                {
                    if (isMouseOver && (c != nullptr) && (window.activeSubMenu != nullptr))
                        window.activeSubMenu->hide (nullptr, true);

                    if (! isMouseOver)
                        itemUnderMouse = nullptr;

                    window.setCurrentlyHighlightedChild (itemUnderMouse);
                }
            }
        }
    }

    bool isMovingTowardsSubmenu (Point<int> newGlobalPos) const
    {
        if (window.activeSubMenu == nullptr)
            return false;

        // try to intelligently guess whether the user is moving the mouse towards a currently-open
        // submenu. To do this, look at whether the mouse stays inside a triangular region that
        // extends from the last mouse pos to the submenu's rectangle..

        auto itemScreenBounds = window.activeSubMenu->getScreenBounds();
        auto subX = (float) itemScreenBounds.getX();

        auto oldGlobalPos = lastMousePos;

        if (itemScreenBounds.getX() > window.getX())
        {
            oldGlobalPos -= Point<int> (2, 0);  // to enlarge the triangle a bit, in case the mouse only moves a couple of pixels
        }
        else
        {
            oldGlobalPos += Point<int> (2, 0);
            subX += itemScreenBounds.getWidth();
        }

        Path areaTowardsSubMenu;
        areaTowardsSubMenu.addTriangle ((float) oldGlobalPos.x, (float) oldGlobalPos.y,
                                        subX, (float) itemScreenBounds.getY(),
                                        subX, (float) itemScreenBounds.getBottom());

        return areaTowardsSubMenu.contains (newGlobalPos.toFloat());
    }

    bool scrollIfNecessary (Point<int> localMousePos, const uint32 timeNow)
    {
        if (window.canScroll()
             && isPositiveAndBelow (localMousePos.x, window.getWidth())
             && (isPositiveAndBelow (localMousePos.y, window.getHeight()) || source.isDragging()))
        {
            if (window.isTopScrollZoneActive() && localMousePos.y < PopupMenuSettings::scrollZone)
                return scroll (timeNow, -1);

            if (window.isBottomScrollZoneActive() && localMousePos.y > window.getHeight() - PopupMenuSettings::scrollZone)
                return scroll (timeNow, 1);
        }

        scrollAcceleration = 1.0;
        return false;
    }

    bool scroll (const uint32 timeNow, const int direction)
    {
        if (timeNow > lastScrollTime + 20)
        {
            scrollAcceleration = jmin (4.0, scrollAcceleration * 1.04);
            int amount = 0;

            for (int i = 0; i < window.items.size() && amount == 0; ++i)
                amount = ((int) scrollAcceleration) * window.items.getUnchecked (i)->getHeight();

            window.alterChildYPos (amount * direction);
            lastScrollTime = timeNow;
        }

        return true;
    }

   #if JUCE_WINDOWS
    bool isValidMousePosition()
    {
        auto screenPos = source.getScreenPosition();
        auto localPos = (window.activeSubMenu == nullptr) ? window.getLocalPoint (nullptr, screenPos)
                                                          : window.activeSubMenu->getLocalPoint (nullptr, screenPos);

        if (localPos.x < 0 && localPos.y < 0)
            return false;

        return true;
    }
   #endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MouseSourceState)
};

//==============================================================================
struct NormalComponentWrapper : public PopupMenu::CustomComponent
{
    NormalComponentWrapper (Component& comp, int w, int h, bool triggerMenuItemAutomaticallyWhenClicked)
        : PopupMenu::CustomComponent (triggerMenuItemAutomaticallyWhenClicked),
          width (w), height (h)
    {
        addAndMakeVisible (comp);
    }

    void getIdealSize (int& idealWidth, int& idealHeight) override;

    void resized() override
    {
        if (auto* child = getChildComponent (0))
            child->setBounds (getLocalBounds());
    }

    const int width, height;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NormalComponentWrapper)
};

};

// The following implementations are outside of the class definitions to avoid spurious
// warning messages when dynamically loading libraries at runtime on macOS
void PopupMenu::HelperClasses::HeaderItemComponent::paint (Graphics& g)
{
    getLookAndFeel().drawPopupMenuSectionHeader (g, getLocalBounds(), getName());
}

PopupMenu::HelperClasses::ItemComponent::~ItemComponent()
{
    if (customComp != nullptr)
        setItem (*customComp, nullptr);

    removeChildComponent (customComp.get());
}

PopupMenu::HelperClasses::MenuWindow::~MenuWindow()
{
    getActiveWindows().removeFirstMatchingValue (this);
    Desktop::getInstance().removeGlobalMouseListener (this);
    activeSubMenu.reset();
    items.clear();
}

void PopupMenu::HelperClasses::MouseSourceState::handleMouseEvent (const MouseEvent& e)
{
    if (! window.windowIsStillValid())
        return;

    startTimerHz (20);
    handleMousePosition (e.getScreenPosition());
}

void PopupMenu::HelperClasses::NormalComponentWrapper::getIdealSize (int& idealWidth, int& idealHeight)
{
    idealWidth = width;
    idealHeight = height;
}

//==============================================================================
PopupMenu::PopupMenu()
{
}

PopupMenu::PopupMenu (const PopupMenu& other)
    : items (other.items),
      lookAndFeel (other.lookAndFeel)
{
}

PopupMenu& PopupMenu::operator= (const PopupMenu& other)
{
    if (this != &other)
    {
        items = other.items;
        lookAndFeel = other.lookAndFeel;
    }

    return *this;
}

PopupMenu::PopupMenu (PopupMenu&& other) noexcept
    : items (std::move (other.items)),
      lookAndFeel (std::move (other.lookAndFeel))
{
}

PopupMenu& PopupMenu::operator= (PopupMenu&& other) noexcept
{
    items = std::move (other.items);
    lookAndFeel = other.lookAndFeel;
    return *this;
}

PopupMenu::~PopupMenu() = default;

void PopupMenu::clear()
{
    items.clear();
}

//==============================================================================
PopupMenu::Item::Item() = default;
PopupMenu::Item::Item (String t) : text (std::move (t)), itemID (-1) {}

PopupMenu::Item::Item (Item&&) = default;
PopupMenu::Item& PopupMenu::Item::operator= (Item&&) = default;

PopupMenu::Item::Item (const Item& other)
  : text (other.text),
    itemID (other.itemID),
    action (other.action),
    subMenu (createCopyIfNotNull (other.subMenu.get())),
    image (other.image != nullptr ? other.image->createCopy() : nullptr),
    customComponent (other.customComponent),
    customCallback (other.customCallback),
    commandManager (other.commandManager),
    shortcutKeyDescription (other.shortcutKeyDescription),
    colour (other.colour),
    isEnabled (other.isEnabled),
    isTicked (other.isTicked),
    isSeparator (other.isSeparator),
    isSectionHeader (other.isSectionHeader)
{
}

PopupMenu::Item& PopupMenu::Item::operator= (const Item& other)
{
    text = other.text;
    itemID = other.itemID;
    action = other.action;
    subMenu.reset (createCopyIfNotNull (other.subMenu.get()));
    image = other.image != nullptr ? other.image->createCopy() : std::unique_ptr<Drawable>();
    customComponent = other.customComponent;
    customCallback = other.customCallback;
    commandManager = other.commandManager;
    shortcutKeyDescription = other.shortcutKeyDescription;
    colour = other.colour;
    isEnabled = other.isEnabled;
    isTicked = other.isTicked;
    isSeparator = other.isSeparator;
    isSectionHeader = other.isSectionHeader;
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setTicked (bool shouldBeTicked) & noexcept
{
    isTicked = shouldBeTicked;
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setEnabled (bool shouldBeEnabled) & noexcept
{
    isEnabled = shouldBeEnabled;
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setAction (std::function<void()> newAction) & noexcept
{
    action = std::move (newAction);
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setID (int newID) & noexcept
{
    itemID = newID;
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setColour (Colour newColour) & noexcept
{
    colour = newColour;
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setCustomComponent (ReferenceCountedObjectPtr<CustomComponent> comp) & noexcept
{
    customComponent = comp;
    return *this;
}

PopupMenu::Item& PopupMenu::Item::setImage (std::unique_ptr<Drawable> newImage) & noexcept
{
    image = std::move (newImage);
    return *this;
}

PopupMenu::Item&& PopupMenu::Item::setTicked (bool shouldBeTicked) && noexcept
{
    isTicked = shouldBeTicked;
    return std::move (*this);
}

PopupMenu::Item&& PopupMenu::Item::setEnabled (bool shouldBeEnabled) && noexcept
{
    isEnabled = shouldBeEnabled;
    return std::move (*this);
}

PopupMenu::Item&& PopupMenu::Item::setAction (std::function<void()> newAction) && noexcept
{
    action = std::move (newAction);
    return std::move (*this);
}

PopupMenu::Item&& PopupMenu::Item::setID (int newID) && noexcept
{
    itemID = newID;
    return std::move (*this);
}

PopupMenu::Item&& PopupMenu::Item::setColour (Colour newColour) && noexcept
{
    colour = newColour;
    return std::move (*this);
}

PopupMenu::Item&& PopupMenu::Item::setCustomComponent (ReferenceCountedObjectPtr<CustomComponent> comp) && noexcept
{
    customComponent = comp;
    return std::move (*this);
}

PopupMenu::Item&& PopupMenu::Item::setImage (std::unique_ptr<Drawable> newImage) && noexcept
{
    image = std::move (newImage);
    return std::move (*this);
}

void PopupMenu::addItem (Item newItem)
{
    // An ID of 0 is used as a return value to indicate that the user
    // didn't pick anything, so you shouldn't use it as the ID for an item..
    jassert (newItem.itemID != 0
              || newItem.isSeparator || newItem.isSectionHeader
              || newItem.subMenu != nullptr);

    items.push_back (std::move (newItem));
}

void PopupMenu::addItem (String itemText, std::function<void()> action)
{
    addItem (std::move (itemText), true, false, std::move (action));
}

void PopupMenu::addItem (String itemText, bool isActive, bool isTicked, std::function<void()> action)
{
    Item i (std::move (itemText));
    i.action = std::move (action);
    i.isEnabled = isActive;
    i.isTicked = isTicked;
    addItem (std::move (i));
}

void PopupMenu::addItem (int itemResultID, String itemText, bool isActive, bool isTicked)
{
    Item i (std::move (itemText));
    i.itemID = itemResultID;
    i.isEnabled = isActive;
    i.isTicked = isTicked;
    addItem (std::move (i));
}

static std::unique_ptr<Drawable> createDrawableFromImage (const Image& im)
{
    if (im.isValid())
    {
        auto d = new DrawableImage();
        d->setImage (im);
        return std::unique_ptr<Drawable> (d);
    }

    return {};
}

void PopupMenu::addItem (int itemResultID, String itemText, bool isActive, bool isTicked, const Image& iconToUse)
{
    addItem (itemResultID, std::move (itemText), isActive, isTicked, createDrawableFromImage (iconToUse));
}

void PopupMenu::addItem (int itemResultID, String itemText, bool isActive,
                         bool isTicked, std::unique_ptr<Drawable> iconToUse)
{
    Item i (std::move (itemText));
    i.itemID = itemResultID;
    i.isEnabled = isActive;
    i.isTicked = isTicked;
    i.image = std::move (iconToUse);
    addItem (std::move (i));
}

void PopupMenu::addCommandItem (ApplicationCommandManager* commandManager,
                                const CommandID commandID,
                                String displayName,
                                std::unique_ptr<Drawable> iconToUse)
{
    jassert (commandManager != nullptr && commandID != 0);

    if (auto* registeredInfo = commandManager->getCommandForID (commandID))
    {
        ApplicationCommandInfo info (*registeredInfo);
        auto* target = commandManager->getTargetForCommand (commandID, info);

        Item i;
        i.text = displayName.isNotEmpty() ? std::move (displayName) : info.shortName;
        i.itemID = (int) commandID;
        i.commandManager = commandManager;
        i.isEnabled = target != nullptr && (info.flags & ApplicationCommandInfo::isDisabled) == 0;
        i.isTicked = (info.flags & ApplicationCommandInfo::isTicked) != 0;
        i.image = std::move (iconToUse);
        addItem (std::move (i));
    }
}

void PopupMenu::addColouredItem (int itemResultID, String itemText, Colour itemTextColour,
                                 bool isActive, bool isTicked, std::unique_ptr<Drawable> iconToUse)
{
    Item i (std::move (itemText));
    i.itemID = itemResultID;
    i.colour = itemTextColour;
    i.isEnabled = isActive;
    i.isTicked = isTicked;
    i.image = std::move (iconToUse);
    addItem (std::move (i));
}

void PopupMenu::addColouredItem (int itemResultID, String itemText, Colour itemTextColour,
                                 bool isActive, bool isTicked, const Image& iconToUse)
{
    Item i (std::move (itemText));
    i.itemID = itemResultID;
    i.colour = itemTextColour;
    i.isEnabled = isActive;
    i.isTicked = isTicked;
    i.image = createDrawableFromImage (iconToUse);
    addItem (std::move (i));
}

void PopupMenu::addCustomItem (int itemResultID,
                               std::unique_ptr<CustomComponent> cc,
                               std::unique_ptr<const PopupMenu> subMenu)
{
    Item i;
    i.itemID = itemResultID;
    i.customComponent = cc.release();
    i.subMenu.reset (createCopyIfNotNull (subMenu.get()));
    addItem (std::move (i));
}

void PopupMenu::addCustomItem (int itemResultID,
                               Component& customComponent,
                               int idealWidth, int idealHeight,
                               bool triggerMenuItemAutomaticallyWhenClicked,
                               std::unique_ptr<const PopupMenu> subMenu)
{
    auto comp = std::make_unique<HelperClasses::NormalComponentWrapper> (customComponent, idealWidth, idealHeight,
                                                                         triggerMenuItemAutomaticallyWhenClicked);
    addCustomItem (itemResultID, std::move (comp), std::move (subMenu));
}

void PopupMenu::addSubMenu (String subMenuName, PopupMenu subMenu, bool isActive)
{
    addSubMenu (std::move (subMenuName), std::move (subMenu), isActive, nullptr, false, 0);
}

void PopupMenu::addSubMenu (String subMenuName, PopupMenu subMenu, bool isActive,
                            const Image& iconToUse, bool isTicked, int itemResultID)
{
    addSubMenu (std::move (subMenuName), std::move (subMenu), isActive,
                createDrawableFromImage (iconToUse), isTicked, itemResultID);
}

void PopupMenu::addSubMenu (String subMenuName, PopupMenu subMenu, bool isActive,
                            std::unique_ptr<Drawable> iconToUse, bool isTicked, int itemResultID)
{
    Item i (std::move (subMenuName));
    i.itemID = itemResultID;
    i.isEnabled = isActive && (itemResultID != 0 || subMenu.getNumItems() > 0);
    i.subMenu.reset (new PopupMenu (std::move (subMenu)));
    i.isTicked = isTicked;
    i.image = std::move (iconToUse);
    addItem (std::move (i));
}

void PopupMenu::addSeparator()
{
    if (items.size() > 0 && ! items.back().isSeparator)
    {
        Item i;
        i.isSeparator = true;
        addItem (std::move (i));
    }
}

void PopupMenu::addSectionHeader (String title)
{
    Item i (std::move (title));
    i.itemID = 0;
    i.isSectionHeader = true;
    addItem (std::move (i));
}

//==============================================================================
PopupMenu::Options::Options()
{
    targetArea.setPosition (Desktop::getMousePosition());
}

PopupMenu::Options PopupMenu::Options::withTargetComponent (Component* comp) const
{
    Options o (*this);
    o.targetComponent = comp;

    if (comp != nullptr)
        o.targetArea = comp->getScreenBounds();

    return o;
}

PopupMenu::Options PopupMenu::Options::withTargetComponent (Component& comp) const
{
    return withTargetComponent (&comp);
}

PopupMenu::Options PopupMenu::Options::withTargetScreenArea (Rectangle<int> area) const
{
    Options o (*this);
    o.targetArea = area;
    return o;
}

PopupMenu::Options PopupMenu::Options::withDeletionCheck (Component& comp) const
{
    Options o (*this);
    o.componentToWatchForDeletion = &comp;
    o.isWatchingForDeletion = true;
    return o;
}

PopupMenu::Options PopupMenu::Options::withMinimumWidth (int w) const
{
    Options o (*this);
    o.minWidth = w;
    return o;
}

PopupMenu::Options PopupMenu::Options::withMinimumNumColumns (int cols) const
{
    Options o (*this);
    o.minColumns = cols;
    return o;
}

PopupMenu::Options PopupMenu::Options::withMaximumNumColumns (int cols) const
{
    Options o (*this);
    o.maxColumns = cols;
    return o;
}

PopupMenu::Options PopupMenu::Options::withStandardItemHeight (int height) const
{
    Options o (*this);
    o.standardHeight = height;
    return o;
}

PopupMenu::Options PopupMenu::Options::withItemThatMustBeVisible (int idOfItemToBeVisible) const
{
    Options o (*this);
    o.visibleItemID = idOfItemToBeVisible;
    return o;
}

PopupMenu::Options PopupMenu::Options::withParentComponent (Component* parent) const
{
    Options o (*this);
    o.parentComponent = parent;
    return o;
}

PopupMenu::Options PopupMenu::Options::withPreferredPopupDirection (PopupDirection direction) const
{
    Options o (*this);
    o.preferredPopupDirection = direction;
    return o;
}

Component* PopupMenu::createWindow (const Options& options,
                                    ApplicationCommandManager** managerOfChosenCommand) const
{
    return items.empty() ? nullptr
                         : new HelperClasses::MenuWindow (*this, nullptr, options,
                                                          ! options.getTargetScreenArea().isEmpty(),
                                                          ModifierKeys::currentModifiers.isAnyMouseButtonDown(),
                                                          managerOfChosenCommand);
}

//==============================================================================
// This invokes any command manager commands and deletes the menu window when it is dismissed
struct PopupMenuCompletionCallback  : public ModalComponentManager::Callback
{
    PopupMenuCompletionCallback()
        : prevFocused (Component::getCurrentlyFocusedComponent()),
          prevTopLevel (prevFocused != nullptr ? prevFocused->getTopLevelComponent() : nullptr)
    {
        PopupMenuSettings::menuWasHiddenBecauseOfAppChange = false;
    }

    void modalStateFinished (int result) override
    {
        if (managerOfChosenCommand != nullptr && result != 0)
        {
            ApplicationCommandTarget::InvocationInfo info (result);
            info.invocationMethod = ApplicationCommandTarget::InvocationInfo::fromMenu;

            managerOfChosenCommand->invoke (info, true);
        }

        // (this would be the place to fade out the component, if that's what's required)
        component.reset();

        if (! PopupMenuSettings::menuWasHiddenBecauseOfAppChange)
        {
            if (prevTopLevel != nullptr)
                prevTopLevel->toFront (true);

            if (prevFocused != nullptr && prevFocused->isShowing())
                prevFocused->grabKeyboardFocus();
        }
    }

    ApplicationCommandManager* managerOfChosenCommand = nullptr;
    std::unique_ptr<Component> component;
    WeakReference<Component> prevFocused, prevTopLevel;

    JUCE_DECLARE_NON_COPYABLE (PopupMenuCompletionCallback)
};

int PopupMenu::showWithOptionalCallback (const Options& options,
                                         ModalComponentManager::Callback* userCallback,
                                         bool canBeModal)
{
    std::unique_ptr<ModalComponentManager::Callback> userCallbackDeleter (userCallback);
    std::unique_ptr<PopupMenuCompletionCallback> callback (new PopupMenuCompletionCallback());

    if (auto* window = createWindow (options, &(callback->managerOfChosenCommand)))
    {
        callback->component.reset (window);

        window->setVisible (true); // (must be called before enterModalState on Windows to avoid DropShadower confusion)
        window->enterModalState (false, userCallbackDeleter.release());
        ModalComponentManager::getInstance()->attachCallback (window, callback.release());

        window->toFront (false);  // need to do this after making it modal, or it could
                                  // be stuck behind other comps that are already modal..

       #if JUCE_MODAL_LOOPS_PERMITTED
        if (userCallback == nullptr && canBeModal)
            return window->runModalLoop();
       #else
        ignoreUnused (canBeModal);
        jassert (! (userCallback == nullptr && canBeModal));
       #endif
    }

    return 0;
}

//==============================================================================
#if JUCE_MODAL_LOOPS_PERMITTED
int PopupMenu::showMenu (const Options& options)
{
    return showWithOptionalCallback (options, nullptr, true);
}
#endif

void PopupMenu::showMenuAsync (const Options& options)
{
    showWithOptionalCallback (options, nullptr, false);
}

void PopupMenu::showMenuAsync (const Options& options, ModalComponentManager::Callback* userCallback)
{
   #if ! JUCE_MODAL_LOOPS_PERMITTED
    jassert (userCallback != nullptr);
   #endif

    showWithOptionalCallback (options, userCallback, false);
}

void PopupMenu::showMenuAsync (const Options& options, std::function<void(int)> userCallback)
{
    showWithOptionalCallback (options, ModalCallbackFunction::create (userCallback), false);
}

//==============================================================================
#if JUCE_MODAL_LOOPS_PERMITTED
int PopupMenu::show (int itemIDThatMustBeVisible, int minimumWidth,
                     int maximumNumColumns, int standardItemHeight,
                     ModalComponentManager::Callback* callback)
{
    return showWithOptionalCallback (Options().withItemThatMustBeVisible (itemIDThatMustBeVisible)
                                              .withMinimumWidth (minimumWidth)
                                              .withMaximumNumColumns (maximumNumColumns)
                                              .withStandardItemHeight (standardItemHeight),
                                     callback, true);
}

int PopupMenu::showAt (Rectangle<int> screenAreaToAttachTo,
                       int itemIDThatMustBeVisible, int minimumWidth,
                       int maximumNumColumns, int standardItemHeight,
                       ModalComponentManager::Callback* callback)
{
    return showWithOptionalCallback (Options().withTargetScreenArea (screenAreaToAttachTo)
                                              .withItemThatMustBeVisible (itemIDThatMustBeVisible)
                                              .withMinimumWidth (minimumWidth)
                                              .withMaximumNumColumns (maximumNumColumns)
                                              .withStandardItemHeight (standardItemHeight),
                                     callback, true);
}

int PopupMenu::showAt (Component* componentToAttachTo,
                       int itemIDThatMustBeVisible, int minimumWidth,
                       int maximumNumColumns, int standardItemHeight,
                       ModalComponentManager::Callback* callback)
{
    auto options = Options().withItemThatMustBeVisible (itemIDThatMustBeVisible)
                            .withMinimumWidth (minimumWidth)
                            .withMaximumNumColumns (maximumNumColumns)
                            .withStandardItemHeight (standardItemHeight);

    if (componentToAttachTo != nullptr)
        options = options.withTargetComponent (componentToAttachTo);

    return showWithOptionalCallback (options, callback, true);
}
#endif

bool JUCE_CALLTYPE PopupMenu::dismissAllActiveMenus()
{
    auto& windows = HelperClasses::MenuWindow::getActiveWindows();
    auto numWindows = windows.size();

    for (int i = numWindows; --i >= 0;)
    {
        if (auto* pmw = windows[i])
        {
            pmw->setLookAndFeel (nullptr);
            pmw->dismissMenu (nullptr);
        }
    }

    return numWindows > 0;
}

//==============================================================================
int PopupMenu::getNumItems() const noexcept
{
    int num = 0;

    for (auto& mi : items)
        if (! mi.isSeparator)
            ++num;

    return num;
}

bool PopupMenu::containsCommandItem (const int commandID) const
{
    for (auto& mi : items)
        if ((mi.itemID == commandID && mi.commandManager != nullptr)
              || (mi.subMenu != nullptr && mi.subMenu->containsCommandItem (commandID)))
            return true;

    return false;
}

bool PopupMenu::containsAnyActiveItems() const noexcept
{
    for (auto& mi : items)
    {
        if (mi.subMenu != nullptr)
        {
            if (mi.subMenu->containsAnyActiveItems())
                return true;
        }
        else if (mi.isEnabled)
        {
            return true;
        }
    }

    return false;
}

void PopupMenu::setLookAndFeel (LookAndFeel* const newLookAndFeel)
{
    lookAndFeel = newLookAndFeel;
}

void PopupMenu::setItem (CustomComponent& c, const Item* itemToUse)
{
    c.item = itemToUse;
    c.repaint();
}

//==============================================================================
PopupMenu::CustomComponent::CustomComponent (bool autoTrigger)
    : triggeredAutomatically (autoTrigger)
{
}

PopupMenu::CustomComponent::~CustomComponent()
{
}

void PopupMenu::CustomComponent::setHighlighted (bool shouldBeHighlighted)
{
    isHighlighted = shouldBeHighlighted;
    repaint();
}

void PopupMenu::CustomComponent::triggerMenuItem()
{
    if (auto* mic = findParentComponentOfClass<HelperClasses::ItemComponent>())
    {
        if (auto* pmw = mic->findParentComponentOfClass<HelperClasses::MenuWindow>())
        {
            pmw->dismissMenu (&mic->item);
        }
        else
        {
            // something must have gone wrong with the component hierarchy if this happens..
            jassertfalse;
        }
    }
    else
    {
        // why isn't this component inside a menu? Not much point triggering the item if
        // there's no menu.
        jassertfalse;
    }
}

//==============================================================================
PopupMenu::CustomCallback::CustomCallback() {}
PopupMenu::CustomCallback::~CustomCallback() {}

//==============================================================================
PopupMenu::MenuItemIterator::MenuItemIterator (const PopupMenu& m, bool recurse) : searchRecursively (recurse)
{
    index.add (0);
    menus.add (&m);
}

PopupMenu::MenuItemIterator::~MenuItemIterator() = default;

bool PopupMenu::MenuItemIterator::next()
{
    if (index.size() == 0 || menus.getLast()->items.size() == 0)
        return false;

    currentItem = const_cast<PopupMenu::Item*> (&(menus.getLast()->items[(size_t) index.getLast()]));

    if (searchRecursively && currentItem->subMenu != nullptr)
    {
        index.add (0);
        menus.add (currentItem->subMenu.get());
    }
    else
    {
        index.setUnchecked (index.size() - 1, index.getLast() + 1);
    }

    while (index.size() > 0 && index.getLast() >= (int) menus.getLast()->items.size())
    {
        index.removeLast();
        menus.removeLast();

        if (index.size() > 0)
            index.setUnchecked (index.size() - 1, index.getLast() + 1);
    }

    return true;
}

PopupMenu::Item& PopupMenu::MenuItemIterator::getItem() const
{
    jassert (currentItem != nullptr);
    return *(currentItem);
}

} // namespace juce
