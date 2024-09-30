import alias from '@rollup/plugin-alias'
import commonjs from '@rollup/plugin-commonjs'
import resolve from '@rollup/plugin-node-resolve'
import replace from '@rollup/plugin-replace'
import terser from '@rollup/plugin-terser'
import builtins from 'rollup-plugin-node-builtins'
import typescript from 'rollup-plugin-typescript2'

const outputs = [
  {
    input: `private_set_intersection/javascript/src/wasm_node.ts`,
    onwarn(warning, warn) {
      // suppress eval warnings from google-protobuf
      if (warning.code === 'EVAL') return
      warn(warning)
    },
    output: {
      file: `private_set_intersection/javascript/dist/psi_wasm_node.js`,
      sourcemap: true,
      format: 'cjs',
      name: 'PSI',
      exports: 'auto',
      plugins: [terser()]
    },
    plugins: [
      // UGLY hack to 'fix' nodejs complaining about 'window' and 'self'
      replace({
        'return this || window || global || self ||':
          'return this || global ||',
        delimiters: ['', ''],
        preventAssignment: true
      }),
      // Order of plugins matters!
      // Depending on the format, we need to correctly transform the build for brower or nodejs
      resolve({ preferBuiltins: true }), // needed to include the external google-protobuf module in the bundle
      commonjs(), // needed to convert commonjs to es6 for protobuf
      alias({
        entries: [
          // Used to replace the paths that use `import * psi from './psi_*'` statement to point to their respective JS files
          {
            find: /^psi_(.*)$/,
            replacement: './private_set_intersection/javascript/bin/psi_$1.js'
          },
          // Used to replace the `import { ServerSetup, Request, Response } from './proto/psi_pb'` statement to point to the JS file
          {
            find: /^\.\/proto\/psi_(.*)$/,
            replacement: './private_set_intersection/javascript/bin/psi_$1.js'
          }
        ]
      }),
      typescript({
        verbosity: 2
      })
    ]
  },
  {
    input: `private_set_intersection/javascript/src/wasm_web.ts`,
    onwarn(warning, warn) {
      // suppress eval warnings from google-protobuf
      if (warning.code === 'EVAL') return
      warn(warning)
    },
    output: {
      file: `private_set_intersection/javascript/dist/psi_wasm_web.js`,
      sourcemap: true,
      format: 'umd',
      name: 'PSI',
      exports: 'auto',
      plugins: [terser()]
    },
    plugins: [
      // Order of plugins matters!
      // Depending on the format, we need to correctly transform the build for brower or nodejs
      resolve({ preferBuiltins: true }), // needed to include the external google-protobuf module in the bundle
      commonjs(), // needed to convert commonjs to es6 for protobuf
      builtins({ fs: true }),
      alias({
        entries: [
          // Used to replace the paths that use `import * psi from './psi_*'` statement to point to their respective JS files
          {
            find: /^psi_(.*)$/,
            replacement: './private_set_intersection/javascript/bin/psi_$1.js'
          },
          // Used to replace the `import { ServerSetup, Request, Response } from './proto/psi_pb'` statement to point to the JS file
          {
            find: /^\.\/proto\/psi_(.*)$/,
            replacement: './private_set_intersection/javascript/bin/psi_$1.js'
          }
        ]
      }),
      typescript({
        verbosity: 2
      })
    ]
  },
  {
    input: `private_set_intersection/javascript/src/wasm_worker.ts`,
    onwarn(warning, warn) {
      // suppress eval warnings from google-protobuf
      if (warning.code === 'EVAL') return
      warn(warning)
    },
    output: {
      file: `private_set_intersection/javascript/dist/psi_wasm_worker.js`,
      sourcemap: true,
      format: 'umd',
      name: 'PSI',
      exports: 'auto',
      plugins: [terser()]
    },
    plugins: [
      // Order of plugins matters!
      // Depending on the format, we need to correctly transform the build for brower or nodejs
      resolve({ preferBuiltins: true }), // needed to include the external google-protobuf module in the bundle
      commonjs(), // needed to convert commonjs to es6 for protobuf
      builtins({ fs: true }),
      alias({
        entries: [
          // Used to replace the paths that use `import * psi from './psi_*'` statement to point to their respective JS files
          {
            find: /^psi_(.*)$/,
            replacement: './private_set_intersection/javascript/bin/psi_$1.js'
          },
          // Used to replace the `import { ServerSetup, Request, Response } from './proto/psi_pb'` statement to point to the JS file
          {
            find: /^\.\/proto\/psi_(.*)$/,
            replacement: './private_set_intersection/javascript/bin/psi_$1.js'
          }
        ]
      }),
      typescript({
        verbosity: 2
      })
    ]
  }
]

export default outputs
