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

#if JUCE_USE_DIRECTWRITE
namespace DirectWriteTypeLayout
{
    class CustomDirectWriteTextRenderer   : public ComBaseClassHelper<IDWriteTextRenderer>
    {
    public:
        CustomDirectWriteTextRenderer (IDWriteFontCollection& fonts, const AttributedString& as)
            : ComBaseClassHelper<IDWriteTextRenderer> (0),
              attributedString (as),
              fontCollection (fonts)
        {
        }

        JUCE_COMRESULT QueryInterface (REFIID refId, void** result) override
        {
            if (refId == __uuidof (IDWritePixelSnapping))
                return castToType<IDWritePixelSnapping> (result);

            return ComBaseClassHelper<IDWriteTextRenderer>::QueryInterface (refId, result);
        }

        JUCE_COMRESULT IsPixelSnappingDisabled (void* /*clientDrawingContext*/, BOOL* isDisabled) noexcept override
        {
            *isDisabled = FALSE;
            return S_OK;
        }

        JUCE_COMRESULT GetCurrentTransform (void*, DWRITE_MATRIX* matrix) noexcept override
        {
            matrix->m11 = 1.0f; matrix->m12 = 0.0f;
            matrix->m21 = 0.0f; matrix->m22 = 1.0f;
            matrix->dx  = 0.0f; matrix->dy  = 0.0f;
            return S_OK;
        }

        JUCE_COMRESULT GetPixelsPerDip (void*, FLOAT* pixelsPerDip) noexcept override
        {
            *pixelsPerDip = 1.0f;
            return S_OK;
        }

        JUCE_COMRESULT DrawUnderline (void*, FLOAT, FLOAT, DWRITE_UNDERLINE const*, IUnknown*) noexcept override
        {
            return E_NOTIMPL;
        }

        JUCE_COMRESULT DrawStrikethrough (void*, FLOAT, FLOAT, DWRITE_STRIKETHROUGH const*, IUnknown*) noexcept override
        {
            return E_NOTIMPL;
        }

        JUCE_COMRESULT DrawInlineObject (void*, FLOAT, FLOAT, IDWriteInlineObject*, BOOL, BOOL, IUnknown*) noexcept override
        {
            return E_NOTIMPL;
        }

        JUCE_COMRESULT DrawGlyphRun (void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE,
                                     DWRITE_GLYPH_RUN const* glyphRun, DWRITE_GLYPH_RUN_DESCRIPTION const* runDescription,
                                     IUnknown* clientDrawingEffect) noexcept override
        {
            auto layout = static_cast<TextLayout*> (clientDrawingContext);

            if (! (baselineOriginY >= -1.0e10f && baselineOriginY <= 1.0e10f))
                baselineOriginY = 0; // DirectWrite sometimes sends NaNs in this parameter

            if (baselineOriginY != lastOriginY)
            {
                lastOriginY = baselineOriginY;
                ++currentLine;

                if (currentLine >= layout->getNumLines())
                {
                    jassert (currentLine == layout->getNumLines());
                    auto line = std::make_unique<TextLayout::Line>();
                    line->lineOrigin = Point<float> (baselineOriginX, baselineOriginY);

                    layout->addLine (std::move (line));
                }
            }

            auto& glyphLine = layout->getLine (currentLine);

            DWRITE_FONT_METRICS dwFontMetrics;
            glyphRun->fontFace->GetMetrics (&dwFontMetrics);

            glyphLine.ascent  = jmax (glyphLine.ascent,  scaledFontSize (dwFontMetrics.ascent,  dwFontMetrics, *glyphRun));
            glyphLine.descent = jmax (glyphLine.descent, scaledFontSize (dwFontMetrics.descent, dwFontMetrics, *glyphRun));

            auto glyphRunLayout = new TextLayout::Run (Range<int> (runDescription->textPosition,
                                                                   runDescription->textPosition + runDescription->stringLength),
                                                       glyphRun->glyphCount);
            glyphLine.runs.add (glyphRunLayout);

            glyphRun->fontFace->GetMetrics (&dwFontMetrics);
            auto totalHeight = std::abs ((float) dwFontMetrics.ascent) + std::abs ((float) dwFontMetrics.descent);
            auto fontHeightToEmSizeFactor = (float) dwFontMetrics.designUnitsPerEm / totalHeight;

            glyphRunLayout->font = getFontForRun (*glyphRun, glyphRun->fontEmSize / fontHeightToEmSizeFactor);
            glyphRunLayout->colour = getColourOf (static_cast<ID2D1SolidColorBrush*> (clientDrawingEffect));

            auto lineOrigin = layout->getLine (currentLine).lineOrigin;
            auto x = baselineOriginX - lineOrigin.x;

            auto extraKerning = glyphRunLayout->font.getExtraKerningFactor()
                                   * glyphRunLayout->font.getHeight();

            for (UINT32 i = 0; i < glyphRun->glyphCount; ++i)
            {
                auto advance = glyphRun->glyphAdvances[i];

                if ((glyphRun->bidiLevel & 1) != 0)
                    x -= advance + extraKerning;  // RTL text

                glyphRunLayout->glyphs.add (TextLayout::Glyph (glyphRun->glyphIndices[i],
                                                               Point<float> (x, baselineOriginY - lineOrigin.y),
                                                               advance));

                if ((glyphRun->bidiLevel & 1) == 0)
                    x += advance + extraKerning;  // LTR text
            }

            return S_OK;
        }

