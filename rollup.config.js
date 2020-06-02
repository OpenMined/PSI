import { terser } from 'rollup-plugin-terser'
import alias from '@rollup/plugin-alias'

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
            alias({
              entries: [
                {
                  find: /^psi_(.*)$/,
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
