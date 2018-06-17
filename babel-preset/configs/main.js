/**
 * Copyright (c) 2015-present, Facebook, Inc.
 *
<<<<<<< HEAD
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */
'use strict';

var resolvePlugins = require('../lib/resolvePlugins');

const getPreset = (src, options) => {
  const plugins = [];
=======
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
'use strict';

const resolvePlugins = require('../lib/resolvePlugins');
const resolvePlugin = resolvePlugins.resolvePlugin;

const defaultPlugins = resolvePlugins([
  'syntax-class-properties',
  'syntax-trailing-function-commas',
  'transform-class-properties',
  'transform-es2015-block-scoping',
  'transform-es2015-computed-properties',
  'transform-es2015-destructuring',
  'transform-es2015-function-name',
  'transform-es2015-literals',
  'transform-es2015-parameters',
  'transform-es2015-shorthand-properties',
  'transform-flow-strip-types',
  'transform-react-jsx',
  'transform-regenerator',
  [
    'transform-es2015-modules-commonjs',
    {strict: false, allowTopLevelThis: true},
  ],
]);

const checkES2015Constants = resolvePlugin('check-es2015-constants');
const es2015ArrowFunctions = resolvePlugin('transform-es2015-arrow-functions');
const es2015Classes = resolvePlugin('transform-es2015-classes');
const es2015ForOf = resolvePlugin(['transform-es2015-for-of', {loose: true}]);
const es2015Spread = resolvePlugin('transform-es2015-spread');
const es2015TemplateLiterals = resolvePlugin(
  'transform-es2015-template-literals'
);
const asyncFunctions = resolvePlugin('syntax-async-functions');
const exponentiationOperator = resolvePlugin(
  'transform-exponentiation-operator'
);
const objectAssign = resolvePlugin('transform-object-assign');
const objectRestSpread = resolvePlugin('transform-object-rest-spread');
const reactDisplayName = resolvePlugin('transform-react-display-name');
const reactJsxSource = resolvePlugin('transform-react-jsx-source');
const symbolMember = [require('../transforms/transform-symbol-member')];

const getPreset = (src, options) => {
>>>>>>> v0.55.4
  const isNull = src === null || src === undefined;
  const hasClass = isNull || src.indexOf('class') !== -1;
  const hasForOf =
    isNull || (src.indexOf('for') !== -1 && src.indexOf('of') !== -1);

<<<<<<< HEAD
  plugins.push(
    'syntax-class-properties',
    'syntax-trailing-function-commas',
    'transform-class-properties',
    'transform-es2015-block-scoping',
    'transform-es2015-computed-properties',
    'transform-es2015-destructuring',
    'transform-es2015-function-name',
    'transform-es2015-literals',
    'transform-es2015-parameters',
    'transform-es2015-shorthand-properties',
    'transform-flow-strip-types',
    'transform-react-jsx',
    'transform-regenerator',
    [
      'transform-es2015-modules-commonjs',
      {strict: false, allowTopLevelThis: true},
    ]
  );

  if (isNull || src.indexOf('async') !== -1 || src.indexOf('await') !== -1) {
    plugins.push('syntax-async-functions');
  }
  if (hasClass) {
    plugins.push('transform-es2015-classes');
  }
  if (isNull || src.indexOf('=>') !== -1) {
    plugins.push('transform-es2015-arrow-functions');
  }
  if (isNull || src.indexOf('const') !== -1) {
    plugins.push('check-es2015-constants');
  }
  if (isNull || hasClass || src.indexOf('...') !== -1) {
    plugins.push('transform-es2015-spread');
    plugins.push('transform-object-rest-spread');
  }
  if (isNull || src.indexOf('`') !== -1) {
    plugins.push('transform-es2015-template-literals');
  }
  if (isNull || src.indexOf('**') !== -1) {
    plugins.push('transform-exponentiation-operator');
  }
  if (isNull || src.indexOf('Object.assign') !== -1) {
    plugins.push('transform-object-assign');
  }
  if (hasForOf) {
    plugins.push(['transform-es2015-for-of', {loose: true}]);
  }
  if (hasForOf || src.indexOf('Symbol') !== -1) {
    plugins.push(require('../transforms/transform-symbol-member'));
=======
  const extraPlugins = [];

  if (isNull || src.indexOf('async') !== -1 || src.indexOf('await') !== -1) {
    extraPlugins.push(asyncFunctions);
  }
  if (hasClass) {
    extraPlugins.push(es2015Classes);
  }
  if (isNull || src.indexOf('=>') !== -1) {
    extraPlugins.push(es2015ArrowFunctions);
  }
  if (isNull || src.indexOf('const') !== -1) {
    extraPlugins.push(checkES2015Constants);
  }
  if (isNull || hasClass || src.indexOf('...') !== -1) {
    extraPlugins.push(es2015Spread);
    extraPlugins.push(objectRestSpread);
  }
  if (isNull || src.indexOf('`') !== -1) {
    extraPlugins.push(es2015TemplateLiterals);
  }
  if (isNull || src.indexOf('**') !== -1) {
    extraPlugins.push(exponentiationOperator);
  }
  if (isNull || src.indexOf('Object.assign') !== -1) {
    extraPlugins.push(objectAssign);
  }
  if (hasForOf) {
    extraPlugins.push(es2015ForOf);
  }
  if (hasForOf || src.indexOf('Symbol') !== -1) {
    extraPlugins.push(symbolMember);
>>>>>>> v0.55.4
  }
  if (
    isNull ||
    src.indexOf('React.createClass') !== -1 ||
    src.indexOf('createReactClass') !== -1
  ) {
<<<<<<< HEAD
    plugins.push('transform-react-display-name');
  }

  if (options && options.dev) {
    plugins.push('transform-react-jsx-source');
=======
    extraPlugins.push(reactDisplayName);
  }

  if (options && options.dev) {
    extraPlugins.push(reactJsxSource);
>>>>>>> v0.55.4
  }

  return {
    comments: false,
    compact: true,
<<<<<<< HEAD
    plugins: resolvePlugins(plugins),
  };
};

const base = getPreset(null);
const devTools = getPreset(null, {dev: true});

module.exports = options => {
  if (options.withDevTools == null) {
    const env = process.env.BABEL_ENV || process.env.NODE_ENV;
    if (!env || env === 'development') {
      return devTools;
    }
  }
  return base;
};

=======
    plugins: defaultPlugins.concat(extraPlugins),
  };
};

const base = getPreset(null);
const devTools = getPreset(null, {dev: true});

module.exports = options => {
  if (options.withDevTools == null) {
    const env = process.env.BABEL_ENV || process.env.NODE_ENV;
    if (!env || env === 'development') {
      return devTools;
    }
  }
  return base;
};

>>>>>>> v0.55.4
module.exports.getPreset = getPreset;