    private:
        const AttributedString& attributedString;
        IDWriteFontCollection& fontCollection;
        int currentLine = -1;
        float lastOriginY = -10000.0f;

        static float scaledFontSize (int n, const DWRITE_FONT_METRICS& metrics, const DWRITE_GLYPH_RUN& glyphRun) noexcept
        {
            return (std::abs ((float) n) / (float) metrics.designUnitsPerEm) * glyphRun.fontEmSize;
        }

        static Colour getColourOf (ID2D1SolidColorBrush* d2dBrush) noexcept
        {
            if (d2dBrush == nullptr)
                return Colours::black;

            const D2D1_COLOR_F colour (d2dBrush->GetColor());
            return Colour::fromFloatRGBA (colour.r, colour.g, colour.b, colour.a);
        }

        Font getFontForRun (const DWRITE_GLYPH_RUN& glyphRun, float fontHeight)
        {
            for (int i = 0; i < attributedString.getNumAttributes(); ++i)
            {
                auto& font = attributedString.getAttribute(i).font;

                if (auto* wt = dynamic_cast<WindowsDirectWriteTypeface*> (font.getTypeface()))
                    if (wt->getIDWriteFontFace() == glyphRun.fontFace)
                        return font.withHeight (fontHeight);
            }

            ComSmartPtr<IDWriteFont> dwFont;
            auto hr = fontCollection.GetFontFromFontFace (glyphRun.fontFace, dwFont.resetAndGetPointerAddress());
            jassert (dwFont != nullptr);

            ComSmartPtr<IDWriteFontFamily> dwFontFamily;
            hr = dwFont->GetFontFamily (dwFontFamily.resetAndGetPointerAddress());

            return Font (getFontFamilyName (dwFontFamily), getFontFaceName (dwFont), fontHeight);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomDirectWriteTextRenderer)
    };

    //==============================================================================
    static float getFontHeightToEmSizeFactor (IDWriteFont& dwFont)
    {
        ComSmartPtr<IDWriteFontFace> dwFontFace;
        dwFont.CreateFontFace (dwFontFace.resetAndGetPointerAddress());

        if (dwFontFace == nullptr)
            return 1.0f;

        DWRITE_FONT_METRICS dwFontMetrics;
        dwFontFace->GetMetrics (&dwFontMetrics);

        const float totalHeight = (float) (dwFontMetrics.ascent + dwFontMetrics.descent);
        return dwFontMetrics.designUnitsPerEm / totalHeight;
    }

    void setTextFormatProperties (const AttributedString& text, IDWriteTextFormat& format)
    {
        DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
        DWRITE_WORD_WRAPPING wrapType = DWRITE_WORD_WRAPPING_WRAP;

        switch (text.getJustification().getOnlyHorizontalFlags())
        {
            case 0:
            case Justification::left:                   break;
            case Justification::right:                  alignment = DWRITE_TEXT_ALIGNMENT_TRAILING; break;
            case Justification::horizontallyCentred:    alignment = DWRITE_TEXT_ALIGNMENT_CENTER; break;
            case Justification::horizontallyJustified:  break; // DirectWrite cannot justify text, default to left alignment
            default:                                    jassertfalse; break; // Illegal justification flags
        }

        switch (text.getWordWrap())
        {
            case AttributedString::none:      wrapType = DWRITE_WORD_WRAPPING_NO_WRAP; break;
            case AttributedString::byWord:    break;
            case AttributedString::byChar:    break; // DirectWrite doesn't support wrapping by character, default to word-wrap
            default:                          jassertfalse; break; // Illegal flags!
        }

        // DirectWrite does not automatically set reading direction
        // This must be set correctly and manually when using RTL Scripts (Hebrew, Arabic)
        if (text.getReadingDirection() == AttributedString::rightToLeft)
        {
            format.SetReadingDirection (DWRITE_READING_DIRECTION_RIGHT_TO_LEFT);

            switch (text.getJustification().getOnlyHorizontalFlags())
            {
                case 0:
                case Justification::left:      alignment = DWRITE_TEXT_ALIGNMENT_TRAILING; break;
                case Justification::right:     alignment = DWRITE_TEXT_ALIGNMENT_LEADING;  break;
                default: break;
            }
        }

        format.SetTextAlignment (alignment);
        format.SetWordWrapping (wrapType);
    }

