import QtQuick 2.4
import React 0.1 as React

Text {
    id: textRoot

    property bool p_allowFontScaling: false
    property string p_fontFamily
    property double p_fontSize: font.pointSize
    property bool p_highlighted: false
    property color p_color
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
    property var flexbox: React.Flexbox {control: textRoot}



    property string typeName: "ReactText"
    property var textManager: null
    property string decoratedText
    //ReactText components can be nested. This property indicates if item is parent
    //of current text blocks
    property bool textIsTopInBlock: parent ? (parent.typeName ? (parent.typeName === "ReactText" ? false : true) : true) : true
    onTextIsTopInBlockChanged: updateMeasureFunction()
    onTextManagerChanged: updateMeasureFunction()


    text: textIsTopInBlock ? decoratedText : ""
    maximumLineCount: p_numberOfLines
    fontSizeMode: p_allowFontScaling ? 3 : 0
    horizontalAlignment: horizontalAlignmentFromTextAlign(p_textAlign)
    textFormat: Text.RichText
    wrapMode: Text.WordWrap


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
        var fontWeight = textManager.nestedPropertyValue(textRoot, "p_fontWeight")
        var fontSize = textManager.nestedPropertyValue(textRoot, "p_fontSize")
        var fontStyle = textManager.nestedPropertyValue(textRoot, "p_fontStyle")
        var textDecorLine = textManager.nestedPropertyValue(textRoot, "p_textDecorationLine")

        return "<span style=\"" + (fontFamily ? ("font-family:"+fontFamily+";") : "")
                + (fontSize? ("font-size:"+fontSize+"pt;") : "")
                + (color ? ("color:"+color+";") : "")
                + (fontStyle ? ("font-style:"+fontStyle+";") : "")
                + (fontWeight ? ("font-weight:"+fontWeight+";") : "")
                + (textDecorLine ? ("text-decoration:"+textDecorLine+";") : "")
                + "\">"
                + textString + "</span>";
    }


    function updateHtmlText() {
        var htmlString = "";

        for (var i = 0; i < textRoot.children.length; i++)
        {
            var child = textRoot.children[i];
            if(isText(child) && child.decoratedText) {
                htmlString += child.decoratedText
            }
            else if(isRawText(child) && child.p_text) {
                htmlString += textToHtml(child.p_text)
            }
        }
        decoratedText = htmlString;
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
