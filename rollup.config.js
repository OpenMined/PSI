import { terser } from 'rollup-plugin-terser'
import alias from '@rollup/plugin-alias'
import commonjs from '@rollup/plugin-commonjs'
import resolve from '@rollup/plugin-node-resolve'

const targets = ['client', 'server', 'combined']
const variants = ['wasm', 'js']
const formats = ['umd', 'es']

const outputs = targets.reduce(
  (acc, target) => [
    ...acc,
    ...variants.reduce(
      (acc, variant) => [
        ...acc,
        {
          input: `private_set_intersection/javascript/tsc-out/index_${target}_${variant}.js`,
          onwarn(warning, warn) {
            // suppress eval warnings from google-protobuf
            if (warning.code === 'EVAL') return
            warn(warning)
          },
          output: formats.reduce(
            (acc, format) => [
              ...acc,
              {
                file: `private_set_intersection/javascript/dist/${target}/${variant}/${format}/index.js`,
                sourcemap: true,
                format,
                name: 'PSI',
                plugins: [terser()]
              }
            ],
            []
          ),
          plugins: [
            resolve(), // needed to include the external google-protobuf module in the bundle
            commonjs(), // needed to convert commonjs to es6 for protobuf
            alias({
              entries: [
                // Used to replace the paths that use `import * psi from './psi_*'` statement to point to their respective JS files
                {
                  find: /^psi_(.*)$/,
                  replacement:
                    './private_set_intersection/javascript/bin/psi_$1.js'
                },
                // Used to replace the `import { ServerSetup, Request, Response } from './proto/psi_pb'` statement to point to the JS file
                {
                  find: /^\.\/proto\/psi_(.*)$/,
                  replacement:
                    './private_set_intersection/javascript/bin/psi_$1.js'
                }
              ]
            })
          ]
        }
      ],
      []
    )
  ],
  []
)

export default outputs
