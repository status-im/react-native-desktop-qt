import QtQuick 2.4
import React 0.1 as React
import QtQuick.Controls 2.2

TextEdit {
    id: textRoot

    property bool p_allowFontScaling: false
    property string p_fontFamily
    property double p_fontSize: font.pointSize
    property bool p_highlighted: false
    property color p_color: "black"
    property color p_backgroundColor: 'transparent'
    property string p_fontStyle
    property string p_fontWeight
    property double p_letterSpacing
    property double p_lineHeight
    property string p_textAlign: "left"
    property string p_textDecorationLine
    property string p_textDecorationStyle
    property color p_textDecorationColor
    property string p_writingDirection
    property int p_numberOfLines: 1000000
    property var flexbox: null
    property double p_opacity: 1
    property bool p_selectable: false
    property color p_selectionColor: "darkblue"
    readonly property bool isText: true


    property string typeName: "ReactText"
    property var textManager: null
    property string decoratedText
    //ReactText components can be nested. This property indicates if item is parent
    //of current text blocks.
    property bool textIsTopInBlock: parent ? (parent.isText ? false : true) : true
    onTextIsTopInBlockChanged: {
        manageFlexbox()
        updateMeasureFunction()
    }
    onTextManagerChanged: updateMeasureFunction()

    text: textIsTopInBlock ? decoratedText : ""
    horizontalAlignment: horizontalAlignmentFromTextAlign(p_textAlign)
    textFormat: Text.RichText
    wrapMode: Text.Wrap
    clip: true
    opacity: p_opacity
    readOnly: true
    selectByMouse: p_selectable
    selectionColor: p_selectionColor
    selectedTextColor: p_color
    onActiveFocusChanged: {
        if(!activeFocus && !persistentSelection) {
            select(0,0)
        }
    }


    onP_allowFontScalingChanged: updateHtmlText();
    onP_fontFamilyChanged: updateHtmlText();
    onP_fontSizeChanged: updateHtmlText();
    onP_highlightedChanged: updateHtmlText();
    onP_colorChanged: updateHtmlText();
    onP_fontStyleChanged: updateHtmlText();
    onP_fontWeightChanged: updateHtmlText();
    onP_letterSpacingChanged: updateHtmlText();
    onP_lineHeightChanged: updateHtmlText();
    onP_textAlignChanged: updateHtmlText();
    onP_textDecorationLineChanged: updateHtmlText();
    onP_textDecorationStyleChanged: updateHtmlText();
    onP_textDecorationColorChanged: updateHtmlText();
    onP_writingDirectionChanged: updateHtmlText();
    onP_numberOfLinesChanged: updateHtmlText();


    onChildrenChanged: {
        subscribeToChildrenTextChanges()
        updateHtmlText()
    }
    onParentChanged: updateHtmlText()

    function manageFlexbox() {
        if(textManager) {
            textManager.manageFlexbox(textRoot, textIsTopInBlock)
        }
    }

    function updateMeasureFunction() {
        if(textManager) {
            textManager.updateMeasureFunction(textRoot)
        }
    }

    function subscribeToChildrenTextChanges() {
        for (var i = 0; i < textRoot.children.length; i++)
        {
            var child = textRoot.children[i];
            if(child.isText) {
                child.decoratedTextChanged.connect(updateHtmlText)
            }
            else if(child.isRawText) {
                child.p_textChanged.connect(updateHtmlText)
            }
        }
    }

    function horizontalAlignmentFromTextAlign(textAlign) {
        switch (textAlign) {
        case "left":    return Text.AlignLeft;
        case "right":   return Text.AlignRight;
        case "center":  return Text.AlignHCenter;
        case "justify": return Text.AlignJustify;
        default:        return Text.AlignLeft;
        }
    }

    function updateHtmlText() {
        var htmlString = "";

        for (var i = 0; i < textRoot.children.length; i++)
        {
            var child = textRoot.children[i];
            if(child.isText && child.decoratedText) {
                htmlString += child.decoratedText
            }
            else if(child.isRawText && child.p_text) {
                htmlString += textManager.textToHtml(textRoot, child.p_text)
            }
        }
        decoratedText = htmlString;
        if (textRoot.flexbox) {
            textRoot.flexbox.markDirty();
        }
    }
}