    void addAttributedRange (const AttributedString::Attribute& attr, IDWriteTextLayout& textLayout,
                             const int textLen, ID2D1RenderTarget& renderTarget, IDWriteFontCollection& fontCollection)
    {
        DWRITE_TEXT_RANGE range;
        range.startPosition = attr.range.getStart();
        range.length = jmin (attr.range.getLength(), textLen - attr.range.getStart());

        {
            auto familyName = FontStyleHelpers::getConcreteFamilyName (attr.font);

            BOOL fontFound = false;
            uint32 fontIndex;
            fontCollection.FindFamilyName (familyName.toWideCharPointer(), &fontIndex, &fontFound);

            if (! fontFound)
                fontIndex = 0;

            ComSmartPtr<IDWriteFontFamily> fontFamily;
            auto hr = fontCollection.GetFontFamily (fontIndex, fontFamily.resetAndGetPointerAddress());

            ComSmartPtr<IDWriteFont> dwFont;
            uint32 fontFacesCount = 0;
            fontFacesCount = fontFamily->GetFontCount();

            for (int i = fontFacesCount; --i >= 0;)
            {
                hr = fontFamily->GetFont (i, dwFont.resetAndGetPointerAddress());

                if (attr.font.getTypefaceStyle() == getFontFaceName (dwFont))
                    break;
            }

            textLayout.SetFontFamilyName (familyName.toWideCharPointer(), range);
            textLayout.SetFontWeight (dwFont->GetWeight(), range);
            textLayout.SetFontStretch (dwFont->GetStretch(), range);
            textLayout.SetFontStyle (dwFont->GetStyle(), range);

            auto fontHeightToEmSizeFactor = getFontHeightToEmSizeFactor (*dwFont);
            textLayout.SetFontSize (attr.font.getHeight() * fontHeightToEmSizeFactor, range);
        }

        {
            auto col = attr.colour;
            ComSmartPtr<ID2D1SolidColorBrush> d2dBrush;
            renderTarget.CreateSolidColorBrush (D2D1::ColorF (col.getFloatRed(),
                                                              col.getFloatGreen(),
                                                              col.getFloatBlue(),
                                                              col.getFloatAlpha()),
                                                d2dBrush.resetAndGetPointerAddress());

            // We need to call SetDrawingEffect with a legitimate brush to get DirectWrite to break text based on colours
            textLayout.SetDrawingEffect (d2dBrush, range);
        }
    }

    bool setupLayout (const AttributedString& text, float maxWidth, float maxHeight,
                      ID2D1RenderTarget& renderTarget, IDWriteFactory& directWriteFactory,
                      IDWriteFontCollection& fontCollection, ComSmartPtr<IDWriteTextLayout>& textLayout)
    {
        // To add color to text, we need to create a D2D render target
        // Since we are not actually rendering to a D2D context we create a temporary GDI render target

        Font defaultFont;
        BOOL fontFound = false;
        uint32 fontIndex;
        fontCollection.FindFamilyName (defaultFont.getTypeface()->getName().toWideCharPointer(), &fontIndex, &fontFound);

        if (! fontFound)
            fontIndex = 0;

        ComSmartPtr<IDWriteFontFamily> dwFontFamily;
        auto hr = fontCollection.GetFontFamily (fontIndex, dwFontFamily.resetAndGetPointerAddress());

        ComSmartPtr<IDWriteFont> dwFont;
        hr = dwFontFamily->GetFirstMatchingFont (DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STRETCH_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                                 dwFont.resetAndGetPointerAddress());
        jassert (dwFont != nullptr);

        auto defaultFontHeightToEmSizeFactor = getFontHeightToEmSizeFactor (*dwFont);

        ComSmartPtr<IDWriteTextFormat> dwTextFormat;
        hr = directWriteFactory.CreateTextFormat (defaultFont.getTypefaceName().toWideCharPointer(), &fontCollection,
                                                  DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
                                                  defaultFont.getHeight() * defaultFontHeightToEmSizeFactor,
                                                  L"en-us", dwTextFormat.resetAndGetPointerAddress());

        setTextFormatProperties (text, *dwTextFormat);

        {
            DWRITE_TRIMMING trimming = { DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0 };
            ComSmartPtr<IDWriteInlineObject> trimmingSign;
            hr = directWriteFactory.CreateEllipsisTrimmingSign (dwTextFormat, trimmingSign.resetAndGetPointerAddress());
            hr = dwTextFormat->SetTrimming (&trimming, trimmingSign);
        }

        auto textLen = text.getText().length();

        hr = directWriteFactory.CreateTextLayout (text.getText().toWideCharPointer(), textLen, dwTextFormat,
                                                  maxWidth, maxHeight, textLayout.resetAndGetPointerAddress());

        if (FAILED (hr) || textLayout == nullptr)
            return false;

        auto numAttributes = text.getNumAttributes();

        for (int i = 0; i < numAttributes; ++i)
            addAttributedRange (text.getAttribute (i), *textLayout, textLen, renderTarget, fontCollection);

        return true;
    }

