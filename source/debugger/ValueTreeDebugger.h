/*
 * (c) Credland Technical Limited.
 * MIT License
 *
 * JCF_DEBUG - Debugging helpers for JUCE.  Demo application.
 *
 * Don't forget to install the VisualStudio or Xcode debug scripts as
 * well.  These ensure that your IDEs debugger displays the contents
 * of ValueTrees, Strings and Arrays in a useful way!
 *
 *
 * Credland Technical Limited provide a range of consultancy and contract
 * services including:
 * - JUCE software development and support
 * - information security consultancy
 *
 * Contact via http://www.credland.net/
 */

#ifndef VALUE_TREE_EDITOR_H_INCLUDED
#define VALUE_TREE_EDITOR_H_INCLUDED

#include <juce_audio_processors/juce_audio_processors.h>

class ValueTreeDebuggerMain;

class ValueTreeDebugger :
    public juce::DocumentWindow
{
public:
    /**
     @class Display a separate desktop window for viewed and editing a value
     tree's property fields.

     Instantate a ValueTreeDebugger instance, then call
     ValueTreeDebugger::setSource(ValueTree &) and it'll display your
     tree.

     For example:

     @code
     vd = new ValueTreeDebugger();
     vd->setSource(myTree);
     @endcode

     @note This code isn't pretty - it's for debugging, not production use!
     */
    ValueTreeDebugger();

    ValueTreeDebugger (juce::ValueTree& treeToShow);

    ~ValueTreeDebugger();

    void closeButtonPressed() override;

    /**
     * Call setSource(ValueTree&) to show a particular ValueTree in
     * the editor. If you attach all the ValueTree's in your program
     * to a common root, you'll be able to view the whole thing in
     * one editor.
     */
    void setSource (juce::ValueTree& treeToShow);

private:
    void construct();
    std::unique_ptr<ValueTreeDebuggerMain> main;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeDebugger);
};


#endif  // VALUE_TREE_EDITOR_H_INCLUDED
