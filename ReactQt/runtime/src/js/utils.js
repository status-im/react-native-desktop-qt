function alignmentQMLValue(jsValue) {
    if (jsValue === "center") {
        return "AlignHCenter";
    } else if (jsValue === "left"){
        return "AlignLeft";
    } else {
        return "AlignRight";
    }
}
