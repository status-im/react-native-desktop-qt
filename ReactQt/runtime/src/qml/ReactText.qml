import QtQuick 2.4

Text {
  id: root

  property string typeName: "ReactText"
  property var textManager: null
  property bool p_allowFontScaling: false
  property string p_fontFamily
  property double p_fontSize
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
  property int p_numberOfLines: 10000000 //TODO: KOZIEIEV: change to max int value

  //ReactText components can be nested. This property indicates if item is parent
  //of current text blocks
  property bool textIsTopInBlock: parent ? (parent.typeName ? (parent.typeName === "ReactText" ? false : true) : true) : true

  maximumLineCount: p_numberOfLines
  fontSizeMode: p_allowFontScaling ? 3 : 0
  horizontalAlignment: horizontalAlignmentFromTextAlign(p_textAlign)

  function horizontalAlignmentFromTextAlign(textAlign) {
    switch (textAlign) {
        case "left":    return Text.AlignLeft;
        case "right":   return Text.AlignRight;
        case "center":  return Text.AlignHCenter;
        case "justify": return Text.AlignJustify;
        default:        return Text.AlignLeft;
    }
  }

  Component.onCompleted: {
    //console.log("maxNumberOfLines: ", maximumLineCount)
  }


  function bzz() {
    console.log("properties() called")
    return {
      asdf: "assssser"
    };
  }


//  property string decoratedText: "<span style=\"";



//"font-family:"+p_fontFamily+";"

//      "font-size:"+p_fontSize+"pt;"
//      "color:"+p_color+";"
//      "font-style:"+p_fontStyle+";"
//      "font-weight:"+p_fontWeight+";"
//      "text-decoration:"+p_textDecorationLine+";"
// "\">" + text.toHtmlEscaped() + "</span>";
//  return decoratedText;


  textFormat: Text.RichText
  wrapMode: Text.WordWrap
}