    void createLayout (TextLayout& layout, const AttributedString& text,
                       IDWriteFactory& directWriteFactory,
                       IDWriteFontCollection& fontCollection,
                       ID2D1DCRenderTarget& renderTarget)
    {
        ComSmartPtr<IDWriteTextLayout> dwTextLayout;

        if (! setupLayout (text, layout.getWidth(), layout.getHeight(), renderTarget,
                           directWriteFactory, fontCollection, dwTextLayout))
            return;

        UINT32 actualLineCount = 0;
        auto hr = dwTextLayout->GetLineMetrics (nullptr, 0, &actualLineCount);

        layout.ensureStorageAllocated (actualLineCount);

        {
            ComSmartPtr<CustomDirectWriteTextRenderer> textRenderer (new CustomDirectWriteTextRenderer (fontCollection, text));
            hr = dwTextLayout->Draw (&layout, textRenderer, 0, 0);
        }

        HeapBlock<DWRITE_LINE_METRICS> dwLineMetrics (actualLineCount);
        hr = dwTextLayout->GetLineMetrics (dwLineMetrics, actualLineCount, &actualLineCount);
        int lastLocation = 0;
        auto numLines = jmin ((int) actualLineCount, layout.getNumLines());
        float yAdjustment = 0;
        auto extraLineSpacing = text.getLineSpacing();

        for (int i = 0; i < numLines; ++i)
        {
            auto& line = layout.getLine (i);
            line.stringRange = Range<int> (lastLocation, (int) lastLocation + dwLineMetrics[i].length);
            line.lineOrigin.y += yAdjustment;
            yAdjustment += extraLineSpacing;
            lastLocation += dwLineMetrics[i].length;
        }
    }

    void drawToD2DContext (const AttributedString& text, const Rectangle<float>& area, ID2D1RenderTarget& renderTarget,
                           IDWriteFactory& directWriteFactory, IDWriteFontCollection& fontCollection)
    {
        ComSmartPtr<IDWriteTextLayout> dwTextLayout;

        if (setupLayout (text, area.getWidth(), area.getHeight(), renderTarget,
                         directWriteFactory, fontCollection, dwTextLayout))
        {
            ComSmartPtr<ID2D1SolidColorBrush> d2dBrush;
            renderTarget.CreateSolidColorBrush (D2D1::ColorF (0.0f, 0.0f, 0.0f, 1.0f),
                                                d2dBrush.resetAndGetPointerAddress());

            renderTarget.DrawTextLayout (D2D1::Point2F ((float) area.getX(), (float) area.getY()),
                                         dwTextLayout, d2dBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
        }
    }
}

static bool canAllTypefacesBeUsedInLayout (const AttributedString& text)
{
    auto numCharacterAttributes = text.getNumAttributes();

    for (int i = 0; i < numCharacterAttributes; ++i)
        if (dynamic_cast<WindowsDirectWriteTypeface*> (text.getAttribute(i).font.getTypeface()) == nullptr)
            return false;

    return true;
}

#endif

bool TextLayout::createNativeLayout (const AttributedString& text)
{
   #if JUCE_USE_DIRECTWRITE
    if (! canAllTypefacesBeUsedInLayout (text))
        return false;

    SharedResourcePointer<Direct2DFactories> factories;

    if (factories->d2dFactory != nullptr && factories->systemFonts != nullptr)
    {
        DirectWriteTypeLayout::createLayout (*this, text,
                                             *factories->directWriteFactory,
                                             *factories->systemFonts,
                                             *factories->directWriteRenderTarget);

        return true;
    }
   #else
    ignoreUnused (text);
   #endif

    return false;
}

} // namespace juce
