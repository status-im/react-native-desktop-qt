var path = require("path");
const defaultResolvePath = require('babel-plugin-module-resolver').resolvePath;
const generator = require("./babel/babel-config-generator.js");

module.exports = generator.create(path.resolve("./node_modules/react-native"), path.resolve("."));
