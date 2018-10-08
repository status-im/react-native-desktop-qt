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


    property string typeName: "ReactText"
    property var textManager
    property string decoratedText
    //ReactText components can be nested. This property indicates if item is parent
    //of current text blocks.
    property bool textIsTopInBlock: parent ? (parent.typeName ? (parent.typeName === "ReactText" ? false : true) : true) : true
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
    onParentChanged: {
        if(parent){
            updateHtmlText()
        }
    }

    function manageFlexbox() {
        //Only topmost text item in a set of nested ones can have a flexbox node.
        if(textIsTopInBlock) {
            textRoot.flexbox = Qt.createQmlObject('import React 0.1 as React; React.Flexbox {control: textRoot; viewManager: (textManager ? textManager : null)}',
                                               textRoot, "dynamicSnippet1");
        }
        else
        {
            if(textRoot.flexbox) {
                textRoot.flexbox.destroy()
                textRoot.flexbox = null;
            }
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
            if(isText(child)) {
                child.decoratedTextChanged.connect(updateHtmlText)
            }
            else if(isRawText(child)) {
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

    function textToHtml(textString) {

        //get props values, own or nested
        var fontFamily = textManager.nestedPropertyValue(textRoot, "p_fontFamily")
        var color = textManager.nestedPropertyValue(textRoot, "p_color")
        var backgroundColor = textManager.nestedPropertyValue(textRoot, "p_backgroundColor")
        var fontWeight = textManager.nestedPropertyValue(textRoot, "p_fontWeight")
        var fontSize = textManager.nestedPropertyValue(textRoot, "p_fontSize")
        var fontStyle = textManager.nestedPropertyValue(textRoot, "p_fontStyle")
        var textDecorLine = textManager.nestedPropertyValue(textRoot, "p_textDecorationLine")

        var result = "<span style=\"white-space: pre; " + (fontFamily ? ("font-family:"+fontFamily+";") : "")
                + (fontSize? ("font-size:"+fontSize+"pt;") : "")
                + (color ? ("color:"+color+";") : "")
                + ( !Qt.colorEqual(backgroundColor, "transparent") ? ("background-color:"+backgroundColor+";") : "")
                + (fontStyle ? ("font-style:"+fontStyle+";") : "")
                + (fontWeight ? ("font-weight:"+fontWeight+";") : "")
                + (textDecorLine ? ("text-decoration:"+textDecorLine+";") : "")
                + "\">"
                + textManager.escape(textString) + "</span>";

        return result.replace(/\n/g, '<br>');
    }


    function updateHtmlText() {
        var htmlString = "";

        for (var i = 0; i < textRoot.children.length; i++)
        {
            var child = textRoot.children[i];
            if(isText(child) && child.decoratedText) {
                var nestedText = child.decoratedText;
                nestedText = "<a href=\"" + i + "\" style=\"text-decoration: none\">" + nestedText + "</a>"; 
                htmlString += nestedText
            }
            else if(isRawText(child) && child.p_text) {
                htmlString += textToHtml(child.p_text)
            }
        }
        decoratedText = htmlString;
        if (textRoot.flexbox) {
            textRoot.flexbox.markDirty();
        }
    }

    function isText(obj)
    {
        if(obj && obj.typeName && obj.typeName === "ReactText")
            return true
        else
            return false
    }

    function isRawText(obj)
    {
        if(obj && obj.typeName && obj.typeName === "ReactRawText")
            return true
        else
            return false
    }
}
