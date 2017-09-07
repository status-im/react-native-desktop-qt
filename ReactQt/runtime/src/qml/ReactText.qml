import QtQuick 2.4

Text {
  id: textRoot

  property bool p_allowFontScaling: (parent && parent.p_allowFontScaling && parent.p_allowFontScaling !== false) ? parent.p_allowFontScaling : p_allowFontScaling
  property string p_fontFamily: (parent && parent.p_fontFamily) ? parent.p_fontFamily : p_fontFamily
  property double p_fontSize: (parent && parent.p_fontSize && parent.p_fontSize !== font.pointSize) ? parent.p_fontSize : p_fontSize
  property bool p_highlighted: (parent && parent.p_highlighted && parent.p_highlighted !== false) ? parent.p_highlighted : p_highlighted
  property color p_color: (parent && parent.p_color && parent.p_color !== Qt.rgba(0, 0, 0, 1)) ? parent.p_color : Qt.rgba(0, 0, 0, 1)
  property string p_fontStyle: (parent && parent.p_fontStyle) ? parent.p_fontStyle : p_fontStyle
  property string p_fontWeight: (parent && parent.p_fontWeight) ? parent.p_fontWeight : p_fontWeight
  property double p_letterSpacing: (parent && parent.p_letterSpacing) ? parent.p_letterSpacing : p_letterSpacing
  property double p_lineHeight: (parent && parent.p_lineHeight) ? parent.p_lineHeight : p_lineHeight
  property string p_textAlign: (parent && parent.p_textAlign) ? parent.p_textAlign : p_textAlign
  property string p_textDecorationLine: (parent && parent.p_textDecorationLine) ? parent.p_textDecorationLine : p_textDecorationLine
  property string p_textDecorationStyle: (parent && parent.p_textDecorationStyle) ? parent.p_textDecorationStyle : p_textDecorationStyle
  property color p_textDecorationColor: (parent && parent.p_textDecorationColor /*&& parent.p_textDecorationColor !== color("#000000")*/) ? parent.p_textDecorationColor : p_textDecorationColor
  property string p_writingDirection: (parent && parent.p_writingDirection) ? parent.p_writingDirection : p_writingDirection
  property int p_numberOfLines: (parent && parent.p_numberOfLines && parent.p_numberOfLines !== 1000000) ? parent.p_numberOfLines : 1000000

  property string typeName: "ReactText"
  property var textManager: null
  property string decoratedText
  //ReactText components can be nested. This property indicates if item is parent
  //of current text blocks
  property bool textIsTopInBlock: parent ? (parent.typeName ? (parent.typeName === "ReactText" ? false : true) : true) : true


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
    return "<span style=\"" + (p_fontFamily ? ("font-family:"+p_fontFamily+";") : "")
                            + (p_fontSize ? ("font-size:"+p_fontSize+"pt;") : "")
                            + (p_color ? ("color:"+p_color+";") : "")
                            + (p_fontStyle ? ("font-style:"+p_fontStyle+";") : "")
                            + (p_fontWeight ? ("font-weight:"+p_fontWeight+";") : "")
                            + (p_textDecorationLine ? ("text-decoration:"+p_textDecorationLine+";") : "")
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
